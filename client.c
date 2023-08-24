#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include "Headers/Utils.h"

int main(){
	struct sockaddr_in server;
	int sd,msgLength;
	char buff[50];
	char result;
	sd=socket(AF_INET,SOCK_STREAM,0);
	server.sin_family=AF_INET;
	server.sin_addr.s_addr=inet_addr("127.0.0.1"); 
	server.sin_port=htons(8080);
	connect(sd,(struct sockaddr *)&server,sizeof(server));
	while(1){
		printf("\e[1;1H\e[2J");
		printf("\n\n\n\t\t***********************************************************************************************************\n");
		printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$   WELCOME TO BANKING SYSTEM    $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		printf("\t\t***********************************************************************************************************\n\n\n");	
		printf("1.Sign In\n2.Exit\n\nEnter your Choice : ");
		int choice;
		scanf("%d",&choice);
		switch(choice)
		{
			case 1:
				chooseOption(sd);
				showMenu(sd);
				break;
			case 2:
				printf("\n\n\n\t\t***********************************************************************************************************\n");
				printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Thanks For Visiting Us $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    Have a Nice Day     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				printf("\t\t***********************************************************************************************************\n\n\n");
				exit(0);
				break;
			default:
				printf("Invalid Option ! Select valid option.\n");
				char c;
				while((c = getchar()) != '\n' && c != EOF);
				sleep(2);
				break;
		}
	}
	close(sd);
	return 0;
}
