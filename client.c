struct product
{
    char id[15];
    char name[45];
    char barcode[45];
    char quantity[15];
    char price[20];
    char categoryId[20];
    char userId[20];
};
struct users
{
    char id[15];
    char fullname[45];
    char username[20];
    char password[20];
    char role[5];
    char soldProduct[20]
    
};
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#define PORT 9998

int authenticateUser(char *uname,char *upassword,char *urole,char *dpassword );
int main(int argc, char const *argv[])
{
struct sockaddr_in address;
int sock = 0;
struct sockaddr_in serv_addr;
char buffer[1024] = {0}, cchat[2001];
char *authPassword = (char *) malloc(20 * sizeof(char));
    int authnum;
   char *user_id = (char *) malloc(15 * sizeof(char));
    char *userfullname = (char *) malloc(45 * sizeof(char));
    char *username = (char *) malloc(20 * sizeof(char));
    char *userpassword = (char *) malloc(20 * sizeof(char));
    char *userrole = (char *) malloc(5 * sizeof(char));
     char valread[3];
    
        char *product_id = (char *) malloc(15 * sizeof(char));
        char *product_name = (char *) malloc(45 * sizeof(char));
        char *product_barcode = (char *) malloc(45 * sizeof(char));
        char *product_quantity = (char *) malloc(15 * sizeof(char));
        char *product_price = (char *) malloc(20 * sizeof(char));
        char *product_categoryId = (char *) malloc(20 * sizeof(char));
        char *product_userId = (char *) malloc(20 * sizeof(char));
        char *product_location = (char *) malloc(45 * sizeof(char));
    
    char *sold_product = (char *) malloc(20 * sizeof(char));
    
    char *category_id = (char *) malloc(15 * sizeof(char));
    char *category_name = (char *) malloc(45 * sizeof(char));
    int success=0;
    struct product *list=NULL;
    struct product item;
    char listSize[10];
    int listSize_int;
    struct users *userList=NULL;
 
    
    printf("CREATING CLIENT SOCKET .....\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    printf("DEFINING SOCKET FAMILY, ADDRESS & PORT .....\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "192.168.43.139", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    printf("CLIENT CONNECTING ON PORT 8080 TO COMMUNICATE WITH SERVER..\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
   
    while(1)
    {
    memset(buffer, 0, sizeof(buffer));
    memset(valread, 0, sizeof(valread));
    read( sock , buffer, 2000);
    printf("%s",buffer);
    fflush(stdin);
    
    fgets (valread, sizeof(valread), stdin);
    printf(valread);
    send(sock , valread , sizeof(valread), 0 );
//    while(1) {
     memset(buffer, 0, sizeof(buffer));
     memset(user_id, 0, sizeof(user_id));
     memset(username, 0, sizeof(username));
     memset(userpassword, 0, sizeof(userpassword));
     memset(userfullname, 0, sizeof(userfullname));
     memset(userrole, 0, sizeof(userrole));
        memset(product_id, 0, sizeof(product_id));
        memset(product_name, 0, sizeof(product_name));
        memset(product_quantity, 0, sizeof(product_quantity));
        memset(product_barcode, 0, sizeof(product_barcode));
        memset(product_price, 0, sizeof(product_price));
        memset(product_userId, 0, sizeof(product_userId));
        memset(product_categoryId, 0, sizeof(product_categoryId));
        memset(category_id, 0, sizeof(category_id));
          memset(category_name, 0, sizeof(category_name));
         fflush(stdin);
    int num = atoi(valread);
    switch (num) {
        case 1:
             memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, sizeof(buffer));
            printf("%s",buffer);
            fgets (username, sizeof(username), stdin);
            send(sock , username , sizeof(username) , 0 );
            read(sock ,user_id ,sizeof(user_id));
            read(sock ,username ,sizeof(username));
            read(sock ,userpassword ,sizeof(userpassword));
            read(sock ,userfullname ,sizeof(userfullname));
            read(sock ,userrole ,sizeof(userrole));
            read(sock ,sold_product ,sizeof(sold_product));
            printf("%s\n",user_id);
            printf("%s\n",username);
            printf("%s\n",userpassword);
            printf("%s\n",userfullname);
            printf("%s\n",sold_product);
            break;
        case 2:
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (userfullname, sizeof(userfullname), stdin);
             send(sock , userfullname , sizeof(userfullname), 0 );
             fflush(stdin);
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (username, sizeof(username), stdin);
             send(sock , username , sizeof(username), 0 );
              fflush(stdin);
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (userpassword, sizeof(userpassword), stdin);
             send(sock , userpassword , sizeof(userpassword), 0 );
              fflush(stdin);
             read(sock ,username ,sizeof(username));
             read(sock ,userpassword ,sizeof(userpassword));
             read(sock ,userfullname ,sizeof(userfullname));
             read(sock ,userrole ,sizeof(userrole));
             printf("%s\n",username);
             printf("%s\n",userpassword);
             printf("%s\n",userfullname);
             printf("%s\n",userrole);
            break;
        case 3:
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (username, sizeof(username), stdin);
            send(sock , username , sizeof(username), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (userpassword, sizeof(userpassword), stdin);
            send(sock , userpassword , sizeof(userpassword), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (userfullname, sizeof(userfullname), stdin);
            send(sock , userfullname , sizeof(userfullname), 0 );
            read(sock ,username ,sizeof(username));
            read(sock ,userpassword ,sizeof(userpassword));
            read(sock ,userfullname ,sizeof(userfullname));
            read(sock ,userrole ,sizeof(userrole));
            printf("%s\n",username);
            printf("%s\n",userpassword);
            printf("%s\n",userfullname);
            printf("%s\n",userrole);
            break;
        case 4:
            read( sock , buffer, sizeof(buffer));
            printf("%s",buffer);
            fgets (username, sizeof(username), stdin);
            send(sock , username , sizeof(username) , 0 );
            break;
        case 5:
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (username, sizeof(username), stdin);
            send(sock , username , sizeof(username), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (userpassword, sizeof(userpassword), stdin);
            send(sock , userpassword , sizeof(userpassword), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (userfullname, sizeof(userfullname), stdin);
            send(sock , userfullname , sizeof(userfullname), 0 );
            read(sock ,username ,sizeof(username));
            read(sock ,userpassword ,sizeof(userpassword));
            read(sock ,userfullname ,sizeof(userfullname));
            read(sock ,userrole ,sizeof(userrole));
            printf("%s\n",username);
            printf("%s\n",userpassword);
            printf("%s\n",userfullname);
            printf("%s\n",userrole);
            break;
        case 6:
            read( sock , buffer, sizeof(buffer));
            printf("%s",buffer);
            fgets (product_name, sizeof(product_name), stdin);
            send(sock ,product_name ,sizeof(product_name), 0 );
            read(sock ,product_id ,sizeof(product_id));
            read(sock ,product_name ,sizeof(product_name));
            read(sock ,product_barcode ,sizeof(product_barcode));
            read(sock ,product_price ,sizeof(product_price));
            read(sock ,product_quantity ,sizeof(product_quantity));
            read(sock ,product_categoryId ,sizeof(product_categoryId));
            read(sock ,product_userId ,sizeof(product_userId));
            read(sock ,product_location ,sizeof(product_location));
            printf("%s\n",product_id);
            printf("%s\n",product_name);
            printf("%s\n",product_barcode);
            printf("%s\n",product_price);
            printf("%s\n",product_quantity);
            printf("%s\n",product_categoryId);
            printf("%s\n",product_userId);
             printf("%s\n",product_location);
            break;
        case 7:
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (product_name, sizeof(product_name), stdin);
             send(sock , product_name , sizeof(product_name), 0 );
              fflush(stdin);
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (product_barcode, sizeof(product_barcode), stdin);
             send(sock , product_barcode , sizeof(product_barcode), 0 );
              fflush(stdin);
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (product_quantity, sizeof(product_quantity), stdin);
            send(sock , product_quantity , sizeof(product_quantity), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (product_price, sizeof(product_price), stdin);
            send(sock , product_price , sizeof(product_price), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (product_categoryId, sizeof(product_categoryId), stdin);
            send(sock , product_categoryId , sizeof(product_categoryId), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (product_userId, sizeof(product_userId), stdin);
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (product_location, sizeof(product_location), stdin);
            send(sock , product_location , sizeof(product_location), 0 );
            fflush(stdin);
             send(sock , product_userId , sizeof(product_userId), 0 );
             read(sock ,product_id ,sizeof(product_id));
             read(sock ,product_name ,sizeof(product_name));
             read(sock ,product_barcode ,sizeof(product_barcode));
             read(sock ,product_price ,sizeof(product_price));
             read(sock ,product_quantity ,sizeof(product_quantity));
             read(sock ,product_categoryId ,sizeof(product_categoryId));
             read(sock ,product_userId ,sizeof(product_userId));
            read(sock ,product_location ,sizeof(product_location));
             printf("%s\n",product_id);
             printf("%s\n",product_name);
             printf("%s\n",product_barcode);
             printf("%s\n",product_price);
              printf("%s\n",product_quantity);
              printf("%s\n",product_categoryId);
              printf("%s\n",product_userId);
            printf("%s\n",product_location);
            break;
        case 8:
            memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (product_name, sizeof(product_name), stdin);
             send(sock , product_name , sizeof(product_name), 0 );
              fflush(stdin);
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (product_barcode, sizeof(product_barcode), stdin);
             send(sock , product_barcode , sizeof(product_barcode), 0 );
              fflush(stdin);
             memset(buffer, 0, sizeof(buffer));
             read( sock , buffer, 2000);
             printf("%s",buffer);
             fgets (product_quantity, sizeof(product_quantity), stdin);
            send(sock , product_quantity , sizeof(product_quantity), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (product_price, sizeof(product_price), stdin);
            send(sock , product_price , sizeof(product_price), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (category_name, sizeof(category_name), stdin);
            send(sock , category_name , sizeof(category_name), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (username, sizeof(username), stdin);
            send(sock , username , sizeof(username), 0 );
            fflush(stdin);
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (product_price, sizeof(product_price), stdin);
            send(sock , product_price , sizeof(product_price), 0 );
            fflush(stdin);
             read(sock ,product_id ,sizeof(product_id));
             read(sock ,product_name ,sizeof(product_name));
             read(sock ,product_barcode ,sizeof(product_barcode));
             read(sock ,product_price ,sizeof(product_price));
             read(sock ,product_quantity ,sizeof(product_quantity));
             read(sock ,product_categoryId ,sizeof(product_categoryId));
             read(sock ,product_userId ,sizeof(product_userId));
             printf("%s\n",product_id);
             printf("%s\n",product_name);
             printf("%s\n",product_barcode);
             printf("%s\n",product_price);
              printf("%s\n",product_quantity);
              printf("%s\n",product_categoryId);
              printf("%s\n",product_userId);
            break;
        case 9:
            read( sock , buffer, sizeof(buffer));
            printf("%s",buffer);
            fgets (product_name, sizeof(product_name), stdin);
            send(sock , product_name , sizeof(product_name) , 0 );
            read(sock ,product_name ,sizeof(product_name));
            printf("%s\n",product_name);
            break;
        case 10:
                read( sock , buffer, sizeof(buffer));
                printf("%s",buffer);
                fgets (product_id, sizeof(product_id), stdin);
                send(sock , product_id , sizeof(product_id) , 0 );
                read(sock ,product_id ,sizeof(product_id));
                printf("%s\n",product_id);
                break;
        case 11: read( sock , buffer, sizeof(buffer));
                printf("%s",buffer);
                fgets (product_id, sizeof(product_id), stdin);
                send(sock ,product_id ,sizeof(product_id), 0 );
                read(sock ,product_id ,sizeof(product_id));
                read(sock ,product_name ,sizeof(product_name));
                read(sock ,product_barcode ,sizeof(product_barcode));
                read(sock ,product_price ,sizeof(product_price));
                read(sock ,product_quantity ,sizeof(product_quantity));
                read(sock ,product_categoryId ,sizeof(product_categoryId));
                read(sock ,product_userId ,sizeof(product_userId));
                printf("%s\n",product_id);
                printf("%s\n",product_name);
                printf("%s\n",product_barcode);
                printf("%s\n",product_price);
                printf("%s\n",product_quantity);
                printf("%s\n",product_categoryId);
                printf("%s\n",product_userId);
                break;
        case 12:
            read( sock , buffer, sizeof(buffer));
            printf("%s",buffer);
            fgets (category_name, sizeof(category_name), stdin);
            send(sock , category_name , sizeof(category_name) , 0 );
            read(sock ,category_id ,sizeof(category_id));
            read(sock ,category_name ,sizeof(category_name));
            printf("%s\n",category_id);
            printf("%s\n",category_name);
            read(sock ,success ,sizeof(success));
            break;
        case 13:
             memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (category_name, sizeof(category_name), stdin);
            send(sock , category_name , sizeof(category_name), 0 );
            read(sock ,category_id ,sizeof(category_id));
            read(sock ,category_name ,sizeof(category_name));
            printf("%s\n",category_id);
            printf("%s\n",category_name);
            read(sock ,success ,sizeof(success));
            break;
        case 14:
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (category_name, sizeof(category_name), stdin);
            send(sock , category_name , sizeof(category_name), 0 );
            fflush(stdin);
            read(sock ,category_id ,sizeof(category_id));
            read(sock ,category_name ,sizeof(category_name));
            printf("%s\n",category_id);
            printf("%s\n",category_name);
            break;
        case 15:
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, 2000);
            printf("%s",buffer);
            fgets (category_name, sizeof(category_name), stdin);
            send(sock , category_name , sizeof(category_name), 0 );
            fflush(stdin);
           
            break;
        case 16:
         memset(buffer, 0, sizeof(buffer));
         read( sock , buffer, sizeof(buffer));
         printf("%s",buffer);
         fgets (username, sizeof(username), stdin);
         fflush(stdin);
         send(sock , username , sizeof(username) , 0 );
         memset(buffer, 0, sizeof(buffer));
         read( sock , buffer, sizeof(buffer));
         printf("%s",buffer);
         fgets (authPassword, sizeof(authPassword), stdin);
         memset(username, 0, sizeof(username));
         memset(userpassword, 0, sizeof(userpassword));
         read(sock ,username ,sizeof(username));
         read(sock ,userpassword ,sizeof(userpassword));
         read(sock ,userfullname ,sizeof(userfullname));
         read(sock ,userrole ,sizeof(userrole));
         printf("%s\n",username);
         printf("%s\n",userpassword);
         printf("%s\n",userfullname);
         printf("%s\n",userrole);
        authnum = authenticateUser(username,authPassword,userrole,userpassword);
            break;
        case 17:
//            memset(listSize, 0, sizeof(listSize));
             read( sock , listSize, sizeof(listSize));
            listSize_int=atoi(listSize);
            printf("\n%d is list size",listSize_int);
            list = calloc(listSize_int, sizeof(struct product));
            //list = realloc(list, listSize_int * sizeof *list);
            for (int i=0; i<listSize_int; i++) {
             memset(list[i].id, 0, sizeof(list[i].id));
             memset(list[i].name, 0, sizeof(list[i].name));
             memset(list[i].barcode, 0, sizeof(list[i].barcode));
             memset(list[i].quantity, 0, sizeof(list[i].quantity));
             memset(list[i].price, 0, sizeof(list[i].price));
             memset(list[i].userId, 0, sizeof(list[i].userId));
             memset(list[i].categoryId, 0, sizeof(list[i].categoryId));
            }
            for (int i=0; i<listSize_int; i++) {
                                                        
                    read(sock ,product_id ,sizeof(product_id));
                    read(sock ,product_name ,sizeof(product_name));
                    read(sock ,product_barcode ,sizeof(product_barcode));
                    read(sock ,product_quantity ,sizeof(product_quantity));
                    read(sock ,product_price ,sizeof(product_price));
                    read(sock ,product_categoryId ,sizeof(product_categoryId));
                    read(sock ,product_userId ,sizeof(product_userId));
                                                                        printf("%s  id ",product_id);
                                                                        printf("%s   name ",product_name);
                                                                        printf("%s  barcode ",product_barcode);
                                                                        printf("%s  price ",product_price);
                                                                        printf("%s  quantity ",product_quantity);
                                                                        printf("%s  categoryId ",product_categoryId);
                                                                        printf("%s userId \n",product_userId);

                                                                                
                
                strcpy(list[i].id,product_id);
                strcpy(list[i].name,product_name);
                strcpy(list[i].barcode,product_barcode);
                strcpy(list[i].quantity,product_quantity);
                strcpy(list[i].price,product_price);
                strcpy(list[i].categoryId,product_categoryId);
                strcpy(list[i].userId,product_userId);
                
                memset(product_name, 0, sizeof(product_name));
                 memset(product_id, 0, sizeof(product_id));
                 memset(product_barcode, 0, sizeof(product_barcode));
                 memset(product_quantity, 0, sizeof(product_quantity));
                 memset(product_price, 0, sizeof(product_price));
                 memset(product_categoryId, 0, sizeof(product_categoryId));
                 memset(product_userId, 0, sizeof(product_userId));
                
                       }
            break;
        case 18:
            memset(buffer, 0, sizeof(buffer));
            read( sock , buffer, sizeof(buffer));
            printf("%s",buffer);
            fgets (username, sizeof(username), stdin);
            send(sock , username , sizeof(username) , 0 );
            memset(listSize, 0, sizeof(listSize));
            read( sock , listSize, sizeof(listSize));
            listSize_int=atoi(listSize);
            printf("\n%d is list size",listSize_int);
             list = calloc(listSize_int, sizeof(struct product));
           // list = realloc(list, listSize_int * sizeof *list);
            for (int i=0; i<listSize_int; i++) {
                memset(list[i].id, 0, sizeof(list[i].id));
                memset(list[i].name, 0, sizeof(list[i].name));
                memset(list[i].barcode, 0, sizeof(list[i].barcode));
                memset(list[i].quantity, 0, sizeof(list[i].quantity));
                memset(list[i].price, 0, sizeof(list[i].price));
                memset(list[i].userId, 0, sizeof(list[i].userId));
                memset(list[i].categoryId, 0, sizeof(list[i].categoryId));
               }
            for (int i=0; i<listSize_int; i++) {
                 printf("%s %s %s %s %s %s %s \n",list[i].id,list[i].name,list[i].barcode,list[i].quantity,list[i].price,list[i].categoryId,list[i].userId);
                
                printf("%s  ",product_id);
                                                        
                
                    read(sock ,product_id ,sizeof(product_id));
                    read(sock ,product_name ,sizeof(product_name));
                    read(sock ,product_barcode ,sizeof(product_barcode));
                    read(sock ,product_quantity ,sizeof(product_quantity));
                    read(sock ,product_price ,sizeof(product_price));
                    read(sock ,product_categoryId ,sizeof(product_categoryId));
                    read(sock ,product_userId ,sizeof(product_userId));
                                                                        printf("%s  id ",product_id);
                                                                        printf("%s   name ",product_name);
                                                                        printf("%s  barcode ",product_barcode);
                                                                        printf("%s  price ",product_price);
                                                                        printf("%s  quantity ",product_quantity);
                                                                        printf("%s  categoryId ",product_categoryId);
                                                                        printf("%s userId \n",product_userId);
                              
                                                                                
                
                strcpy(list[i].id,product_id);
                strcpy(list[i].name,product_name);
                strcpy(list[i].barcode,product_barcode);
                strcpy(list[i].quantity,product_quantity);
                strcpy(list[i].price,product_price);
                strcpy(list[i].categoryId,product_categoryId);
                strcpy(list[i].userId,product_userId);
                
                memset(product_name, 0, sizeof(product_name));
                 memset(product_id, 0, sizeof(product_id));
                 memset(product_barcode, 0, sizeof(product_barcode));
                 memset(product_quantity, 0, sizeof(product_quantity));
                 memset(product_price, 0, sizeof(product_price));
                 memset(product_categoryId, 0, sizeof(product_categoryId));
                 memset(product_userId, 0, sizeof(product_userId));
                
                       }
            
            
            for (int i=0; i<listSize_int; i++) {

                printf("%s %s %s %s %s %s %s \n",list[i].id,list[i].name,list[i].barcode,list[i].quantity,list[i].price,list[i].categoryId,list[i].userId);
            }

            break;
        case 19:
                        memset(listSize, 0, sizeof(listSize));
                        read( sock , listSize, sizeof(listSize));
                        listSize_int=atoi(listSize);
                       printf("\n%d is list size",listSize_int);
                       userList = calloc(listSize_int, sizeof(struct users));
                       //list = realloc(list, listSize_int * sizeof *list);
                       for (int i=0; i<listSize_int; i++) {
                        memset(userList[i].id, 0, sizeof(userList[i].id));
                        memset(userList[i].fullname, 0, sizeof(userList[i].fullname));
                        memset(userList[i].username, 0, sizeof(userList[i].username));
                        memset(userList[i].password, 0, sizeof(userList[i].password));
                        memset(userList[i].role, 0, sizeof(userList[i].role));
                        memset(userList[i].soldProduct, 0, sizeof(userList[i].soldProduct));
                       }
                       for (int i=0; i<listSize_int; i++) {
                                                                   
                               read(sock ,user_id ,sizeof(user_id));
                               read(sock ,userfullname ,sizeof(userfullname));
                               read(sock ,username ,sizeof(username));
                               read(sock ,userpassword ,sizeof(userpassword));
                               read(sock ,userrole ,sizeof(userrole));
                               read(sock ,sold_product ,sizeof(sold_product));
                                                                                   printf("%s  id ",user_id);
                                                                                   printf("%s   fn ",userfullname);
                                                                                   printf("%s  un ",username);
                                                                                   printf("%s  p ",userpassword);
                                                                                   printf("%s  r ",userrole);
                                                                                   printf("%s  s_p ",sold_product);
                           
                                                                                                       strcpy(user_id,userList[i].id);
                           strcpy(userList[i].fullname,userfullname);
                           strcpy(userList[i].username,username);
                           strcpy(userpassword,userList[i].password);
                           strcpy(userList[i].role,userrole);
                           strcpy(userList[i].soldProduct,sold_product);

                           
                           memset(user_id, 0, sizeof(user_id));
                           memset(userfullname, 0, sizeof(userfullname));
                           memset(username, 0, sizeof(username));
                           memset(userpassword, 0, sizeof(userpassword));
                           memset(userrole, 0, sizeof(userrole));
                           memset(sold_product, 0, sizeof(sold_product));

            break;
        case 25:
            return 0;
            break;
        default:printf("Please enter valid value");
            break;
        }
        
    }
    }
}
   
int authenticateUser(char *uname,char *upassword,char *urole,char *dpassword )
{
    if (uname==NULL) {
        return 0;
    }
    else{
        if (upassword == NULL) {
             return 0;
        }
        else{
            char *role="user";
            if(strcmp(upassword,dpassword)>=0)
            {
                printf("correct password \n");
                 if(strcmp(urole,role)>=0)
                 {
                     return 1;
                }
                else
                {
                    return 2;
                }
            }
            else{
                return 0;
            }
            
        }
    }
}
