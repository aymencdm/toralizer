#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>



#define PROXY "127.0.0.1"
#define PORT   9050
#define USERNAME "toralize"
#define reqsize sizeof(struct proxy_request)
#define ressize sizeof(struct proxy_responce)


typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;


/*
+----+----+----+----+----+----+----+----+----+----+....+----+
		| VN | CD | DSTPORT |      DSTIP        | USERID       |NULL|
		+----+----+----+----+----+----+----+----+----+----+....+----+
 
           1    1      2              4           variable       1
*/

struct proxy_request {
    int8 vn;
    int8 cd;
    int16 distport;
    int32 distip;
    unsigned char username[9];
};
typedef struct proxy_request Req;




/*
		+----+----+----+----+----+----+----+----+
		| VN | CD | DSTPORT |      DSTIP        |
		+----+----+----+----+----+----+----+----+
 #  	   1    1      2              4
*/

struct proxy_responce {
    int8 vn;
    int8 cd;
    int16 DSTPORT;
    int32 DSTIP;
};
typedef struct proxy_responce Res;



Req *request(struct sockaddr_in*);
int connect(int , const struct sockaddr*,socklen_t );

