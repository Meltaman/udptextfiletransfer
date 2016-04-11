/* Author: Andrew Cooper
*
*
*
*/

#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h> 

main(int argc, char const *argv[])
{
	
	char sourceFileName[20];
    char destFileName[20];
    char compName[20];
    char *getAt;
	int socketFile;
    char fileNameBuf[1024] = "";
    char fileBuffer[1024] = "";
	char readBuffer[1024] = "";
	time_t start_t;
	time_t end_t;
	
    struct sockaddr_in sender, receiver;
	ssize_t nread;
	
	// check for proper number of arguments
	if (argc != 3){
		printf("Format: ncp <source_file_name> <dest_file_name>@<comp_name>\n");
        return 1;

    }
	
	// get source file
    strcpy(sourceFileName, argv[1]);
    
	// determine where the @ sign is
	getAt = strchr(argv[2], '@');
	
	// check for appropriate format		
    if (getAt == NULL){
		printf("Format: ncp <source_file_name> <dest_file_name>@<comp_name>\n");
        return 1;
    }
	
	// find the destination file and the computer name
	*getAt = 0;
	strcpy(destFileName, argv[2]);
    strcpy(compName, getAt + 1);
	 	
	// set up socket
	socketFile = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&sender, sizeof(sender));
 	sender.sin_family = AF_INET;
	inet_aton("127.0.0.1",&sender.sin_addr);
 	sender.sin_port = htons(4647);
 	int connection = connect(socketFile, (struct sockaddr *)&sender,sizeof(sender));
 	
	if (connection != 0){
		printf("CONNECTION FAILURE WITH CODE %d\n",connection);
	}
	
	// copy source file into fileBuffer
	strcpy(fileBuffer, sourceFileName);
	
	// copy destination file name into fileNameBuf
	strcpy(fileNameBuf, destFileName);
	
	// copy the fileBuffer into another Buffer for reading
	strcpy(readBuffer, fileBuffer);
	
	// get buffer size
	int readFile = open(readBuffer, O_RDONLY);
	nread = read(readFile, readBuffer, 1024);
	close(readFile);	 			
 	
	// source file pointer
	FILE *f = fopen(sourceFileName, "rb");
	
	// get file size
	size_t pos = ftell(f);
	fseek(f, 0, SEEK_END);
	size_t length = ftell(f);
	fseek(f, pos, SEEK_SET);
	
	// initialize the number of packets needed to 1
	int chunkNumber = 1; 
	
	// temperory buffer to hold the current packet
	char saveBuffer[1050] = "";
	
	// if the length of the file is greater than buffer determine the number of needed packets
	if (length > 1024){
		chunkNumber = length/1024;
	}
	
	// initialize counter	
	int i = 0;
	
	// send desired destination file name
	sendto(socketFile, fileNameBuf, strlen(fileNameBuf), 0, (struct sockaddr *)&sender, sizeof(sender));
	
	// clear the destination name buffer
    memset(fileNameBuf, 0, sizeof(fileNameBuf));

			
	// reset file pointer to beginning of file
	fseek(f, pos, SEEK_SET);
	
	// start clock
	start_t = clock();
	
	// loop until the number of packets determined to be sent are sent
	for(i = 0; i < chunkNumber; i++){
		
		// point to location in file
		int location = fseek(f, i*1024, SEEK_SET);
	
		// read file to temporary buffer
		fread(saveBuffer, 1024, 1, f);
		
		// copy temporary buffer into fileBuffer
		strcpy(fileBuffer, saveBuffer);
		
		// clear temporary buffer
		memset(saveBuffer, 0, sizeof(saveBuffer)); 
			
		// send the fileBuffer
		sendto(socketFile, fileBuffer, strlen(fileBuffer), 0, (struct sockaddr *)&sender,sizeof(sender));
	 	
		// clear the fileBuffer
		memset(fileBuffer, 0, sizeof(fileBuffer));
				
	}
	
	// stop clock
	end_t = clock();

	printf("File named: %s sent for destination file: %s.\n",sourceFileName, destFileName);
    printf("Sent file size is %d bytes.\n", nread*i);
	double diff_t = difftime(end_t, start_t);	
	printf("Sending took %f seconds.\n", diff_t/CLOCKS_PER_SEC);

 //	close source file and socket
 fclose(f); 		
 close(socketFile);

}//	end main
