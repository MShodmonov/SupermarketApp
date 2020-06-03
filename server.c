#include<stdio.h>
#include<string.h> //strlen
#include<stdlib.h> //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h> //write
#include<pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <mysql.h>
#include <netinet/tcp.h>

struct users
{
    char id[15];
    char fullname[45];
    char username[20];
    char password[20];
    char role[5];
    char soldProduct[20]
    
};
struct product
{
    char id[15];
    char name[45];
    char barcode[45];
    char quantity[15];
    char price[20];
    char categoryId[20];
    char userId[20];
    char location[45];
};
struct category
{
    char id[11];
    char name[45];
};
struct product updateProductInfo(struct product item, MYSQL *connection);
struct product selectProductInfo(char *product_id, MYSQL *connection);
struct product createProduct(struct product item, MYSQL *connection);
struct product deleteProduct(char * product_id,MYSQL *connection);

void *connection_handler(void *socket_desc);

struct users updateUserInfo(struct users user, MYSQL *connection);
struct users selectUserInfo(char *user_id, MYSQL *connection);
struct users createUser(struct users user, MYSQL *connection);
struct users deleteUser(char * user_id,MYSQL *connection);

struct category updateCategoryInfo(struct category cat, MYSQL *connection);
struct category selectCategoryInfo(char *cat_id, MYSQL *connection);
struct category createCategory(struct category cat, MYSQL *connection);
struct category deleteCategory(char * cat_id,MYSQL *connection);
struct category simulateMainCategory(int crud_choice,struct category cat);

struct users simulateMain(int crud_choice,struct users user_main);
struct users createAdmin(struct users user, MYSQL *connection);
struct product simulateMainItem(int crud_choice,struct product item);
struct product *selectAllProducts(MYSQL *connection);
void incrementUserProductIndicator(struct product item,struct users user,MYSQL *connection);
MYSQL *getConnection();

int main(int argc , char *argv[])
    {
    int socket_desc , new_socket , c , *new_sock;
    struct sockaddr_in server , client;
    char *message;
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
        
        if (socket_desc == -1)
        {
            printf("Could not create socket");
        }
        int flag = 1;
        int result = setsockopt(socket_desc,IPPROTO_TCP | SO_REUSEADDR,TCP_NODELAY, &flag, sizeof(int));
    //Prepare the sockaddr_in structure
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = INADDR_ANY;
        server.sin_port = htons( 9998 );
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
        puts("bind done");
   
        //Listen
        listen(socket_desc , 3);
        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        while(1){
        while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
        {
            
            pthread_t sniffer_thread;
            new_sock = (int*)malloc(sizeof(int));
            *new_sock = new_socket;
            if( pthread_create( &sniffer_thread , NULL , connection_handler , (void*) new_sock) < 0)
            {
                perror("could not create thread");
                free(socket_desc);
                return 1;
            }
            puts("Handler assigned");
        }
        if (new_socket<0) {
            free(socket_desc);
            perror("accept failed");
            return 1;
        }
        free(socket_desc);
        return 0;
        }
    }

