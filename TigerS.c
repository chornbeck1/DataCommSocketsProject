// Tiger Server that handles user authentication, with username creation.
// Users able to handle the action to download and upload files.
// 
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include "UserHeader.h"
#define PORT 4444
#define TRUE 1
#define FALSE 0
#define MAX_BUFFER 120
#define MAX_FILES  100
#define MAX_USERS 101
#define MAX_LENGTH 20
#define RECEIVED "\n Data has been received."
#define INVALID "Received wrong input."
#define users_database "users_database.txt"


void clear_buffer(char* buffer){
    for(int i = 0; i < 1024; i++)
    {
        buffer[i] = '\0';
    }
}


int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket;
    int numUsers=0;
    FILE* fp;
    char recvBuff[256];
    //int valread;
    struct sockaddr_in address;
    struct USER userList[MAX_USERS];
    int bytesReceived = 0;
    char final_path[1024] = {0};
    int opt = 1; 
    int addresslength = sizeof(address);
    char buffer[1024] = {0};
    char* command1;
    char* command2;
    char* command3;
    char* command4;
    char* users_file = "/home/pi/projects/DataCommProject1/serverFiles/usersList.txt";
    char* files_folder = "/home/pi/projects/DataCommProject1/serverFiles/";
    int userConnected=0;
    
    //Populate known users list
    char* line = NULL;
    size_t len = 0;
    fp = fopen(users_file, "r");
    while (getline(&line, &len, fp) != -1) {
		struct USER u;
		strncpy(u.username,strtok(line," \n"),MAX_BUFFER);
		strncpy(u.password,strtok(NULL," \n"),MAX_BUFFER);
		userList[numUsers++]=u;
    }
    fclose(fp);
       
    // Creating socket file descriptor 
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
                                                  &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address.sin_family = AF_INET; 
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons( PORT ); 
       
    // Forcefully attaching socket to the port 8080 
    if (bind(server_fd, (struct sockaddr *)&address,  
                                 sizeof(address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    printf("Server socket bound.\n");
    printf("Listening for clients...\n");
	listen(server_fd,3);

    while(1) {
		if(userConnected==1)
		{
			//fflush(0);
			//Clear buffers
			//clear_buffer(final_path);
			clear_buffer(buffer);
			
			//Receive command from the client
			printf("Waiting for client command...\n>>> ");
			//Check if client has disconnected?
			if(recv(new_socket,buffer,sizeof(buffer),MSG_PEEK | MSG_DONTWAIT)==0)
			{
				printf("Client Disconnected\n");
				command1="exit";
			}
			else
			{
				read(new_socket, buffer, 1024);
				printf("%s\n",buffer);
				if (buffer == NULL){
					printf("%s\n",buffer);
					printf("\nInvalid Command Received \n");
					printf("%s\n",buffer);
					printf("\nPlease input another command. \n");
					clear_buffer(buffer);
				}
				else {
					//Parse apart the possible commands
					command1 = strtok(buffer, " \n");
					command2 = strtok(NULL, " \n");
					command3 = strtok(NULL, " \n");
					command4 = strtok(NULL, " \n");
				}
			}
			
			
			if (strncmp(command1,"tconnect", sizeof(buffer))==0) {
				printf("\nInput received.");
				printf("\n Your inputted Username is : %s",command3);
				fflush(stdout);
				printf("\n Your inputted Password is : %s",command4);
				fflush(stdout);
				printf("\nRunning Credentials.\n");
				//Loop through UserList to find username/password combo
				int found=0;
				for(int i=0;i<MAX_USERS;i++)
				{
					//Make sure a user exists in slot
					if(found==0)
					{
						//Compare usernames
						if(strncmp(command3,userList[i].username,strlen(command3))==0)
						{
							//If exists, compare passwords
							if(strncmp(command4,userList[i].password,strlen(command4))==0)
							{
								found=1;
							}
						}
					}
				}
				//No match
				if(found==0)
				{
					write(new_socket,"-1",1);
				}
				//Match
				else
				{
					write(new_socket,"1",1);
				}
			}
			else if (strncmp(command1,"tget", sizeof(buffer))==0) {
				clear_buffer(final_path);
				printf("Input received.\n");
				printf("File Download Requested\n");
				if(strncmp(command2, "", sizeof(buffer)) == 0){
					printf("No specified file.\n");
					char* rsp = "-2";
					write(new_socket, rsp, strlen(rsp));
				}
				else{
					strcat(final_path,files_folder);
					strcat(final_path,command2);
					FILE *download_file = fopen(final_path,"rb");
					if(download_file==NULL)
					{
						write(new_socket, "-1", 1);
						printf("Failure to open file\n");
					}
					else
					{
						write(new_socket, "1", 1);
						printf("Ready to Send\n");
						read(new_socket,buffer,MAX_BUFFER);
						int total=0;
						while (1) {
							unsigned char buff[256]={0};
							int nread = fread(buff,1,256,download_file);
							if(nread==0)
							{
								//Send 'done' code to prevent perfect-byte transfer
								write(new_socket, "10110011011",11);
							}
							if(nread > 0){
								
								printf("Sending %d Bytes\n",nread);
								write(new_socket, buff, nread);
								total+=nread;
							}
							if(nread < 256)
							{
								if(feof(download_file)){
									printf("End of file.\nSuccessfully sent %d bytes\n",total);
								}
								if(ferror(download_file)){
									printf("Error reading\n");
								}
								break;
							}
						}
					}
				}
			}
			else if (strncmp(command1,"tput", sizeof(buffer))==0) {
				clear_buffer(final_path);
				printf("Input received.\n");
				if(strncmp(command2, "", sizeof(buffer)) == 0){
					//Send back initial response as failure
					printf("No specified file.\n");
					char* rsp = "-2";
					write(new_socket, rsp, strlen(rsp));
				}
				else {
					//Start to prepare a file to receive the data
					strcat(final_path,files_folder);
					strcat(final_path,command2);
					fp = fopen(final_path,"wb");
					if(NULL == fp){
						printf("Error opening file");
						return 1;
					}
					printf("Ready to Accept...\n");
					//Send back initial response as success
					write(new_socket, "1", 1);
					int total=0;
					while((bytesReceived = recv(new_socket,recvBuff,256,0)) > 0){
                        if(bytesReceived<256)
                        {
                            if(strstr(recvBuff,"10110011011")==NULL)
                            {
                                int written = fwrite(recvBuff,1,bytesReceived,fp);
                                printf("Wrote %d bytes.\n", written); 
                            }
                        }
                        else
                        {
							int written = fwrite(recvBuff,1,bytesReceived,fp);
							printf("Wrote %d bytes.\n", written);
							total+=bytesReceived;
						}
						if(bytesReceived < 256){
							printf("File Transfer Complete\nSuccessfully received %d bytes\n",total);
							break;
						}
					}
					if(fclose(fp)!=0)
					{
						printf("Error closing filestream\n");
					}
					if(bytesReceived < 0)
					{
						printf("\n Write Error \n");
					}
				}
			}
			else if (strncmp(command1,"exit", sizeof(buffer))==0){
				printf("User Disconnection\n");
                if(close(new_socket)<0)
                {
                    printf("Failure closing server-side socket\n");
                }
				userConnected=0;
			}
		}
        //Accept the next incoming connection
        else
        {
            new_socket=accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addresslength);
            printf("---Connection acquired---\n");
            userConnected=1;
        }
    }
    return 0; 
} 
