
Before compiling , please make sure you have the openssl dev package installed .<br/>
Compile using : 
       <br/> gcc -Wall -g -o ssl_shell.c ssl_shell.o -lssl -lcrypto</br>
As a side note, for some scenarios, you will probaly want to link statically against the library. 

You can either use ncat (http://edoceo.com/cli/ncat) or any other tool that will allow you to send commands to the client .