void *connection_handler(void *socket_desc)
    {
        int sock = *(int*)socket_desc;
        char client_choice='0';
        char schat[2000], s1[15],quit[4];
        char s[3];
        struct users user_struct;
        struct product product_struct;
        struct category category_struct;
       
        char *user_id = (char *) malloc(15 * sizeof(char));
        char *userfullname = (char *) malloc(45 * sizeof(char));
        char *username = (char *) malloc(20 * sizeof(char));
        char *userpassword = (char *) malloc(20 * sizeof(char));
        char *userrole = (char *) malloc(5 * sizeof(char));
        
        char *product_id = (char *) malloc(15 * sizeof(char));
        char *product_name = (char *) malloc(45 * sizeof(char));
        char *product_barcode = (char *) malloc(45 * sizeof(char));
        char *product_quantity = (char *) malloc(15 * sizeof(char));
        char *product_price = (char *) malloc(20 * sizeof(char));
        char *product_categoryId = (char *) malloc(20 * sizeof(char));
        char *product_userId = (char *) malloc(20 * sizeof(char));
        char *product_location = (char *) malloc(45 * sizeof(char));
        
        char *category_id = (char *) malloc(15 * sizeof(char));
        char *category_name = (char *) malloc(45 * sizeof(char));
        char *sold_product = (char *) malloc(20 * sizeof(char));
        int success=0,selectSize=0,num_fields;
        int num_rows = (int *) malloc( sizeof(int));
        char num_rows_char[5];
        
        char *query_s=(char *) malloc(1024 * sizeof(char));
        char *query_template_s=(char *) malloc(150 * sizeof(char));
        MYSQL_RES *res=NULL;
        MYSQL * con=NULL;
        struct product *itemList=NULL;
        struct users *userList=NULL;
        MYSQL_ROW row;
        int num;
        
        
       
        


        while(1){
           memset(schat, 0, sizeof(schat));
           memset(s, 0, sizeof(s));
           memset(s1, 0, sizeof(s1));
           strcpy(schat, "please enter 1 for select user\n 2 for update user small bug\n 3 for create user\n 4 for delete user\n 5 to create admin user \n 6 for select product \n 7 for update product\n 8 for create product\n 9 for delete product by name \n 10 for delete product by id\n 11 for select product by id\n 12 for select category by name\n 13 for update catedory name with id\n 14 for create category\n 15 for delete category by name\n 16 for authentication \n 17 for select all products \n 25 for exit \n 18 for select product by username\n ");
           send(sock ,schat ,strlen(schat),0);

   
                read(sock, s, 10);
                num = atoi(s);
                memset(user_id, 0, sizeof(user_id));
                memset(userfullname, 0, sizeof(userfullname));
                memset(username, 0, sizeof(username));
                memset(userpassword, 0, sizeof(userpassword));
                memset(userrole, 0, sizeof(userrole));
                memset(sold_product, 0, sizeof(sold_product));
                memset(product_id, 0, sizeof(product_id));
                memset(product_name, 0, sizeof(product_name));
                memset(product_barcode, 0, sizeof(product_barcode));
                memset(product_quantity, 0, sizeof(product_quantity));
                memset(product_price, 0, sizeof(product_price));
                memset(product_categoryId, 0, sizeof(product_categoryId));
                memset(product_userId, 0, sizeof(product_userId));
             memset(product_location, 0, sizeof(product_location));
                memset(category_id, 0, sizeof(category_id));
                memset(category_name, 0, sizeof(category_name));
                memset(schat, 0, sizeof(schat));
                memset(quit, 0, sizeof(quit));
             memset(user_struct.id, 0, sizeof(user_struct.id));
             memset(user_struct.username, 0, sizeof(user_struct.username));
             memset(user_struct.fullname, 0, sizeof(user_struct.fullname));
             memset(user_struct.password, 0, sizeof(user_struct.password));
             memset(user_struct.role, 0, sizeof(user_struct.role));
            
             memset(product_struct.id, 0, sizeof(product_struct.id));
             memset(product_struct.name, 0, sizeof(product_struct.name));
            memset(product_struct.barcode, 0, sizeof(product_struct.barcode));
             memset(product_struct.quantity, 0, sizeof(product_struct.quantity));
             memset(product_struct.price, 0, sizeof(product_struct.price));
             memset(product_struct.userId, 0, sizeof(product_struct.userId));
            memset(product_struct.categoryId, 0, sizeof(product_struct.categoryId));
            
            memset(category_struct.id, 0, sizeof(category_struct.id));
            memset(category_struct.name, 0, sizeof(category_struct.name));
           


            
                printf("%d\n",num);

               switch (num) {
                   case 1:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter username\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,username,sizeof(username));
                       printf("%s\n",username);
                       strcpy(user_struct.username,username);
                       printf(user_struct.username);
                       user_struct = simulateMain(num,user_struct);
                       strcpy(user_id,user_struct.id);
                       strcpy(userfullname,user_struct.fullname);
                       strcpy(username,user_struct.username);
                       strcpy(userpassword,user_struct.password);
                        strcpy(userrole,user_struct.role);
                       strcpy(sold_product,user_struct.soldProduct);
                       printf("%s\n",user_id);
                       printf("%s\n",userfullname);
                       printf("%s\n",username);
                       printf("%s\n",userpassword);
                       printf("%s\n",userrole);
                        printf("%s\n",sold_product);
                        send(sock ,user_id ,sizeof(user_id),0);
                        send(sock ,userfullname ,sizeof(userfullname),0);
                        send(sock ,username ,sizeof(username),0);
                       send(sock ,userpassword ,sizeof(userpassword),0);
                       send(sock ,userrole ,sizeof(userrole),0);
                       send(sock ,sold_product ,sizeof(sold_product),0);
                       break;
                   case 2:
                            memset(schat, 0, sizeof(schat));
                            strcpy(schat,"please enter  fullname \n");
                            send(sock ,schat ,strlen(schat),0);
                            read(sock,userfullname,sizeof(userfullname));
                            strcpy(user_struct.fullname,userfullname);
                            memset(schat, 0, sizeof(schat));
                            strcpy(schat,"please enter username \n");
                            send(sock ,schat ,strlen(schat),0);
                            read(sock,username,sizeof(username));
                            strcpy(user_struct.username,username);
                            memset(schat, 0, sizeof(schat));
                            strcpy(schat,"please enter  password\n");
                            send(sock ,schat ,strlen(schat),0);
                            read(sock,userpassword,sizeof(userpassword));
                            strcpy(user_struct.password,userpassword);
                            memset(schat, 0, sizeof(schat));
                                user_struct = simulateMain(2,user_struct);
                                if (strlen(user_struct.username) == 0) {
                                memset(username, 0, sizeof(username));
                                memset(userpassword, 0, sizeof(userpassword));
                                memset(userfullname, 0, sizeof(userfullname));
                                memset(userrole, 0, sizeof(userrole));
                                send(sock ,username ,sizeof(username),0);
                                send(sock ,userpassword ,sizeof(userpassword),0);
                                send(sock ,userfullname ,sizeof(userfullname),0);
                                send(sock ,userrole ,sizeof(userrole),0);
                                    }
                            else{
                                strcpy(username,user_struct.username);
                                strcpy(userpassword,user_struct.password);
                                strcpy(userfullname,user_struct.fullname);
                                strcpy(userrole,user_struct.role);
                                printf("%s\n",username);
                                printf("%s\n",userpassword);
                                printf("%s\n",userfullname);
                                printf("%s\n",userrole);
                                send(sock ,username ,sizeof(username),0);
                                send(sock ,userpassword ,sizeof(userpassword),0);
                                send(sock ,userfullname ,sizeof(userfullname),0);
                                send(sock ,userrole ,sizeof(userrole),0);
                                }
                       break;
                   case 3:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter username \n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,username,sizeof(username));
                       strcpy(user_struct.username,username);
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter user password\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,userpassword,sizeof(userpassword));
                       strcpy(user_struct.password,userpassword);
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter full name \n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,userfullname,sizeof(userfullname));
                       strcpy(user_struct.fullname,userfullname);
                       memset(schat, 0, sizeof(schat));
                       user_struct = simulateMain(3,user_struct);
                       strcpy(username,user_struct.username);
                       strcpy(userpassword,user_struct.password);
                       strcpy(userfullname,user_struct.fullname);
                       strcpy(userrole,user_struct.role);
                       printf("%s\n",username);
                       printf("%s\n",userpassword);
                       printf("%s\n",userfullname);
                       printf("%s\n",userrole);
                       send(sock ,username ,sizeof(username),0);
                       send(sock ,userpassword ,sizeof(userpassword),0);
                       send(sock ,userfullname ,sizeof(userfullname),0);
                       send(sock ,userrole ,sizeof(userrole),0);
                       break;
                   case 4:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter username\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,username,sizeof(username));
                       strcpy(user_struct.username,username);
                       user_struct = simulateMain(4,user_struct);
                       break;
                   case 5:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter username \n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,username,sizeof(username));
                       strcpy(user_struct.username,username);
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter user password\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,userpassword,sizeof(userpassword));
                       strcpy(user_struct.password,userpassword);
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter full name \n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,userfullname,sizeof(userfullname));
                       strcpy(user_struct.fullname,userfullname);
                       memset(schat, 0, sizeof(schat));
                       user_struct = simulateMain(5,user_struct);
                       strcpy(username,user_struct.username);
                       strcpy(userpassword,user_struct.password);
                       strcpy(userfullname,user_struct.fullname);
                       strcpy(userrole,user_struct.role);
                       printf("%s\n",username);
                       printf("%s\n",userpassword);
                       printf("%s\n",userfullname);
                       printf("%s\n",userrole);
                       send(sock ,username ,sizeof(username),0);
                        send(sock ,userpassword ,sizeof(userpassword),0);
                        send(sock ,userfullname ,sizeof(userfullname),0);
                        send(sock ,userrole ,sizeof(userrole),0);
                        break;
                   case 6:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter product name\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,product_name,sizeof(product_name));
                       strcpy(product_struct.name,product_name);
                       printf(product_struct.name);
                       product_struct = simulateMainItem(1,product_struct);
                      if (strlen(product_struct.id) == 0) {
                           memset(product_name, 0, sizeof(product_name));
                           memset(product_id, 0, sizeof(product_id));
                           memset(product_barcode, 0, sizeof(product_barcode));
                           memset(product_quantity, 0, sizeof(product_quantity));
                             memset(product_price, 0, sizeof(product_price));
                             memset(product_categoryId, 0, sizeof(product_categoryId));
                            memset(product_userId, 0, sizeof(product_userId));
                          memset(product_location, 0, sizeof(product_location));
                            send(sock ,product_id ,sizeof(product_id),0);
                           send(sock ,product_name,sizeof(product_name),0);
                           send(sock ,product_barcode ,sizeof(product_barcode),0);
                           send(sock ,product_price,sizeof(product_price),0);
                           send(sock ,product_quantity ,sizeof(product_quantity),0);
                           send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                           send(sock ,product_userId ,sizeof(product_userId),0);
                          send(sock ,product_location ,sizeof(product_location),0);
                                    }
                       else{
                           strcpy(product_id,product_struct.id);
                           strcpy(product_name,product_struct.name);
                           strcpy(product_barcode,product_struct.barcode);
                           strcpy(product_price,product_struct.price);
                           strcpy(product_quantity,product_struct.quantity);
                       strcpy(product_categoryId,product_struct.categoryId);
                           strcpy(product_userId,product_struct.userId);
                           printf("%s\n",product_id);
                           printf("%s\n",product_name);
                           printf("%s\n",product_barcode);
                           printf("%s\n",product_price);
                           printf("%s\n",product_quantity);
                           printf("%s\n",product_categoryId);
                           printf("%s\n",product_userId);
                           printf("%s\n",product_location);
                           send(sock ,product_id ,sizeof(product_id),0);
                           send(sock ,product_name,sizeof(product_name),0);
                           send(sock ,product_barcode ,sizeof(product_barcode),0);
                           send(sock ,product_price,sizeof(product_price),0);
                           send(sock ,product_quantity ,sizeof(product_quantity),0);
                           send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                           send(sock ,product_location ,sizeof(product_location),0);
                       }
                       break;
                   case 7:
                    memset(schat, 0, sizeof(schat));
                   strcpy(schat,"please enter  product name \n");
                    send(sock ,schat ,strlen(schat),0);
                    read(sock,product_name,sizeof(product_name));
                    strcpy(product_struct.name,product_name);
                    memset(schat, 0, sizeof(schat));
                    strcpy(schat,"please enter product barcode \n");
                    send(sock ,schat ,strlen(schat),0);
                    read(sock,product_barcode,sizeof(product_barcode));
                    strcpy(product_struct.barcode,product_barcode);
                   memset(schat, 0, sizeof(schat));
                    strcpy(schat,"please enter  product quantity\n");
                    send(sock ,schat ,strlen(schat),0);
                   read(sock,product_quantity,sizeof(product_quantity));
                    strcpy(product_struct.quantity,product_quantity);
                    memset(schat, 0, sizeof(schat));
                   strcpy(schat,"please enter  product price\n");
                    send(sock ,schat ,strlen(schat),0);
                     read(sock,product_price,sizeof(product_price));
                   strcpy(product_struct.price,product_price);
                   memset(schat, 0, sizeof(schat));
                    strcpy(schat,"please enter  product category name\n");
                    send(sock ,schat ,strlen(schat),0);
                     read(sock,category_name,sizeof(category_name));
                     memset(schat, 0, sizeof(schat));
                    strcpy(schat,"please enter user name\n");
                    send(sock ,schat ,strlen(schat),0);
                    read(sock,username,sizeof(username));
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter product location\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,product_location,sizeof(product_location));
                       strcpy(product_struct.location,product_location);
                     memset(user_struct.username, 0, sizeof(user_struct.username));
                     strcpy(user_struct.username,username);
                     user_struct = simulateMain(1,user_struct);
                     memset(product_struct.userId, 0, sizeof(product_struct.userId));
                     strcpy(product_struct.userId,user_struct.id);
                     printf("\n%s\n",product_struct.userId);
                    strcpy(category_struct.name,category_name);
                    category_struct = simulateMainCategory(1,category_struct);
                     memset(product_struct.categoryId, 0, sizeof(product_struct.categoryId));
                     printf("\n%s\n",category_struct.id);
                     strcpy(product_struct.categoryId,category_struct.id);
                     printf("\n%s\n",product_struct.categoryId);
                    product_struct = simulateMainItem(2,product_struct);
                     printf("due to main switch case\n");
                    if (strlen(product_struct.id) == 0) {
                        memset(product_name, 0, sizeof(product_name));
                        memset(product_id, 0, sizeof(product_id));
                        memset(product_barcode, 0, sizeof(product_barcode));
                        memset(product_quantity, 0, sizeof(product_quantity));
                          memset(product_price, 0, sizeof(product_price));
                          memset(product_categoryId, 0, sizeof(product_categoryId));
                         memset(product_userId, 0, sizeof(product_userId));
                        memset(product_location, 0, sizeof(product_location));
                         send(sock ,product_id ,sizeof(product_id),0);
                        send(sock ,product_name,sizeof(product_name),0);
                        send(sock ,product_barcode ,sizeof(product_barcode),0);
                        send(sock ,product_price,sizeof(product_price),0);
                        send(sock ,product_quantity ,sizeof(product_quantity),0);
                        send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                        send(sock ,product_userId ,sizeof(product_userId),0);
                        send(sock ,product_location ,sizeof(product_location),0);
                    }
                    else{
                        strcpy(product_id,product_struct.id);
                        strcpy(product_name,product_struct.name);
                        strcpy(product_barcode,product_struct.barcode);
                        strcpy(product_price,product_struct.price);
                        strcpy(product_quantity,product_struct.quantity);
                        strcpy(product_categoryId,product_struct.categoryId);
                        strcpy(product_userId,product_struct.userId);
                        strcpy(product_location,product_struct.location);
                        printf("%s\n",product_id);
                        printf("%s\n",product_name);
                        printf("%s\n",product_barcode);
                        printf("%s\n",product_price);
                        printf("%s\n",product_quantity);
                        printf("%s\n",product_categoryId);
                        printf("%s\n",product_userId);
                        printf("%s\n",product_location);
                        send(sock ,product_id ,sizeof(product_id),0);
                        send(sock ,product_name,sizeof(product_name),0);
                        send(sock ,product_barcode ,sizeof(product_barcode),0);
                        send(sock ,product_price,sizeof(product_price),0);
                        send(sock ,product_quantity ,sizeof(product_quantity),0);
                        send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                        send(sock ,product_userId ,sizeof(product_userId),0);
                        send(sock ,product_location ,sizeof(product_location),0);
                   }
                       break;
                   case 8:
                            memset(schat, 0, sizeof(schat));
                          strcpy(schat,"please enter  product name \n");
                           send(sock ,schat ,strlen(schat),0);
                           read(sock,product_name,sizeof(product_name));
                           strcpy(product_struct.name,product_name);
                           memset(schat, 0, sizeof(schat));
                           strcpy(schat,"please enter product barcode \n");
                           send(sock ,schat ,strlen(schat),0);
                           read(sock,product_barcode,sizeof(product_barcode));
                           strcpy(product_struct.barcode,product_barcode);
                          memset(schat, 0, sizeof(schat));
                           strcpy(schat,"please enter  product quantity\n");
                           send(sock ,schat ,strlen(schat),0);
                          read(sock,product_quantity,sizeof(product_quantity));
                           strcpy(product_struct.quantity,product_quantity);
                           memset(schat, 0, sizeof(schat));
                          strcpy(schat,"please enter  product price\n");
                           send(sock ,schat ,strlen(schat),0);
                            read(sock,product_price,sizeof(product_price));
                          strcpy(product_struct.price,product_price);
                          memset(schat, 0, sizeof(schat));
                           strcpy(schat,"please enter  product category name\n");
                           send(sock ,schat ,strlen(schat),0);
                            read(sock,category_name,sizeof(category_name));
                            memset(schat, 0, sizeof(schat));
                           strcpy(schat,"please enter user name\n");
                           send(sock ,schat ,strlen(schat),0);
                           read(sock,username,sizeof(username));
                            memset(schat, 0, sizeof(schat));
                            strcpy(schat,"please enter  product location\n");
                            send(sock ,schat ,strlen(schat),0);
                            read(sock,product_location,sizeof(product_location));
                            strcpy(product_struct.location,product_location);
                            memset(user_struct.username, 0, sizeof(user_struct.username));
                            strcpy(user_struct.username,username);
                            user_struct = simulateMain(1,user_struct);
                            memset(product_struct.userId, 0, sizeof(product_struct.userId));
                            strcpy(product_struct.userId,user_struct.id);
                            printf("\n%s\n",product_struct.userId);
                           strcpy(category_struct.name,category_name);
                           category_struct = simulateMainCategory(1,category_struct);
                            memset(product_struct.categoryId, 0, sizeof(product_struct.categoryId));
                            printf("\n%s\n",category_struct.id);
                            strcpy(product_struct.categoryId,category_struct.id);
                            printf("\n%s\n",product_struct.categoryId);
                           product_struct = simulateMainItem(3,product_struct);
                            con = getConnection();
                            incrementUserProductIndicator(product_struct,user_struct,con);
                           if (strlen(user_struct.username) == 0) {
                               memset(product_name, 0, sizeof(product_name));
                               memset(product_id, 0, sizeof(product_id));
                               memset(product_barcode, 0, sizeof(product_barcode));
                               memset(product_quantity, 0, sizeof(product_quantity));
                                 memset(product_price, 0, sizeof(product_price));
                                 memset(product_categoryId, 0, sizeof(product_categoryId));
                                memset(product_userId, 0, sizeof(product_userId));
                               memset(product_location, 0, sizeof(product_location));
                                send(sock ,product_id ,sizeof(product_id),0);
                               send(sock ,product_name,sizeof(product_name),0);
                               send(sock ,product_barcode ,sizeof(product_barcode),0);
                               send(sock ,product_price,sizeof(product_price),0);
                               send(sock ,product_quantity ,sizeof(product_quantity),0);
                               send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                               send(sock ,product_userId ,sizeof(product_userId),0);
                               send(sock ,product_location ,sizeof(product_location),0);
                           }
                           else{
                               strcpy(product_id,product_struct.id);
                               strcpy(product_name,product_struct.name);
                               strcpy(product_barcode,product_struct.barcode);
                               strcpy(product_price,product_struct.price);
                               strcpy(product_quantity,product_struct.quantity);
                               strcpy(product_categoryId,product_struct.categoryId);
                               strcpy(product_userId,product_struct.userId);
                               strcpy(product_location,product_struct.location);
                               printf("%s\n",product_id);
                               printf("%s\n",product_name);
                               printf("%s\n",product_barcode);
                               printf("%s\n",product_price);
                               printf("%s\n",product_quantity);
                               printf("%s\n",product_categoryId);
                               printf("%s\n",product_userId);
                               printf("%s\n",product_location);
                               send(sock ,product_id ,sizeof(product_id),0);
                               send(sock ,product_name,sizeof(product_name),0);
                               send(sock ,product_barcode ,sizeof(product_barcode),0);
                               send(sock ,product_price,sizeof(product_price),0);
                               send(sock ,product_quantity ,sizeof(product_quantity),0);
                               send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                               send(sock ,product_userId ,sizeof(product_userId),0);
                               send(sock ,product_location ,sizeof(product_location),0);
                          }
                       break;
                   case 9:
                       memset(schat, 0, sizeof(schat));
                        strcpy(schat,"please enter product name\n");
                        send(sock ,schat ,strlen(schat),0);
                        read(sock,product_name,sizeof(product_name));
                        strcpy(product_struct.name,product_name);
                        product_struct = simulateMainItem(4,product_struct);
                        strcpy(product_name,product_struct.name);
                        printf("%s\n",product_name);
                        send(sock ,product_name ,sizeof(product_name),0);
                       break;
                   case 10:
                       memset(schat, 0, sizeof(schat));
                        strcpy(schat,"please enter product id\n");
                        send(sock ,schat ,strlen(schat),0);
                        read(sock,product_id,sizeof(product_id));
                        strcpy(product_struct.id,product_id);
                        product_struct = simulateMainItem(6,product_struct);
                        strcpy(product_id,product_struct.id);
                        printf("%s\n",product_id);
                        send(sock ,product_id ,sizeof(product_id),0);
                       break;
                   case 11:
                       memset(schat, 0, sizeof(schat));
                        strcpy(schat,"please enter product id\n");
                        send(sock ,schat ,strlen(schat),0);
                       read(sock,product_id,sizeof(product_id));
                        strcpy(product_struct.id,product_id);
                        printf(product_struct.id);
                        product_struct = simulateMainItem(5,product_struct);
                        strcpy(product_id,product_struct.id);
                        strcpy(product_name,product_struct.name);
                        strcpy(product_barcode,product_struct.barcode);
                        strcpy(product_price,product_struct.price);
                       strcpy(product_quantity,product_struct.quantity);
                       strcpy(product_categoryId,product_struct.categoryId);
                        strcpy(product_userId,product_struct.userId);
                        printf("%s\n",product_id);
                        printf("%s\n",product_name);
                        printf("%s\n",product_barcode);
                        printf("%s\n",product_price);
                        printf("%s\n",product_quantity);
                        printf("%s\n",product_categoryId);
                        printf("%s\n",product_userId);
                        send(sock ,product_id ,sizeof(product_id),0);
                        send(sock ,product_name,sizeof(product_name),0);
                        send(sock ,product_barcode ,sizeof(product_barcode),0);
                        send(sock ,product_price,sizeof(product_price),0);
                        send(sock ,product_quantity ,sizeof(product_quantity),0);
                        send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                        send(sock ,product_userId ,sizeof(product_userId),0);
                       break;
                       case 12:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter category name\n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,category_name,sizeof(category_name));
                       strcpy(category_struct.name,category_name);
                       category_struct = simulateMainCategory(1,category_struct);
                       strcpy(category_id,category_struct.id);
                       strcpy(category_name,category_struct.name);
                       printf("%s\n",category_id);
                       printf("%s\n",category_name);
                       send(sock ,category_id ,sizeof(category_id),0);
                       send(sock ,category_name ,sizeof(category_name),0);
                           break;
                       case 13:
                         memset(schat, 0, sizeof(schat));
                        strcpy(schat,"please enter  category name \n");
                        send(sock ,schat ,strlen(schat),0);
                        read(sock,category_name,sizeof(category_name));
                        strcpy(category_struct.name,category_name);
                        simulateMainCategory(2,category_struct);
                        if (strlen(user_struct.username) == 0) {
                            memset(category_id, 0, sizeof(category_id));
                            memset(category_name, 0, sizeof(category_name));
                            send(sock ,category_id ,sizeof(category_id),0);
                            send(sock ,category_name ,sizeof(category_name),0);
                        }
                         else{
                            strcpy(category_id,category_struct.id);
                                strcpy(category_name,category_struct.name);
                                printf("%s\n",category_id);
                                printf("%s\n",category_name);
                                send(sock ,product_id ,sizeof(product_id),0);
                                send(sock ,product_name,sizeof(product_name),0);
                                                 }
                           break;
                case 14:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter category name \n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,category_name,sizeof(category_name));
                       strcpy(category_struct.name,category_name);
                       category_struct = simulateMainCategory(3,category_struct);
                       strcpy(category_name,category_struct.name);
                       send(sock ,category_id ,sizeof(category_id),0);
                       send(sock ,category_name ,sizeof(category_name),0);
                            break;
                case 15:
                       memset(schat, 0, sizeof(schat));
                       strcpy(schat,"please enter category name \n");
                       send(sock ,schat ,strlen(schat),0);
                       read(sock,category_name,sizeof(category_name));
                       strcpy(category_struct.name,category_name);
                       category_struct = simulateMainCategory(4,category_struct);
                           break;
                case 16:
                       memset(user_id, 0, sizeof(user_id));
                       memset(userfullname, 0, sizeof(userfullname));
                        memset(username, 0, sizeof(username));
                        memset(userpassword, 0, sizeof(userpassword));
                        memset(userrole, 0, sizeof(userrole));
                        memset(schat, 0, sizeof(schat));
                        read(sock,username,sizeof(username));
                        strcpy(user_struct.username,username);
                       printf(user_struct.username);
                       user_struct = simulateMain(num,user_struct);
                       if (strlen(user_struct.username) == 0) {
                        memset(username, 0, sizeof(username));
                        memset(userpassword, 0, sizeof(userpassword));
                        memset(userfullname, 0, sizeof(userfullname));
                        memset(userrole, 0, sizeof(userrole));
                        send(sock ,username ,sizeof(username),0);
                        send(sock ,userpassword ,sizeof(userpassword),0);
                        send(sock ,userfullname ,sizeof(userfullname),0);
                           send(sock ,userrole ,sizeof(userrole),0);
                                }
                        else{
                            strcpy(username,user_struct.username);
                            strcpy(userpassword,user_struct.password);
                            strcpy(userfullname,user_struct.fullname);
                            strcpy(userrole,user_struct.role);
                            printf("%s\n",username);
                            printf("%s\n",userpassword);
                            printf("%s\n",userfullname);
                            printf("%s\n",userrole);
                            send(sock ,username ,sizeof(username),0);
                            send(sock ,userpassword ,sizeof(userpassword),0);
                            send(sock ,userfullname ,sizeof(userfullname),0);
                            send(sock ,userrole ,sizeof(userrole),0);
                                           }
                       break;
                   case 17:
                       
                                    con=getConnection();
                       
                       
                                    strcpy(query_template_s,"select * from product;");
                                   sprintf(query_s,query_template_s);
                                   printf("%s \n",query_s);


                                   if (mysql_query(con, query_s)) {
                                       printf("mysql query error\n");
                                     fprintf(stderr, "%s\n", mysql_error(con));
                                       num_rows=0;
                                        send(sock ,num_rows ,sizeof(num_rows),0);
                                   }

                                    res = mysql_store_result(con);
                                    num_rows = mysql_num_rows(res);
                                    memset(num_rows_char, 0, sizeof(num_rows_char));
                                    sprintf(num_rows_char,"%d",num_rows);
                                    itemList = calloc(num_rows, sizeof(struct product));
                                   
                                    num_fields = mysql_num_fields(res);
                                    send(sock ,num_rows_char ,sizeof(num_rows_char),0);
                                    printf("%s\n",num_rows_char);
                       
                                      int i=0;
                                       while((row = mysql_fetch_row(res))){
                                           printf("\n%d\n",i);
                                               row[0] != NULL?strcpy(itemList[i].id,row[0]):memset(itemList[i].id, 0, sizeof(itemList[i].id));
                                               row[1] != NULL?strcpy(itemList[i].name,row[1]):memset(itemList[i].name, 0, sizeof(itemList[i].name));
                                               row[2] != NULL?strcpy(itemList[i].barcode,row[2]):memset(itemList[i].barcode, 0, sizeof(itemList[i].barcode));
                                               row[3] != NULL?strcpy(itemList[i].quantity,row[3]):memset(itemList[i].quantity, 0, sizeof(itemList[i].quantity));
                                               row[4] != NULL?strcpy(itemList[i].price,row[4]):memset(itemList[i].price, 0, sizeof(itemList[i].price));
                                               row[5] != NULL?strcpy(itemList[i].categoryId,row[5]):memset(itemList[i].categoryId, 0, sizeof(itemList[i].categoryId));
                                               row[6] != NULL?strcpy(itemList[i].userId,row[6]):memset(itemList[i].userId, 0, sizeof(itemList[i].userId));
                                           memset(product_name, 0, sizeof(product_name));
                                           memset(product_id, 0, sizeof(product_id));
                                           memset(product_barcode, 0, sizeof(product_barcode));
                                           memset(product_quantity, 0, sizeof(product_quantity));
                                           memset(product_price, 0, sizeof(product_price));
                                           memset(product_categoryId, 0, sizeof(product_categoryId));
                                           memset(product_userId, 0, sizeof(product_userId));

                                                                                strcpy(product_id,itemList[i].id);
                                                                                strcpy(product_name,itemList[i].name);
                                                                                strcpy(product_barcode,itemList[i].barcode);
                                                                                strcpy(product_quantity,itemList[i].quantity);
                                                                                strcpy(product_price,itemList[i].price);
                                                                                strcpy(product_categoryId,itemList[i].categoryId);
                                                                                strcpy(product_userId,itemList[i].userId);
                                           
                                            send(sock ,product_id ,sizeof(product_id),0);
                                           usleep(1000);
                                           send(sock ,product_name ,sizeof(product_name),0);
                                            usleep(1000);
                                           send(sock ,product_barcode ,sizeof(product_barcode),0);
                                            usleep(1000);
                                           send(sock ,product_quantity ,sizeof(product_quantity),0);
                                            usleep(1000);
                                           send(sock ,product_price ,sizeof(product_price),0);
                                            usleep(1000);
                                           send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                                            usleep(1000);
                                        send(sock ,product_userId ,sizeof(product_userId),0);
                                           
                                           i++;
                                       }
                       for (int j=0; j<i; j++) {
                           printf("%s %s %s %s %s %s %s \n",itemList[j].id,itemList[j].name,itemList[j].barcode,itemList[j].quantity,itemList[j].price,itemList[j].categoryId,itemList[j].userId);
                       }
                                        printf("done!!!\n");
                                        mysql_free_result(res);
                                       memset(schat, 0, sizeof(schat));
                                    
                       break;
                   case 18:
                       
                                     memset(schat, 0, sizeof(schat));
                                    strcpy(schat,"please enter username\n");
                                    send(sock ,schat ,strlen(schat),0);
                                    read(sock,username,sizeof(username));
                                    strcpy(user_struct.username,username);
                                    printf(user_struct.username);
                                    user_struct = simulateMain(num,user_struct);
                                    strcpy(user_id,user_struct.id);
                       
                                    con=getConnection();
                                    user_struct=selectUserInfo(username,con);
                                   if (strlen(user_struct.id) == 0) {
                                       send(sock ,num_rows ,sizeof(num_rows),0);
                                       
                                        printf("done!!!\n");
                                        mysql_free_result(res);
                                       memset(schat, 0, sizeof(schat));
                                   }else{
                                   
                                    strcpy(query_template_s,"select * from product where user_id=%s;");
                                   sprintf(query_s,query_template_s,user_struct.id);
                                   printf("%s \n",query_s);


                                   if (mysql_query(con, query_s)) {
                                       printf("mysql query error\n");
                                     fprintf(stderr, "%s\n", mysql_error(con));
                                       num_rows=0;
                                        send(sock ,num_rows ,sizeof(num_rows),0);
                                   }

                                    res = mysql_store_result(con);
                                    num_rows = mysql_num_rows(res);
                                    memset(num_rows_char, 0, sizeof(num_rows_char));
                                    sprintf(num_rows_char,"%d",num_rows);
                                    itemList = calloc(num_rows, sizeof(struct product));
                                   
                                    num_fields = mysql_num_fields(res);
                                    send(sock ,num_rows_char ,sizeof(num_rows_char),0);
                                    printf("%s\n",num_rows_char);
                       
                                      
                                       while((row = mysql_fetch_row(res))){
                                           int i=0;
                                           
                                           memset(product_name, 0, sizeof(product_name));
                                           memset(product_id, 0, sizeof(product_id));
                                           memset(product_barcode, 0, sizeof(product_barcode));
                                           memset(product_quantity, 0, sizeof(product_quantity));
                                           memset(product_price, 0, sizeof(product_price));
                                           memset(product_categoryId, 0, sizeof(product_categoryId));
                                           memset(product_userId, 0, sizeof(product_userId));

                                                                                 
                                               row[0] != NULL?strcpy(itemList[i].id,row[0]):memset(itemList[i].id, 0, sizeof(itemList[i].id));
                                               row[1] != NULL?strcpy(itemList[i].name,row[1]):memset(itemList[i].name, 0, sizeof(itemList[i].name));
                                               row[2] != NULL?strcpy(itemList[i].barcode,row[2]):memset(itemList[i].barcode, 0, sizeof(itemList[i].barcode));
                                               row[3] != NULL?strcpy(itemList[i].quantity,row[3]):memset(itemList[i].quantity, 0, sizeof(itemList[i].quantity));
                                               row[4] != NULL?strcpy(itemList[i].price,row[4]):memset(itemList[i].price, 0, sizeof(itemList[i].price));
                                               row[5] != NULL?strcpy(itemList[i].categoryId,row[5]):memset(itemList[i].categoryId, 0, sizeof(itemList[i].categoryId));
                                               row[6] != NULL?strcpy(itemList[i].userId,row[6]):memset(itemList[i].userId, 0, sizeof(itemList[i].userId));
                                           strcpy(product_id,itemList[i].id);
                                           strcpy(product_name,itemList[i].name);
                                           strcpy(product_barcode,itemList[i].barcode);
                                           strcpy(product_quantity,itemList[i].quantity);
                                           strcpy(product_price,itemList[i].price);
                                           strcpy(product_categoryId,itemList[i].categoryId);
                                           strcpy(product_userId,itemList[i].userId);
                                           printf("%s  ",product_id);
                                           printf("%s  ",product_name);
                                           printf("%s  ",product_barcode);
                                           printf("%s  ",product_price);
                                           printf("%s  ",product_quantity);
                                           printf("%s  ",product_categoryId);
                                           printf("%s\n",product_userId);
                                           
                                           
                                           
                                               send(sock ,product_id ,sizeof(product_id),0);
                                              usleep(1000);
                                              send(sock ,product_name ,sizeof(product_name),0);
                                               usleep(1000);
                                              send(sock ,product_barcode ,sizeof(product_barcode),0);
                                               usleep(1000);
                                              send(sock ,product_quantity ,sizeof(product_quantity),0);
                                               usleep(1000);
                                              send(sock ,product_price ,sizeof(product_price),0);
                                               usleep(1000);
                                              send(sock ,product_categoryId ,sizeof(product_categoryId),0);
                                               usleep(1000);
                                              send(sock ,product_userId ,sizeof(product_userId),0);
                                           
//                                           printf("%s  ",product_id);
//                                           printf("%s  ",product_name);
//                                           printf("%s  ",product_barcode);
//                                           printf("%s  ",product_price);
//                                           printf("%s  ",product_quantity);
//                                           printf("%s  ",product_categoryId);
//                                           printf("%s\n",product_userId);
//
                                            
                                           
                                           
                                           printf("%s %s %s %s %s %s %s \n",itemList[i].id,itemList[i].name,itemList[i].barcode,itemList[i].quantity,itemList[i].price,itemList[i].categoryId,itemList[i].userId);


                                            printf("\n%d\n",i);
                                           
                                            i++;
                                       }
                                        printf("done!!!\n");
                                        mysql_free_result(res);
                                       memset(schat, 0, sizeof(schat));
                                   }
                                    
                       break;
                   case 19:
                       con=getConnection();
                                             
                                             
                                                          strcpy(query_template_s,"select * from users;");
                                                         sprintf(query_s,query_template_s);
                                                         printf("%s \n",query_s);


                                                         if (mysql_query(con, query_s)) {
                                                             printf("mysql query error\n");
                                                           fprintf(stderr, "%s\n", mysql_error(con));
                                                             num_rows=0;
                                                              send(sock ,num_rows ,sizeof(num_rows),0);
                                                         }

                                                          res = mysql_store_result(con);
                                                          num_rows = mysql_num_rows(res);
                                                          memset(num_rows_char, 0, sizeof(num_rows_char));
                                                          sprintf(num_rows_char,"%d",num_rows);
                                                          userList = calloc(num_rows, sizeof(struct users));
                                                         
                                                          num_fields = mysql_num_fields(res);
                                                          send(sock ,num_rows_char ,sizeof(num_rows_char),0);
                                                          printf("%s\n",num_rows_char);
                                                            int in=0;
                                                             while((row = mysql_fetch_row(res))){
                                                                 printf("\n%d\n",in);
                                                                     row[0] != NULL?strcpy(userList[in].id,row[0]):memset(userList[in].id, 0, sizeof(userList[in].id));
                                                                     row[1] != NULL?strcpy(userList[in].fullname,row[1]):memset(userList[in].fullname, 0, sizeof(userList[in].fullname));
                                                                     row[2] != NULL?strcpy(userList[in].username,row[2]):memset(userList[in].username, 0, sizeof(userList[in].username));
                                                                     row[3] != NULL?strcpy(userList[in].password,row[3]):memset(userList[in].password, 0, sizeof(userList[in].password));
                                                                     row[4] != NULL?strcpy(userList[in].role,row[4]):memset(userList[in].role, 0, sizeof(userList[in].role));
                                                                     row[5] != NULL?strcpy(userList[in].soldProduct,row[5]):memset(userList[in].soldProduct, 0, sizeof(userList[in].soldProduct));
                                                                 memset(user_id, 0, sizeof(user_id));
                                                                 memset(userfullname, 0, sizeof(userfullname));
                                                                 memset(username, 0, sizeof(username));
                                                                 memset(userpassword, 0, sizeof(userpassword));
                                                                 memset(userrole, 0, sizeof(userrole));
                                                                 memset(sold_product, 0, sizeof(sold_product));
                                                                                                      strcpy(user_id,userList[in].id);
                                                                                                      strcpy(userfullname,userList[in].fullname);
                                                                                                      strcpy(username,userList[in].username);
                                                                                                      strcpy(userpassword,userList[in].password);
                                                                                                      strcpy(userrole,userList[in].role);
                                                                                                      strcpy(sold_product,userList[in].soldProduct);
                                                                 
                                                                  send(sock ,user_id ,sizeof(user_id),0);
                                                                 usleep(1000);
                                                                 send(sock ,userfullname ,sizeof(userfullname),0);
                                                                  usleep(1000);
                                                                 send(sock ,username ,sizeof(username),0);
                                                                  usleep(1000);
                                                                 send(sock ,userpassword ,sizeof(userpassword),0);
                                                                  usleep(1000);
                                                                 send(sock ,userrole ,sizeof(userrole),0);
                                                                  usleep(1000);
                                                                 send(sock ,sold_product ,sizeof(sold_product),0);
                                                                  
                                                                 in++;
                                                             }
                                             for (int j=0; j<i; j++) {
                                                 printf("%s %s %s %s %s %s %s \n",userList[j].id,userList[j].fullname,userList[j].username,userList[j].password,userList[j].role[j],userList[j].soldProduct);
                                             }
                                                              printf("done!!!\n");
                                                              mysql_free_result(res);
                                                             memset(schat, 0, sizeof(schat));
                                                         
                       break;
                   case 25:
                       strcpy(quit,"bye");
                       break;
                   default:printf("Please enter valid value");
                       break;
               }
            if (strlen(quit) == 3) {
                break;
            }
    }
