#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<ctype.h>

#include "StructuresDef.h"

union semun
{		
int val; 
} semSet,semSet1;

int semIdentifier, semIdentifier1;
bool lock_critical_section(struct sembuf *semOp, int semIdentifier);
bool unlock_critical_section(struct sembuf *sem_op, int semIdentifier);

#define mode 0744
#define flags O_RDWR | O_CREAT

void normalUserLogin(int sd);
void jointUserLogin(int sd);
void adminLogin(int sd);
void showMenu(int sd);
void deposit(int sd);
void withdraw(int sd);
void balanceEnquiry(int sd);
void changePassword(int sd);
void viewDetails(int sd);
void addAccount(int sd);
void deleteAccount(int sd);
void modifyAccount(int sd);
void searchAccount(int sd);
void createAdmin(int sd);
int option,currUserID;

bool lock_critical_section(struct sembuf *semOp, int semIdentifier)
{
    semOp->sem_flg = SEM_UNDO;
    semOp->sem_op = -1;
    semOp->sem_num = 0;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error While Locking Critical Section");
        return false;
    }
    return true;
}

bool unlock_critical_section(struct sembuf *semOp, int semIdentifier)
{
    semOp->sem_op = 1;
    int semopStatus = semop(semIdentifier, semOp, 1);
    if (semopStatus == -1)
    {
        perror("Error While Operating on Semaphore!");
        _exit(1);
    }
    return true;
}


void chooseOption(int sd){
	printf("\e[1;1H\e[2J");
	printf("\n\n\n\t\t***********************************************************************************************************\n");
	printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$   WELCOME TO BANKING SYSTEM  Account Login Mode  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
	printf("\t\t***********************************************************************************************************\n\n\n");
	printf("1 : Normal User Login\n");
	printf("2 : Joint Account User Login\n");
	printf("3 : Administrator Login\n");
	printf("4 : Exit\n");
	printf("Choose an Option : ");
	scanf("%d",&option);
	switch(option){
		case 1 :
			normalUserLogin(sd);
			break;
		case 2 :
			jointUserLogin(sd);
			break;
		case 3 :
			adminLogin(sd);
			break;
		case 4 :
			printf("\n\n\n\t\t***********************************************************************************************************\n");
			printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Thanks For Visiting Us $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    Have a Nice Day     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
			printf("\t\t***********************************************************************************************************\n\n\n");
			exit(0);
			break;
		default :
			printf("Invalid Option ! Select Valid Option.\n");
			char c;
			while((c = getchar()) != '\n' && c != EOF);
			sleep(2);
			chooseOption(sd);
			break;
	}
	return;
}

