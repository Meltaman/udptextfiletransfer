/* Author: Andrew Cooper
*  
*
*
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