//                    free(user_id);
//                    free(userfullname);
//                    free(username);
//                    free(userpassword);
//                    free(userrole);
//                    free(product_id);
//                    free(product_name);
//                    free(product_barcode);
//                    free(product_quantity);
//                    free(product_price);
//                    free(product_categoryId);
//                    free(product_userId);
//                    free(category_id);
//                    free(category_name);
//                    free(num_rows);
//                    free(query_s);
//                    free(query_template_s);
//                    free(sold_product);
//
                    

       
    }
  
struct users selectUserInfo(char *user_name, MYSQL *connection)
{
    struct users user;
    memset(user.id, 0, sizeof(user.id));
    memset(user.username, 0, sizeof(user.username));
    memset(user.fullname, 0, sizeof(user.fullname));
    memset(user.password, 0, sizeof(user.password));
    memset(user.role, 0, sizeof(user.role));
    
 
    char *query=(char *) malloc(1024 * sizeof(char));
    char *query_template=(char *) malloc(100 * sizeof(char));
    strcpy(query_template,"select * from users where username='%s';");
    sprintf(query,query_template,user_name);
    printf("%s \n",query);
 
  
    if (mysql_query(connection, query)) {
       
      fprintf(stderr, "%s\n", mysql_error(connection));
    }
    else{
        
    MYSQL_RES *result = mysql_store_result(connection);
        if(result==NULL)
         {
             return user;
         }
    if ( mysql_num_fields(result) == 0) {
         mysql_free_result(result);
        return user;
    }
    else{
         printf("successfull\n");
         MYSQL_ROW row;
         while((row = mysql_fetch_row(result)))
         {
             
             row[0]!=NULL ? strcpy(user.id,row[0]):memset(user.id, 0, sizeof(user.id));
             row[1]!=NULL ? strcpy(user.fullname,row[1]):memset(user.fullname, 0, sizeof(user.fullname));
             row[2]!=NULL ? strcpy(user.username,row[2]):memset(user.username, 0, sizeof(user.username));
             row[3]!=NULL ? strcpy(user.password,row[3]):memset(user.password, 0, sizeof(user.password));
             row[4]!=NULL ? strcpy(user.role,row[4]):memset(user.role, 0, sizeof(user.role));
             row[4]!=NULL ? strcpy(user.soldProduct,row[5]):memset(user.soldProduct, 0, sizeof(user.soldProduct));
             mysql_free_result(result);
             return user;
         }
        }
    }
     