void showMenu(int sd){
	int select;
	if(option==1 || option==2){
		printf("\e[1;1H\e[2J");
		printf("\n\n\n\t\t***********************************************************************************************************\n");
		printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$   WELCOME TO BANKING SYSTEM  User Portal  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		printf("\t\t***********************************************************************************************************\n\n\n");
		printf("1 : Deposit Money\n");
		printf("2 : Withdraw Money\n");
		printf("3 : Balance Enquiry\n");
		printf("4 : Change my Password\n");
		printf("5 : View Passbook\n");
		printf("6 : Exit\n");
		printf("Choose an Option : ");
		scanf("%d",&select);
		switch(select){
			case 1 :
				deposit(sd);
				break;
			case 2 :
				withdraw(sd);
				break;
			case 3 :
				balanceEnquiry(sd);
				break;
			case 4 :
				changePassword(sd);
				break;
			case 5 :
				viewDetails(sd);
				break;
			case 6 :
				int todo = 6;
				write(sd,&todo,sizeof(int));
				printf("\n\n\n\t\t***********************************************************************************************************\n");
				printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Thanks For Visiting Us $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    Have a Nice Day     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				printf("\t\t***********************************************************************************************************\n\n\n");
				exit(0);
			default :
				printf("Invalid Option ! Select Valid Option.\n");
				char c;
				while((c = getchar()) != '\n' && c != EOF);
				sleep(2);
				showMenu(sd);
				break;
		}
	}
	else if(option==3){
		printf("\e[1;1H\e[2J");
		printf("\n\n\n\t\t***********************************************************************************************************\n");
		printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$   WELCOME TO BANKING SYSTEM  Admin Portal  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
		printf("\t\t***********************************************************************************************************\n\n\n");
		printf("1 : Add Account\n");
		printf("2 : Delete Account\n");
		printf("3 : Modify Account\n");
		printf("4 : Search Account\n");
		printf("5 : Create New Admin\n");
		printf("6 : Exit\n");
		printf("Choose an Option : ");
		scanf("%d",&select);
		switch(select){
			case 1 :
				addAccount(sd);
				break;
			case 2 :
				deleteAccount(sd);
				break;
			case 3 :
				modifyAccount(sd);
				break;
			case 4 :
				searchAccount(sd);
				break;
			case 5 :
				createAdmin(sd);
				break;
			case 6 :
				int todo = 6;
				write(sd,&todo,sizeof(int));
				printf("\n\n\n\t\t***********************************************************************************************************\n");
				printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Thanks For Visiting Us $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				printf("\t\t$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$    Have a Nice Day     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
				printf("\t\t***********************************************************************************************************\n\n\n");
				exit(0);
			default :
				printf("Invalid Option ! Select a Valid Option.\n");
				char c;
				while((c = getchar()) != '\n' && c != EOF);
				sleep(2);
				showMenu(sd);
				break;
		}
	}
}

void normalUserLogin(int sd){   
	bool result;
	normalUser currUser;
	printf("User ID : ");
	scanf("%d",&currUser.userID);
	if(currUser.userID<1001 || currUser.userID>9999)
		currUser.userID = -1;
	//printf("%d", currUser.userID);
	currUserID=currUser.userID;
  	char *pass=getpass("Password: ");
  	memcpy(currUser.password,pass,sizeof(pass));
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(normalUser));
	read(sd,&result,sizeof(result)); 
        
	if(!result){
		printf("\n\t\t************************************** Invalid Login !!! ********************************************\n");
		char c;
		while((c = getchar()) != '\n' && c != EOF);
		sleep(2);
		chooseOption(sd);
	}
	else{
		printf("\n\t******************************!!! Successfully Logged In !!!****************************\n\n");
		key_t semKey = ftok("Database/Normal_user", currUser.userID); // Generate a key based on the account number hence, different customers will have different semaphores

		int semctlStatus;
		semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
		if (semIdentifier == -1)
		{
		    semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
		    if (semIdentifier == -1)
		    {
		        perror("Error While Creating Semaphore!");
		        _exit(1);
		    }
		    semSet.val = 1; // Set a binary semaphore
		    semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
		    if (semctlStatus == -1)
		    {
		        perror("Error While Initializing a Binary Sempahore!");
		        _exit(1);
		    }
		}
		int semctlStatus1;
		semIdentifier1 = semget(semKey, 1, 0); // Get the semaphore if it exists
		if (semIdentifier1 == -1)
		{
		    semIdentifier1 = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
		    if (semIdentifier1 == -1)
		    {
		        perror("Error While Creating Semaphore!");
		        _exit(1);
		    }

		    semSet1.val = 1; // Set a binary semaphore
		    semctlStatus1 = semctl(semIdentifier1, 0, SETVAL, semSet1);
		    if (semctlStatus1 == -1)
		    {
		        perror("Error While Initializing a Binary Sempahore!");
		        _exit(1);
		    }
		}
	}
	return;
}

