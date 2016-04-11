/* Author: Andrew Cooper
* This program consists of 2 .c files: ncp.c and rcv.c.
* It is a udp server/client set up that transfers text from one file to another accross a udp connection.
* To use the udp client/server ncp rcv:
    1. Put both files into a directory.
    2. Have a file with contents you wish to copy also in the directory.
    3. Compile udp and ncp with gcc -o ncp ncp.c and gcc -o rcv rcv.c
    4. Open a second terminal window and navigate to the directory the executables are now located in.
    5. Execute rcv first by typing ./rcv and hitting return. Terminal should display "Receiving ..."
    6. Next go to the other terminal window.
    7. Type ./ncp yoursourcefile yourdestinationfile@computername and hit return.
    8. ncp should state that it is sending the source file to the destination file,
         size of the source file, and the time to send.
    9. ncp will stop executing once it sends a single file. To send a new file, run the program again
    10. rcv should display packets being transferred, their size, and time to receive them
    11. rcv will remain waiting for new transfers. ctrl + c to exit rcv program.
    If you wish to send more data to the SAME FILE you can continue to let this loop and send from the ncp function.
    If you wish to send more data to a NEW FILE you will need to ctrl + c out of rcv and re execute it before you send
        new files from ncp.
*/


#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<string.h>
#include<time.h>

main(int argc, char const *argv[]){
    
    int socketFile;
    char fileNameBuf[1024] = "";
    char fileBuffer[1024] = "" ;
    struct sockaddr_in sender, receiver;
    ssize_t nread = 1;
    ssize_t total = 0;
    time_t start_t;
    time_t end_t;
    
    // set up socket
    socketFile = socket(AF_INET,SOCK_DGRAM,0);
    bzero(&sender, sizeof(sender));
    sender.sin_family = AF_INET;
    sender.sin_port = htons(4647);
    int bindSocket = bind(socketFile,(struct sockaddr *)&sender,sizeof(sender));
    
    if (bindSocket != 0){
        printf("BINDING FAILED WITH CODE %d\n", bindSocket);
    }

    int length = sizeof(receiver);
    
    printf("Receiving. . .\n"); 
    
    // receive destination file name
    recvfrom(socketFile, fileNameBuf, 1024, 0, (struct sockaddr *)&receiver,&length);
    
    int count = 0;
    
    // loop
    for(;;)
    {
            
        // receive file from filebuffer		
        recvfrom(socketFile, fileBuffer, 1024, 0, (struct sockaddr *)&receiver,&length);
        // start timer
        start_t = clock();
        
        // open and write into designated destination file from buffer
        int writeFile = open(fileNameBuf,O_WRONLY|O_CREAT|O_APPEND,0644);
        nread =	write(writeFile, fileBuffer, 1024);
        
        // close destination file
        close(writeFile);
        
        // clear the fileBuffer			        			
        memset(fileBuffer, 0, sizeof(fileBuffer));
               
        count++;
        
        // end timing and determine time for packet reception
        end_t = clock();
        double diff_t = difftime(end_t, start_t);
        double totaltime = totaltime + diff_t;

        //printf("Packet received and written to destination file: %s.\n",fileNameBuf);	
        total = nread * count;
        
        printf("Packet received and written to destination file: %s.\n",fileNameBuf);
        printf("Received %d bytes from all packets so far. Current packet number: %d.\n", total, count);
        printf("Receiving took %f seconds.\n", totaltime/CLOCKS_PER_SEC);     
                    
    }

    // close the socket
    close(socketFile);
    return 0;

}// end main