     return user;
}
struct users updateUserInfo(struct users user, MYSQL *connection) //bug
{
    
        struct users failuser;
    
        memset(failuser.id, 0, sizeof(failuser.id));
       memset(failuser.username, 0, sizeof(failuser.username));
       memset(failuser.fullname, 0, sizeof(failuser.fullname));
       memset(failuser.password, 0, sizeof(failuser.password));
       memset(failuser.role, 0, sizeof(failuser.role));
    
      char *query = (char *) malloc(1024 * sizeof(char));
      char *query_template_select=(char *) malloc(50 * sizeof(char));
      strcpy(query_template_select,"update users set  password= '%s', full_name='%s, ' where username= '%s';");
      char *query_template=(char *) malloc(50 * sizeof(char));
      sprintf(query,query_template_select,user.password,user.fullname,user.username);
      printf("%s\n",query);
    
      if (mysql_query(connection, query)) {
          fprintf(stderr, "%s\n", mysql_error(connection));
          return failuser;
      }
      else {
          return user;
       }
}
struct users createUser(struct users user, MYSQL *connection)
{
    struct users failuser;
    
    memset(failuser.id, 0, sizeof(failuser.id));
    memset(failuser.username, 0, sizeof(failuser.username));
    memset(failuser.fullname, 0, sizeof(failuser.fullname));
    memset(failuser.password, 0, sizeof(failuser.password));
    memset(failuser.role, 0, sizeof(failuser.role));
    