void jointUserLogin(int sd){
	bool result;
	jointUser currUser;    
	printf("User ID : ");
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
  	char *pass=getpass("Password: ");
  	memcpy(currUser.password,pass,sizeof(pass));
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(jointUser));
	read(sd,&result,sizeof(result)); 
	if(!result){
		printf("\n\t\t************************************** Invalid Login !!! ******************************************\n");
		char c;
		while((c = getchar()) != '\n' && c != EOF);
		sleep(2);
		chooseOption(sd);
	}
	else{
		printf("\n\t*******************************!!! Successfully Logged In !!!***************************\n\n");
		key_t semKey = ftok("Database/Joint_user", currUser.userID); // Generate a key based on the account number hence, different customers will have different semaphores
		int semctlStatus;
		semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
		if (semIdentifier == -1)
		{
		    semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
		    if (semIdentifier == -1)
		    {
		        perror("Error While Creating Semaphore!");
		        _exit(1);
		    }

		    semSet.val = 1; // Set a binary semaphore
		    semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
		    if (semctlStatus == -1)
		    {
		        perror("Error While Initializing a Binary Sempahore!");
		        _exit(1);
		    }
		}
		int semctlStatus1;
		semIdentifier1 = semget(semKey, 1, 0); // Get the semaphore if it exists
		if (semIdentifier1 == -1)
		{
		    semIdentifier1 = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
		    if (semIdentifier1 == -1)
		    {
		        perror("Error While Creating Semaphore!");
		        _exit(1);
		    }

		    semSet1.val = 1; // Set a binary semaphore
		    semctlStatus1 = semctl(semIdentifier1, 0, SETVAL, semSet1);
		    if (semctlStatus1 == -1)
		    {
		        perror("Error While Initializing a Binary Sempahore!");
		        _exit(1);
		    }
		}
	}
	return;
}

void adminLogin(int sd){
	bool result;
	admin currUser;
	printf("User ID : ");
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
  	char *pass=getpass("Password: ");
  	memcpy(currUser.password,pass,sizeof(pass));
	write(sd,&option,sizeof(int));
	write(sd,&currUser,sizeof(admin));
	read(sd,&result,sizeof(result));
	if(!result){
		printf("\n\t\t************************************** Invalid Login !!! *******************************************\n");
		char c;
		while((c = getchar()) != '\n' && c != EOF);
		sleep(2);
		chooseOption(sd);
	}
	else{
		printf("\n\t********************************!!! Successfully Logged In !!!*****************************\n\n");
	}
	return;
}

void deposit(int sd){
	float amt;
	int flag = 0;
	int todo=1;
	bool result;
	struct sembuf semOp;
	while(!semctl(semIdentifier, 0, GETVAL, semSet)){
		for ( int loop = 0; loop < 1; ++loop) {
        		for ( int each = 0; each < 4; ++each) {
            			printf ("\rLoading%.*s   \b\b\b", each, "...");
            			fflush (stdout);
            			sleep(1);
        	}
    	}
	}
    	lock_critical_section(&semOp,semIdentifier);
    	printf("\nDone !\n");
	printf("\nDeposit Amount : Rs.");
	while(scanf("%f", &amt)!=1 || getchar()!='\n')
    	{
        scanf("%*[^\n]%*c");
        printf("\n\t\t******************************** Enter a Valid Amount!! ****************************************\n");
        printf("Deposit Amount : Rs.");
    	}
	while(amt<=0){
		printf("\n\t\t******************************** Enter a Valid Amount!! ****************************************\n");
		printf("Deposit Amount : Rs.");
		while(scanf("%f", &amt)!=1 || getchar()!='\n')
    		{
        		scanf("%*[^\n]%*c");
        		printf("\n\t\t******************************** Enter a Valid Amount!! ****************************************\n");
        		printf("Deposit Amount : Rs.");
    		}
	}
	write(sd,&todo,sizeof(int));
	write(sd,&amt,sizeof(float));
	read(sd,&result,sizeof(result)); 
	unlock_critical_section(&semOp,semIdentifier);
	if(!result){
		printf("\n\n\t\t**********************************  Error While Depositing Money !!! ********************************************\n");
	}
	else{
		printf("\n\n\t\t*********************************!!! Successfully Deposited Money !!!********************************************\n");
	}
	sleep(2);
	showMenu(sd);
	return;
}

