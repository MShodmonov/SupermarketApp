# SupermarketApp
this project uses libmysql library that should be inserted externally this link shows how to do this with packege config
https://dev.mysql.com/doc/refman/8.0/en/c-api-building-clients-pkg-config.html
addtionally you should have intalled the library on your machine and export the path to PKG_CONFIG_PATH system variable. I would recommend you to see the second answer on the web site https://unix.stackexchange.com/questions/8241/how-to-make-pkg-config-path-variable-to-refer-an-installed-library/8246#8246?newreg=b2931268a854474e97636fd85a97de5b
After all you can compile with the following commands
gcc -c `pkg-config --cflags mysqlclient` server.c
gcc -o server server.o `pkg-config --libs mysqlclient`
and run ./server
 then in another terminal you can compile client. However before doing it you have to change ip addreass of your server machine manually in code in row 81
 Then you can compile it simply
 gcc -o client.c client
 then run ./client