    char *query = (char *) malloc(1024 * sizeof(char));
    char *query_template_create=(char *) malloc(100 * sizeof(char));
    strcpy(query_template_create,"insert into users(username,password,full_name) values('%s','%s','%s');");
    sprintf(query,query_template_create,user.username,user.password,user.fullname);
    printf("%s\n",query);
    MYSQL_RES *result =mysql_query(connection, query);
     if (result!=NULL) {
         fprintf(stderr, "%s\n", mysql_error(connection));
         
          mysql_free_result(result);
         return failuser;
     }
     else{
          mysql_free_result(result);
         return user;
     }
    
}
struct users deleteUser(char * username,MYSQL *connection)
{
    struct users user;
//    item = selectItemInfo(item_id,connection);
    char *query = (char *) malloc(1024 * sizeof(char));
     char *query_template_delete=(char *) malloc(50 * sizeof(char));
    strcpy(query_template_delete,"delete from users where username = '%s';");
    sprintf(query,query_template_delete,username);
    printf("%s\n",query);
    MYSQL_RES *result =mysql_query(connection, query);
    if (result!=NULL) {
         fprintf(stderr, "%s\n", mysql_error(connection));
         mysql_free_result(result);
         return user;
             
     }
    else{
         mysql_free_result(result);
        return user;
    }
    
}
struct users simulateMain(int crud_choice,struct users user_main)
{
    MYSQL *conn = mysql_init(NULL);
       