void withdraw(int sd){
	float amt;
	int todo=2;
	bool result;
	struct sembuf semOp1;
	while(!semctl(semIdentifier1, 0, GETVAL, semSet1)){
		for ( int loop = 0; loop < 1; ++loop) {
        		for ( int each = 0; each < 4; ++each) {
            			printf ("\rLoading%.*s   \b\b\b", each, "...");
            			fflush (stdout);
            			sleep (1);
        	}
    		}
	}
    	lock_critical_section(&semOp1, semIdentifier1);
    	printf("\nDone !\n");
	printf("\nWithdrawal Amount : Rs.");
	while(scanf("%f", &amt)!=1 || getchar()!='\n')
    	{
        scanf("%*[^\n]%*c");
        printf("\n\t\t******************************** Enter a Valid Amount!! ****************************************\n");
        printf("Withdrawal Amount : Rs.");
    	}
	while(amt<=0){
		printf("\n\t\t******************************** Enter a Valid Amount!! ****************************************\n");
		printf("Withdrawal Amount : Rs.");
		while(scanf("%f", &amt)!=1 || getchar()!='\n')
    		{
        		scanf("%*[^\n]%*c");
        		printf("\n\t\t******************************** Enter a Valid Amount!! ****************************************\n");
        		printf("Withdrawal Amount : Rs.");
    		}
	}
	write(sd,&todo,sizeof(int));
	write(sd,&amt,sizeof(float));
	read(sd,&result,sizeof(result));
	unlock_critical_section(&semOp1, semIdentifier1);
	if(!result){
		printf("\n\n\t********************* Error Withdrawing Your Money, Please Check Your Balance!! ******************************\n\n");
	}
	else{
		printf("\n\n\t\t**********************************!!! Successfully Withdrew Money !!!********************************************\n");
	}
	sleep(2);
	showMenu(sd);
	return;
}

void balanceEnquiry(int sd){
	float amt;
	int todo=3;
	int len;
	write(sd,&todo,sizeof(int));
	len=read(sd,&amt,sizeof(float));	
	printf("\nAvailable Balance : Rs.");
	printf("%0.2f\n\n",amt);
	printf("\n\nPress ENTER to Go Back to Menu\n");
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	scanf("%c",&c);
	showMenu(sd);
	return;
}

void changePassword(int sd){
	int todo=4;
	char nPassword[10];
	bool result;
  	char *pass=getpass("\nEnter the New Password (max 10 characters): ");
  	memcpy(nPassword,pass,sizeof(pass));
	write(sd,&todo,sizeof(int));
	write(sd,nPassword,sizeof(nPassword));
	read(sd,&result,sizeof(result));
	if(!result){
		printf("\n\n\t************************* Error Changing Your Password !! ****************************\n\n");
	}
	else{
		printf("\n\n\t**************************!!! Successfully Changed Your Password !!!*****************************\n\n");
	}
	sleep(2);
	showMenu(sd);
	return;
}
void viewDetails(int sd){
	int todo=5;
	write(sd,&todo,sizeof(int));
	if(option==1){
		normalUser currUser1;
		read(sd,&currUser1,sizeof(normalUser));
		printf("\n*********************************** User Details *********************************************\n");
		printf("\nUser ID : %d\n",currUser1.userID);
		printf("Name : %s\n",currUser1.name);
		printf("Account Number : %d\n",currUser1.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser1.balance);
		printf("Status : %s\n\n",currUser1.status);
		char buff[10000];
		int len=read(sd,buff,10000);
		if(strcmp(buff,"empty")==0)
		{
		printf("\n\n*************************** Your passbook is empty !!! ******************************\n\n");
		}
		else
		{
		printf("\n\n******************************** Transaction Details ****************************************");
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
		printf("\n%s",buff);
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n");
		}
		
	}
	else if(option==2){
		jointUser currUser2;
		read(sd,&currUser2,sizeof(jointUser));
		printf("\n*********************************** User Details ********************************************\n");
		printf("\nUser ID : %d\n",currUser2.userID);
		printf("Main Account Holder's Name : %s\n",currUser2.name1);
		printf("Other Account Holder's Name : %s\n",currUser2.name2);
		printf("Account Number : %d\n",currUser2.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser2.balance);
		printf("Status : %s\n\n",currUser2.status);
		char buff[10000];
		int len=read(sd,buff,10000);
		if(strcmp(buff,"empty")==0)
		{
		printf("\n\n*************************** Your passbook is empty !!! ******************************\n\n");
		}
		else
		{
		printf("\n\n******************************** Transaction Details ****************************************");
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
		printf("\n%s",buff);
		printf("\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\n");
		}
	}
	printf("\n\nPress ENTER to go Back to Menu\n");
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	scanf("%c",&c);
	showMenu(sd);
	return;
}

