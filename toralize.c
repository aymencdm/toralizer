#include "toralize.h"



Req *request(struct sockaddr_in *sock2){
    Req *req;
    req=malloc(reqsize);
    if (req == NULL) {
        perror("malloc");
        return NULL;
    }

    memset(req, 0, reqsize);


    req->vn=4;
    req->cd=1;
    req->distip=sock2->sin_addr.s_addr;
    req->distport=sock2->sin_port;
    strncpy(req->username,USERNAME,8);
    req->username[8] = 0;


    return req;
}


int connect(int s2, const struct sockaddr *sock2,socklen_t addrlen){

    int (*c)(int , const struct sockaddr*,socklen_t );
    
    int s;
    struct sockaddr_in sock;
    Req *req;
    Res *res;
    char buff[ressize];
    int success;
    c = dlsym(RTLD_NEXT,"connect");
    if (c == NULL){
        fprintf(stderr, "Error loading connect function: %s\n", dlerror());
        return -1;
    }


    // Check if this is already a connection to our proxy
    struct sockaddr_in *target = (struct sockaddr_in *)sock2;
    if (target->sin_family == AF_INET && 
        target->sin_addr.s_addr == inet_addr(PROXY) && 
        ntohs(target->sin_port) == PORT) {
        
        return c(s2, sock2, addrlen);
    }




    s=socket(AF_INET,SOCK_STREAM,0);
    if (s<0){
        perror("socket");
        return -1;

    };
    //socket struct
    sock.sin_family= AF_INET;
    sock.sin_port=htons(PORT);
    sock.sin_addr.s_addr=inet_addr(PROXY);


    //connect
    if (c(s,(struct sockaddr*) &sock,sizeof(sock))!=0){
        perror("connect");
        printf("Trying to connect to %s:%d\n", PROXY, PORT);
        close(s);
        return -1;

    };
    printf("connected to proxy\n");



    //reqwest function
    req=request((struct sockaddr_in *)sock2);
   
    if (write(s, req, reqsize) != reqsize) {
        perror("write to proxy");
        free(req);
        close(s);
        return -1;
    }



    //empty buff
    memset(buff,0,ressize);



    //read data
    int bytes_read = read(s, buff, ressize);
    if (bytes_read < ressize) {
        perror("read from proxy");
        printf("Expected %d bytes, got %d\n", ressize, bytes_read);
        free(req);
        close(s);
        return -1;
    }

    
    //accsess buff data
    res=(Res*) buff;
    success=(res->cd==90);
    if (!success) {
        fprintf(stderr, "SOCKS4 error - response code: %d\n", res->cd);
        switch(res->cd) {
            case 91: fprintf(stderr, "Request rejected or failed\n"); break;
            case 92: fprintf(stderr, "Request rejected - cannot connect to identd\n"); break;
            case 93: fprintf(stderr, "Request rejected - identd reports different user-id\n"); break;
            default: fprintf(stderr, "Unknown error code\n"); break;
        }
        free(req);
        close(s);
        return -1;
    }


    printf("seccessfully connected thrugh the proxy\n");
    
    dup2(s,s2);
    close(s);
    free(req);
    return 0;


} 