       char *server = (char *) malloc(15 * sizeof(char));
       char *user = (char *) malloc(15 * sizeof(char));
       char *password = (char *) malloc(15 * sizeof(char));
       char *database = (char *) malloc(15 * sizeof(char));
       
       strcpy(server,"localhost");
       strcpy(password,"u1710009");
       strcpy(database,"cdb");
       strcpy(user,"musobek");
    
         if (!mysql_real_connect(conn, server, user, password,
                                           database, 0, NULL, 0)) {
             fprintf(stderr, "%s\n", mysql_error(conn));
             exit(1);
         }
      
    struct users user_return;
        
        
    switch (crud_choice) {
        case 1:
         
           user_return = selectUserInfo(user_main.username,conn);
            break;
        case 2:
           user_return = updateUserInfo(user_main,conn);
            break;
        case 3:
           user_return = createUser(user_main,conn);
            break;
        case 4:
           user_return = deleteUser(user_main.username,conn);
            break;
        case 5:
            user_return = createAdmin(user_main,conn);
            break;
        default:printf("Please enter valid value");
            break;
    }
     mysql_close(conn);
    return user_return;
}
struct users createAdmin(struct users user, MYSQL *connection)
{
    char *query = (char *) malloc(1024 * sizeof(char));
    char *query_template_create=(char *) malloc(100 * sizeof(char));
    strcpy(query_template_create,"insert into users(username,password,full_name,role) values('%s','%s','%s','adm');");
    sprintf(query,query_template_create,user.username,user.password,user.fullname);
    printf("%s\n",query);
    MYSQL_RES *result =mysql_query(connection, query);
     if (result!=NULL) {
         fprintf(stderr, "%s\n", mysql_error(connection));
         struct users failuser;
          mysql_free_result(result);
         return failuser;
     }
     else{
          mysql_free_result(result);
         return user;
     }
    
}