void addAccount(int sd){
	int todo=1;
	int type;
	bool result;
	write(sd,&todo,sizeof(int));
	printf("\nEnter the Type of Account (1: Normal Account 2: Joint Account) : ");
	while(scanf("%d", &type)!=1 || getchar()!='\n'){
        scanf("%*[^\n]%*c");
        printf("\nPlease Select proper Account Type : ");
    	}
	while(type<1 || type>2){
		printf("\nPlease Select proper Account Type : ");
		while(scanf("%d", &type)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nPlease Select proper Account Type : ");
    	}
	}
	write(sd,&type,sizeof(int));
	if(type==1){
		normalUser addUser1;
		printf("Name of the Account Holder : ");
		scanf(" %[^\n]",addUser1.name);
    		char *pass=getpass("Password (max 10 characters) : ");
    		memcpy(addUser1.password,pass,sizeof(pass));
		printf("Initial Deposit : Rs.");
		while(scanf("%f", &addUser1.balance)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter a Float Value : ");
    		}
		while(addUser1.balance<0){
			printf("\nENTER VALID AMOUNT!! (can start from 0) Rs. ");
			while(scanf("%f", &addUser1.balance)!=1 || getchar()!='\n'){
        		scanf("%*[^\n]%*c");
        		printf("\nEnter a Float Value : ");
    		}
		}
		write(sd,&addUser1,sizeof(normalUser));
		read(sd,&addUser1,sizeof(normalUser)); 
		if(strcmp(addUser1.status,"ACTIVE")){
			printf("\n\n\t************************** Error While Adding Account ! ************************************\n\n");
		}
		else{
			printf("\n\n\t**************************!!! Successfully Added the Account !!!*****************************\n\n");
		}
	}

	if(type==2){
		jointUser addUser2;
		printf("Name of the Primary Account Holder : ");
		scanf(" %[^\n]",addUser2.name1);
		printf("Name of the Other Account Holder : ");
		scanf(" %[^\n]",addUser2.name2);
   	 	char *pass=getpass("Password(max 10 characters): ");
    		memcpy(addUser2.password,pass,sizeof(pass));
		printf("Initial Deposit : Rs.");
		while(scanf("%f", &addUser2.balance)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter a Float Value : ");
    		}
		while(addUser2.balance<0){
			printf("\nENTER VALID AMOUNT!! (can start from 0) Rs. ");
			while(scanf("%f", &addUser2.balance)!=1 || getchar()!='\n'){
        		scanf("%*[^\n]%*c");
        		printf("\nEnter a Float Value : ");
    		}
		}
		write(sd,&addUser2,sizeof(jointUser));
		read(sd,&addUser2,sizeof(jointUser));
		if(strcmp(addUser2.status,"ACTIVE")){
			printf("\n\n\t************************** Error While Adding Account ! ************************************\n\n");
		}
		else{
			printf("\n\n\t**************************!!! Successfully Added the Account !!!*****************************\n\n");
		}
	}
	sleep(2);
	showMenu(sd);
	return;
}

