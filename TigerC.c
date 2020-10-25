#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include "UserHeader.h"
#define PORT 4444


int bytesRead;
int byesToRead;
char fileBuffer[1024] = {0};
char* clientFilesPath = "/home/pi/projects/DataCommProject1/clientFiles/";

void ClearBuffer(char* buffer){
    for(int i = 0; i < 1024; i++)
    {
        buffer[i] = '\0';
    }
}


int main(int argc, char const *argv[]) 
{ 
    int socketNum = 0;
    int bytesReceived = 0;
    int connected=0;
    char filePath[1024] = {0};
    char recvBuff[256];
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char input[1024] = {0};
    char serverResponse[1024] = {0};
    char* command;
    FILE *fp;
    
    //Welcome messag
    printf("\nWelcome to TigerS, please enter a command. \n"
           "Your options are: \n"
           "tconnect <TigerS IP Address> <User> <Password>\n"
           "tget <File Name>\n"
           "tput <File Name>\n"
           "exit");
    
    while(1)
    {
        ClearBuffer(input);
        ClearBuffer(buffer);
        //Ask user for input.
        printf("\n\n>>> ");
               
        //Retreive input
        fgets(input, MAX_BUFFER, stdin);
        strncpy(buffer,input,strlen(input));
        command=strtok(buffer, " \n");
        
        
        //If command is to connect, and not already connected
        if(strncmp(command,"tconnect",strlen(command))==0)
        {
            //Already connected
            if(connected>0){printf("Already connected to server\n");}
            //New connection
            else
            {
                serv_addr.sin_family = AF_INET; 
                serv_addr.sin_port = htons(PORT);
                
                printf("Connecting to Server...\n");
                char* ip=strtok(NULL, " \n");
                // Convert IP Address to binary form
                if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)  
                { 
                    printf("\nInvalid address\n"); 
                    return -1; 
                }
        
                
                //Create Socket
                if ((socketNum = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
                { 
                    printf("\n Socket creation error \n"); 
                    return -1; 
                }
    
                //Connect to the server
                if (connect(socketNum, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
                { 
                    printf("\nConnection Failed \n"); 
                    return -1; 
                }
                write(socketNum,input,strlen(input));
                //Read back success/failure message from server and parse
                read(socketNum,serverResponse,1024);
                int status=atoi(strtok(serverResponse, " \n"));
                
                if(status>0)
                {
                    printf("Connection Successful. Welcome!\n");
                    connected=1;
                }
                else
                {
                    printf("Invalid Credentials\n");
                    printf("Would you like to create a new account with these credentials? (y/n)\n>>> ");
                    fgets(input,MAX_BUFFER,stdin);
                }
                
                
            }
        }
        //Close the socket
        else if(strncmp(command,"exit",strlen(command))==0)
        {
            if(connected==1)
            {
                //Tell server to close socket
                printf("Initiating socket shutdown...\n");
                write(socketNum,input,strlen(input));
                int status=1;
                if(status<0 || close(socketNum)<0)
                {
                    printf("Failure closing client-side socket\n");
                }
                else
                {
                    printf("Socket Closed.\n");
                }
            }
            connected=0;
            return -1;
        }
        //If connected and requesting tget
        else if(strncmp(command,"tget",strlen(command))==0)
        {
            //Not connected to a server
            if(connected==0){printf("Not connected to server\n");}
            else
            {
                ClearBuffer(filePath);
                char* fileName=strtok(NULL, " \n"); 
                strcat(filePath, clientFilesPath);
                strcat(filePath, fileName); 
                write(socketNum,input,strlen(input));
                //Read back success/failure message from server and parse
                read(socketNum,serverResponse,1024);
                int status=atoi(strtok(serverResponse, " \n"));
                if(status>0)
                {              
                    fp=fopen(filePath,"wb");
                    if(NULL == fp){
                        printf("Error opening file");
                        return 1;
                    }
                    printf("Ready to receive\n");
                    write(socketNum,"1",1);
                    int total=0;
                    while((bytesReceived = read(socketNum,recvBuff,256)) > 0){
                        //If only an EOF is received, server finished sending file.
                        if(strncmp(recvBuff,"EOF",3)==0)
                            break;
                        fwrite(recvBuff,1,bytesReceived,fp);
                        total+=bytesReceived;
                    }
                    printf("File Transfer Complete\nSuccessfully downloaded %d bytes\n",total);
                    fclose(fp);
                    if(bytesReceived < 0)
                    {
                        printf("\n Read Error \n");
                    }
                }
                else
                {
                    printf("Invalid File Transfer Request\n");
                }
            }
        }
        //if connected and requesting tput
        else if(strncmp(command,"tput",strlen(command))==0)
        {
            //Not connected to a server
            if(connected==0){printf("Not connected to server\n");}
            else
            {
                ClearBuffer(filePath);
                char* fileName=strtok(NULL, " \n"); 
                strcat(filePath, clientFilesPath);
                strcat(filePath, fileName); 
                if((fp=fopen(filePath,"rb"))==NULL)
                {
                    printf("Invalid File Transfer Request\n");
                }
                else
                {
                    write(socketNum,input,strlen(input));
                    //Read back success/failure message from server and parse
                    read(socketNum,serverResponse,1024);
                    int status=atoi(strtok(serverResponse, " \n"));
                    if(status>0)
                    {
                        int total=0;
                        while (1) {
                            unsigned char buff[256]={0};
                            int nread = fread(buff,1,256,fp);
                            //If can read no more, send specific EOF to client
                            if(nread==0)
                            {
                                write(socketNum, "EOF",3);
                                break;
                            }
                            write(socketNum, buff, nread);
                            total+=nread;
                        }
                        printf("Upload Complete\nSuccesfully sent %d bytes\n",total);
                        if(fclose(fp)!=0)
                        {
                            printf("Failure closing file\n");
                        }
                    }
                    else
                    {
                        printf("Failure during tput\n");
                    }
                }
            }
        }         
        //Loop handling input parsing and sending
    }

}
     