struct product selectProductInfo(char *product_name, MYSQL *connection)
{
    
    struct product item;

    memset(item.id, 0, sizeof(item.id));
     memset(item.name, 0, sizeof(item.name));
    memset(item.barcode, 0, sizeof(item.barcode));
     memset(item.quantity, 0, sizeof(item.quantity));
     memset(item.price, 0, sizeof(item.price));
     memset(item.userId, 0, sizeof(item.userId));
    memset(item.categoryId, 0, sizeof(item.categoryId));
    memset(item.location, 0, sizeof(item.location));
    
    char *query=(char *) malloc(2000 * sizeof(char));
    char *query_template=(char *) malloc(150 * sizeof(char));
    strcpy(query_template,"select product.id, product.name, product.barcode, product.quantity, product.price, users.username, category.name as cat_name, product.location from product,users,category where product.user_id=users.id and product.category_id=category.id and product.name='%s';");
    sprintf(query,query_template,product_name);
    printf("%s \n",query);
 
   
    if (mysql_query(connection, query)) {
        printf("mysql query error\n");
      fprintf(stderr, "%s\n", mysql_error(connection));
         return item;
    }
    MYSQL_RES *result = mysql_store_result(connection);
     
   int num_fields = mysql_num_fields(result);
    if (num_fields == 0) {
        return item;
    }
    else{
          MYSQL_ROW row;
        while((row = mysql_fetch_row(result))){
             printf("inside while\n");
            row[0]!= NULL? strcpy(item.id,row[0]):memset(item.id, 0, sizeof(item.id));
            row[1]!= NULL? strcpy(item.name,row[1]):memset(item.name, 0, sizeof(item.name));
            row[2]!= NULL? strcpy(item.barcode,row[2]):memset(item.barcode, 0, sizeof(item.barcode));
            row[3]!= NULL? strcpy(item.quantity,row[3]):memset(item.quantity, 0, sizeof(item.quantity));
            row[4]!= NULL? strcpy(item.price,row[4]):memset(item.price, 0, sizeof(item.price));
            row[5]!= NULL? strcpy(item.categoryId,row[5]):memset(item.categoryId, 0, sizeof(item.categoryId));
            row[6]!= NULL? strcpy(item.userId,row[6]):memset(item.userId, 0, sizeof(item.userId));
            row[7]!= NULL? strcpy(item.location,row[6]):memset(item.location, 0, sizeof(item.location));
            
            
            
        return item;
        }
         mysql_free_result(result);
         printf("outside while\n");
        return item;
     }
}
struct product selectProductInfoById(char *product_id, MYSQL *connection)
{
      struct product item;

      memset(item.id, 0, sizeof(item.id));
       memset(item.name, 0, sizeof(item.name));
      memset(item.barcode, 0, sizeof(item.barcode));
       memset(item.quantity, 0, sizeof(item.quantity));
       memset(item.price, 0, sizeof(item.price));
       memset(item.userId, 0, sizeof(item.userId));
      memset(item.categoryId, 0, sizeof(item.categoryId));
 
    char *query=(char *) malloc(1024 * sizeof(char));
    char *query_template=(char *) malloc(50 * sizeof(char));
    strcpy(query_template,"select * from product where id=%s;");
    sprintf(query,query_template,product_id);
    printf("%s \n",query);
 
  
    if (mysql_query(connection, query)) {
        printf("mysql query error\n");
      fprintf(stderr, "%s\n", mysql_error(connection));
        return item;
    }
    
    MYSQL_RES *result = mysql_store_result(connection);
    int num_fields = mysql_num_fields(result);
    if (num_fields==0) {
      printf("Sorry but there is not such item");
         mysql_free_result(result);
        return item;
    }
    else{
         printf("successfull");
         MYSQL_ROW row;
        while( (row = mysql_fetch_row(result)))
        {  strcpy(item.id,row[0]);
          strcpy(item.name,row[1]);
          strcpy(item.barcode,row[2]);
          strcpy(item.quantity,row[3]);
          strcpy(item.price,row[4]);
          strcpy(item.categoryId,row[5]);
          strcpy(item.userId,row[6]);
        mysql_free_result(result);
         return item;
        }
        mysql_free_result(result);
            return item;
        
     }
}

struct product updateProductInfo(struct product item, MYSQL *connection)
{
    struct product failproduct;
                memset(failproduct.id, 0, sizeof(failproduct.id));
                memset(failproduct.name, 0, sizeof(failproduct.name));
                memset(failproduct.barcode, 0, sizeof(failproduct.barcode));
                memset(failproduct.quantity, 0, sizeof(failproduct.quantity));
                memset(failproduct.price, 0, sizeof(failproduct.price));
                memset(failproduct.userId, 0, sizeof(failproduct.userId));
                memset(failproduct.categoryId, 0, sizeof(failproduct.categoryId));
                memset(failproduct.location, 0, sizeof(failproduct.location));
               
      char *query = (char *) malloc(1024 * sizeof(char));
      char *query_template_select=(char *) malloc(100 * sizeof(char));
      strcpy(query_template_select,"update product set barcode = '%s', quantity = %s, price = %s, location = '%s' where name = '%s'; ");
      sprintf(query,query_template_select,item.barcode,item.quantity,item.price,item.location,item.name);
      printf("%s\n",query);
    
      if (mysql_query(connection, query)) {
          fprintf(stderr, "%s\n", mysql_error(connection));
          return failproduct;
      }
      else {
          return item;
       }
}
struct product createProduct(struct product item, MYSQL *connection)
{
    struct product failproduct;
                memset(failproduct.id, 0, sizeof(failproduct.id));
                memset(failproduct.name, 0, sizeof(failproduct.name));
                memset(failproduct.barcode, 0, sizeof(failproduct.barcode));
                memset(failproduct.quantity, 0, sizeof(failproduct.quantity));
                memset(failproduct.price, 0, sizeof(failproduct.price));
                memset(failproduct.userId, 0, sizeof(failproduct.userId));
                memset(failproduct.categoryId, 0, sizeof(failproduct.categoryId));
                memset(failproduct.location, 0, sizeof(failproduct.location));
    
    char *query = (char *) malloc(1024 * sizeof(char));
    char *query_template_create=(char *) malloc(100 * sizeof(char));
    strcpy(query_template_create,"insert into product(name,barcode,quantity,price,category_id,user_id,location) values('%s','%s',%s ,%s ,%s ,%s,'%s');");
    sprintf(query,query_template_create,item.name,item.barcode,item.quantity,item.price,item.categoryId,item.userId,item.location);
    printf("%s\n",query);
    
     if (mysql_query(connection, query)) {
         fprintf(stderr, "%s\n", mysql_error(connection));
         return failproduct;
     }
     else{
         return item;
     }
}
struct product deleteProductByName(char * product_name,MYSQL *connection)
{
     struct product item;
    