void deleteAccount(int sd){
	int todo=2;
	int type,userID;
	int result;

	write(sd,&todo,sizeof(int));

	printf("\nEnter the Type of Account (1: Normal Account 2: Joint Account) : ");
	while(scanf("%d", &type)!=1 || getchar()!='\n'){
        scanf("%*[^\n]%*c");
        printf("\nPlease Select Proper Account Type : ");
    	}
	while(type<1 || type>2){
		printf("\nPlease Select Proper Account Type : ");
		while(scanf("%d", &type)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nPlease Select Proper Account Type : ");
    	}
		printf("\n");
	}

	write(sd,&type,sizeof(int));

	printf("User ID : ");
	while(scanf("%d", &userID)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter an Integer Value : ");
    	}
	write(sd,&userID,sizeof(int));
	read(sd,&result,sizeof(result));
	if(result==-1){
		printf("\n\n\t\t********************* Error Deleting the Account, Balance in the Account is not Zero *********************\n\n");
	}
	else if(result==0){
		printf("\n\n\t************************ Error Deleting the Account, Please Re-check the User ID !! ********************\n\n");
	}
	else{
		printf("\n\n\t**************************!!! Successfully Deleted the Account !!!*****************************\n\n");
	} 
	sleep(2);
	printf("\n\nPress ENTER to go Back to Menu\n");
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	showMenu(sd);
	return;
}

void modifyAccount(int sd){
	int todo=3;
	int type;
	bool result;

	write(sd,&todo,sizeof(int));

	printf("\nEnter the Type of Account (1: Normal Account 2: Joint Account) : ");
	while(scanf("%d", &type)!=1 || getchar()!='\n'){
        scanf("%*[^\n]%*c");
        printf("\nPlease Select Proper Account Type : ");
    	}
	while(type<1 || type>2){
		printf("\nPlease Select Proper Account Type : ");
		while(scanf("%d", &type)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nPlease Select Proper Account Type : ");
    	}
	}

	write(sd,&type,sizeof(int));

	if(type==1){
		normalUser modUser1;
		printf("User ID : ");
		while(scanf("%d", &modUser1.userID)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter an Integer Value : ");
    		}
		printf("Account Number : ");
		while(scanf("%d", &modUser1.account_no)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter an Integer Value : ");
    		}	
		printf("New Name of the Account Holder : ");
		scanf(" %[^\n]",modUser1.name);
    		char *pass=getpass("Password(max 10 characters): ");
    		memcpy(modUser1.password,pass,sizeof(pass));
		printf("New Balance : Rs.");
		while(scanf("%f", &modUser1.balance)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter a Float Value : ");
    		}
		while(modUser1.balance<0){
			printf("Account Balance cannot be less then 0\n");
			printf("Please Enter Proper Value : \n");
			while(scanf("%f", &modUser1.balance)!=1 || getchar()!='\n'){
        		scanf("%*[^\n]%*c");
        		printf("\nEnter a Float Value : ");
    		}
		}
		write(sd,&modUser1,sizeof(normalUser));
	}

	if(type==2){
		jointUser modUser2;
		printf("User ID : ");
		while(scanf("%d", &modUser2.userID)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter a Integer Value : ");
    		}
		printf("Account Number : ");
		while(scanf("%d", &modUser2.account_no)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter a Integer Value : ");
    		}
		printf("New Name of the Primary Account Holder : ");
		scanf(" %[^\n]",modUser2.name1);
		printf("New Name of the Other Account Holder : ");
		scanf(" %[^\n]",modUser2.name2);
    		char *pass=getpass("Password(max 10 characters): ");
    		memcpy(modUser2.password,pass,sizeof(pass));
		printf("New Balance : Rs.");
		while(scanf("%f", &modUser2.balance)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nEnter a Float Value : ");
    		}
		while(modUser2.balance<0){
			printf("(Account Balance cannot be less then 0)\n");
			printf("Please Enter Proper Value : \n");
			while(scanf("%f", &modUser2.balance)!=1 || getchar()!='\n'){
        		scanf("%*[^\n]%*c");
        		printf("\nEnter a Float Value : ");
    		}
		}
		write(sd,&modUser2,sizeof(jointUser));
	}

	read(sd,&result,sizeof(result)); 

	if(!result){
		printf("\n\n\t************************ Error Modifying the Account, Please re-check the User ID and Account No !! *******************\n\n");
	}
	else{
		printf("\n\n\t**************************!!! Succesfully Modified the Account !!!*****************************\n\n");
	}
	sleep(2);
	showMenu(sd);
	return;
}

