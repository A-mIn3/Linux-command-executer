

## Compilation and Usage :
 
  Before compiling, please make sure you have the OpenSSL dev package installed.
  Compile using : </br>
       <b> gcc -Wall -g -o shell shell.c -lssl -lcrypto </b></br>

  You will probably want to link statically against the library to avoid dependencies issues on the target. 
  You can either use [ncat](http://edoceo.com/cli/ncat) or any other tool with ssl support to send commands to the client .
  To test the client, you can use openssl s_server utility on your server and wait for connections: </br>
       <b>openssl s_server -key key.pem -cert cert.pem -accept 443</b>



## Rubber Ducky Script :
  
  This script illusrates a simple use case scenario where we deliver our payload using HAK5 [Rubber Ducky USB stick](https://ducktoolkit.com).


## Disclaimer
Theses tools are made only for educational purposes and can be only used in legitimate penetration tests. Author does not take any responsibility for any actions taken by its users.