    char *query = (char *) malloc(1024 * sizeof(char));
     char *query_template_delete=(char *) malloc(100 * sizeof(char));
    strcpy(query_template_delete,"delete from product where name = '%s';");
    sprintf(query,query_template_delete,product_name);
    printf("%s\n",query);
    MYSQL_RES *result =mysql_query(connection, query);
    if (mysql_query(connection, query)) {
            fprintf(stderr, "%s\n", mysql_error(connection));
            return item;
        }
        else{
            return item;
        }
}
struct product deleteProductById(char * product_id,MYSQL *connection)
{
     struct product item;
    
    char *query = (char *) malloc(1024 * sizeof(char));
     char *query_template_delete=(char *) malloc(100 * sizeof(char));
    strcpy(query_template_delete,"delete from product where name = '%s';");
    sprintf(query,query_template_delete,product_id);
    printf("%s\n",query);
    MYSQL_RES *result =mysql_query(connection, query);
     if (mysql_query(connection, query)) {
               fprintf(stderr, "%s\n", mysql_error(connection));
               return item;
           }
           else{
               return item;
           }
}
struct product simulateMainItem(int crud_choice,struct product item)
{
            MYSQL *conn = mysql_init(NULL);
           char *server = (char *) malloc(15 * sizeof(char));
           char *user = (char *) malloc(15 * sizeof(char));
           char *password = (char *) malloc(15 * sizeof(char));
           char *database = (char *) malloc(15 * sizeof(char));
           
           strcpy(server,"localhost");
           strcpy(password,"u1710009");
           strcpy(database,"cdb");
           strcpy(user,"musobek");
        
             if (!mysql_real_connect(conn, server, user, password,
                                               database, 0, NULL, 0)) {
                 fprintf(stderr, "%s\n", mysql_error(conn));
                 exit(1);
             }
          
        struct product product_return;
            
        switch (crud_choice) {
            case 1:
             
               product_return = selectProductInfo(item.name,conn);
                break;
            case 2:
               product_return = updateProductInfo(item,conn);
                break;
            case 3:
               product_return = createProduct(item,conn);
                printf("due to simulate main item\n");
                break;
            case 4:
               product_return = deleteProductByName(item.name,conn);
                break;
            case 5:
                product_return = selectProductInfoById(item.id,conn);
                break;
            case 6:
                product_return = deleteProductById(item.id,conn);
                break;
            default:printf("Please enter valid value");
                break;
        }
        return product_return;
    }
struct category simulateMainCategory(int crud_choice,struct category cat)
{
            MYSQL *conn = mysql_init(NULL);
           
           char *server = (char *) malloc(15 * sizeof(char));
           char *user = (char *) malloc(15 * sizeof(char));
           char *password = (char *) malloc(15 * sizeof(char));
           char *database = (char *) malloc(15 * sizeof(char));
           
           strcpy(server,"localhost");
           strcpy(password,"u1710009");
           strcpy(database,"cdb");
           strcpy(user,"musobek");
        
             if (!mysql_real_connect(conn, server, user, password,
                                               database, 0, NULL, 0)) {
                 fprintf(stderr, "%s\n", mysql_error(conn));
                 exit(1);
             }
          
        struct category cat_return;
            
        switch (crud_choice) {
            case 1:
               cat_return = selectCategoryInfo(cat.name,conn);
                break;
            case 2:
               cat_return = updateCategoryInfo(cat,conn);
                break;
            case 3:
               cat_return = createCategory(cat,conn);
                break;
            case 4:
               cat_return = deleteCategory(cat.name,conn);
                break;
                          
            default:printf("Please enter valid value");
                break;
        }
   
        return cat_return;
}
                                 

struct category selectCategoryInfo(char *cat_name, MYSQL *connection)
{
    struct category cat;
    memset(cat.id, 0, sizeof(cat.id));
    memset(cat.name, 0, sizeof(cat.name));

 
    char *query=(char *) malloc(1024 * sizeof(char));
    char *query_template=(char *) malloc(100 * sizeof(char));
    strcpy(query_template,"select * from category where name='%s';");
    sprintf(query,query_template,cat_name);
    printf("%s \n",query);
 
  
    if (mysql_query(connection, query)) {
      fprintf(stderr, "%s\n", mysql_error(connection));
        return cat;
    }
    
    MYSQL_RES *result = mysql_store_result(connection);
    int num_fields = mysql_num_fields(result);
    if (num_fields==0) {
        mysql_free_result(result);
        return cat;
    }
    else{
         printf("successfull");
         MYSQL_ROW row;
         if(row = mysql_fetch_row(result))
         {
         strcpy(cat.id,row[0]);
         strcpy(cat.name,row[1]);
             printf("%s\n",cat.id);
             printf("%s\n",cat.name);
        mysql_free_result(result);
        return cat;
         }
         else {
              return cat;
         }
        
     }
}
struct category updateCategoryInfo(struct category cat, MYSQL *connection)
{
      struct category failcat;

         memset(failcat.id, 0, sizeof(failcat.id));
         memset(failcat.name, 0, sizeof(failcat.name));
    
      char *query = (char *) malloc(1024 * sizeof(char));
      char *query_template_select=(char *) malloc(50 * sizeof(char));
      strcpy(query_template_select,"update category set  name='%s' where id= %s;");
      char *query_template=(char *) malloc(50 * sizeof(char));
      sprintf(query,query_template_select,cat.name,cat.id);
      printf("%s\n",query);
    
      if (mysql_query(connection, query)) {
          fprintf(stderr, "%s\n", mysql_error(connection));
        
          return failcat;
      }
      else {
          return cat;
       }
}
struct category createCategory(struct category cat, MYSQL *connection)
{
    struct category failcat;

          memset(failcat.id, 0, sizeof(failcat.id));
          memset(failcat.name, 0, sizeof(failcat.name));
     
    char *query = (char *) malloc(1024 * sizeof(char));
    char *query_template_create=(char *) malloc(100 * sizeof(char));
    strcpy(query_template_create,"insert into category(name) values('%s');");
    sprintf(query,query_template_create,cat.name);
    printf("%s\n",query);
   
    if (mysql_query(connection, query)) {
        fprintf(stderr, "%s\n", mysql_error(connection));
      
        return failcat;
    }
    else {
        return cat;
     }
}
struct category deleteCategory(char * cat_name,MYSQL *connection)
{
    struct category cat;
    char *query = (char *) malloc(1024 * sizeof(char));
     char *query_template_delete=(char *) malloc(50 * sizeof(char));
    strcpy(query_template_delete,"delete from category where name = '%s';");
    sprintf(query,query_template_delete,cat_name);
    printf("%s\n",query);
   
    if (mysql_query(connection, query)) {
        fprintf(stderr, "%s\n", mysql_error(connection));
      
        return cat;
    }
    else {
        return cat;
     }
}
MYSQL *getConnection()
{
    MYSQL *con = mysql_init(NULL);
       char *server = (char *) malloc(15 * sizeof(char));
       char *user = (char *) malloc(15 * sizeof(char));
       char *password = (char *) malloc(15 * sizeof(char));
       char *database = (char *) malloc(15 * sizeof(char));
       
       strcpy(server,"localhost");
       strcpy(password,"u1710009");
       strcpy(database,"cdb");
       strcpy(user,"musobek");
    
         if (!mysql_real_connect(con, server, user, password,
                                           database, 0, NULL, 0)) {
             fprintf(stderr, "%s\n", mysql_error(con));
             exit(1);
         }
    return con;
}
void incrementUserProductIndicator(struct product item,struct users user,MYSQL *connection)
{
         char *query = (char *) malloc(1024 * sizeof(char));
         char *query_template_select=(char *) malloc(100 * sizeof(char));
        int total=atoi(user.soldProduct)+atoi(item.price)*atoi(item.quantity);
        char *total_char = (char *) malloc(20 * sizeof(char));
        sprintf(total_char,"%d",total);
         strcpy(query_template_select,"update users set sold_product= %s where username='%s'; ");
         sprintf(query,query_template_select,total_char,user.username);
        printf("%s\n",total_char);
         printf("%s\n",query);
       
         if (mysql_query(connection, query)) {
             fprintf(stderr, "%s\n", mysql_error(connection));
         }
        
}


 