void searchAccount(int sd){
	int todo=4;
	int type,len;
	bool result;

	int writeStatus=write(sd,&todo,sizeof(int));
	if(writeStatus==-1){
	perror("Error while sending data to server in searchAccount");
	_exit(1);
	}

	printf("\nEnter the type account(1: Normal Account 2: Joint Account) : ");
	while(scanf("%d", &type)!=1 || getchar()!='\n'){
        scanf("%*[^\n]%*c");
        printf("\nPlease Select Proper Account Type : ");
   	}
	while(type<1 || type>2){
		printf("\nPlease Select Proper Account Type : ");
		while(scanf("%d", &type)!=1 || getchar()!='\n'){
        	scanf("%*[^\n]%*c");
        	printf("\nPlease Select Proper Account Type : ");
    	}
	}

	writeStatus=write(sd,&type,sizeof(int));
	if(writeStatus==-1){
	perror("Error while sending type data to server in searchAccount");
	_exit(1);
	}

	if(type==1){
	normalUser searchUser1;
	int userID;
	printf("User ID : ");
	scanf("%d%*c",&userID);
	writeStatus=write(sd,&userID,sizeof(userID));
	if(writeStatus==-1){
	perror("Error while sending userInfo to server in searchAccount");
	_exit(1);
	}

	len=read(sd,&searchUser1,sizeof(normalUser));
	if(len==-1){
	perror("Error while reading userInfo from server in searchAccount");
	_exit(1);
	}
	else if(len==0 || searchUser1.userID==-1){
	printf("\n\n*******************************UserID not found !!!\tPlease re-check the User ID!!***************************************\n\n");
	}
	else{
	printf("User ID : %d\n",searchUser1.userID);
	printf("Name : %s\n",searchUser1.name);
	printf("Account Number : %d\n",searchUser1.account_no);
	printf("Available Balance : Rs.%0.2f\n",searchUser1.balance);
	printf("Status : %s\n\n",searchUser1.status);
	}
	}

	if(type==2){
	jointUser searchUser2;
	int userID1;
	printf("User ID : ");
	scanf("%d%*c",&userID1);
	writeStatus=write(sd,&userID1,sizeof(userID1));
	if(writeStatus==-1){
	perror("Error while sending userInfo to server in searchAccount");
	_exit(1);
	}

	len=read(sd,&searchUser2,sizeof(jointUser));
	if(len==-1){
	perror("Error while reading userInfo from server in searchAccount");
	_exit(1);
	}
	else if(len==0 || searchUser2.userID==-1){
	printf("UserID not found !!!\tPlease re-check the User ID!!\n\n");
	}
	else{
	printf("User ID : %d\n",searchUser2.userID);
	printf("Main Account Holder's Name : %s\n",searchUser2.name1);
	printf("Other Account Holder's Name : %s\n",searchUser2.name2);
	printf("Account Number : %d\n",searchUser2.account_no);
	printf("Available Balance : Rs.%0.2f\n",searchUser2.balance);
	printf("Status : %s\n\n",searchUser2.status);
	}
	}
	printf("Press Enter to go back to Menu\n");
	char c;
	while((c = getchar()) != '\n' && c != EOF);
	showMenu(sd);
	return;
}

void createAdmin(int sd){
	admin newAdmin;
	printf("Please enter the name of the admin: ");
	scanf(" %[^\n]",newAdmin.username);
	char *pass=getpass("Please enter the password(max 10 characters): ");
	memcpy(newAdmin.password,pass,sizeof(pass));
	int todo=5;
	int writeStatus=write(sd,&todo,sizeof(int));
	int type=0;
	writeStatus=write(sd,&type,sizeof(int));
	writeStatus=write(sd,&newAdmin,sizeof(newAdmin));
	if(writeStatus==-1){
		perror("Error while writing data to the record");
		exit(0);
	}
	int res;
	int r=read(sd,&res,sizeof(res));
	if(r==-1){
		perror("Error");
		exit(0);
	}
	printf("Your userID is : %d\n",res);
	printf("Press Enter to go back to Menu\n");
	sleep(1);
	getchar();
	showMenu(sd);
	
}

