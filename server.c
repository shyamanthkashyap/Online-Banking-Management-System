#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include<time.h>
#include<errno.h>

#include "Headers/StructuresDef.h"

bool checkNormalUser(normalUser currUser);
bool checkJointUser(jointUser currUser);
bool checkAdmin(admin currUser);


bool check(int j){
	if(j!=0){
		return true;
	}
	else{
		return false;
	}
}

void updatepassbook(normalUser currUser, int type, float amount)
{
	char buf[100],path[100];
	sprintf(path,"passbook/NormalUser/%d",currUser.userID);
	time_t t;
	time(&t);
	int len;
	if(type==0)
		len=sprintf(buf,"%s\t\tDeposit=%f\t\t\tBalance=%f\n",ctime(&t),amount,currUser.balance);
	else
		len=sprintf(buf,"%s\t\tWithdrew=%f\t\t\tBalance=%f\n",ctime(&t),amount,currUser.balance);
	int fd=open(path,O_CREAT|O_RDWR,0744);
	lseek(fd,0,SEEK_END);
	write(fd,buf,len);
	close(fd);
}

void updatepassbookJ(jointUser currUser, int type, float amount)
{
	char buf[100],path[100];
	sprintf(path,"passbook/JointUser/%d",currUser.userID);
	time_t t;
	time(&t);
	int len;
	if(type==0)
		len=sprintf(buf,"%s\t\tDeposit=%f\t\t\tBalance=%f\n",ctime(&t),amount,currUser.balance);
	else
		len=sprintf(buf,"%s\t\tWithdrew=%f\t\t\tBalance=%f\n",ctime(&t),amount,currUser.balance);
	int fd=open(path,O_CREAT|O_RDWR,0744);
	lseek(fd,0,SEEK_END);
	write(fd,buf,len);
	close(fd);
}

bool checkNormalUser(normalUser currUser){
	int i=currUser.userID-1001;
	if(i<0){
		return false;
	}
	int fd=open("Database/Normal_user",O_RDONLY,0744);
	bool result;
	normalUser temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	  
	lock.l_len=sizeof(normalUser);	           
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
	read(fd,&temp,sizeof(normalUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE")){
		result=true;
	}
	else{
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}

bool checkJointUser(jointUser currUser){
	int i=currUser.userID-1001;
	if(i<0){
		return false;
	}
	int fd=open("Database/Joint_user",O_RDONLY,0744);
	bool result;
	jointUser temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     
	lock.l_len=sizeof(jointUser);	             
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);	
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET); 
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE")){	
		result=true;
	}
	else{
		result=false;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkAdmin(admin currUser){
	int i=currUser.userID-1001;
	if(i<0){
		return false;
	}
	int fd=open("Database/Admin",O_RDONLY,0744);
	bool result;
	admin temp;
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);         
	lock.l_len=sizeof(admin);            
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);
	lseek(fd,(i)*sizeof(admin),SEEK_SET);  
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,currUser.password)){
	result=true;
	}
	else{
	result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}

normalUser getNormalUser(int ID){
	int i=ID-1001;
	normalUser currUser;
	if(i<0){
	printf("Invalid userID\n");
	currUser.userID=-1;
	return currUser;
	}
	int fd=open("Database/Normal_user",O_RDONLY,0744);
	if(fd==-1){
		printf("Invalid userID\n");
		currUser.userID=-1;
	}
	else{
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    	     
		lock.l_len=sizeof(normalUser);	                    
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		int offset=lseek(fd,-sizeof(struct normalUser),SEEK_END);  
		if(errno==EINVAL){
			printf("Invalid userID\n");
			currUser.userID=-1;
		}
		else{
			read(fd,&currUser,sizeof(normalUser));
			if((currUser.userID)<(i+1001)){
				printf("Invalid userID\n");
				currUser.userID=-1;
			}
			else{
			i = currUser.userID - ID+1;
			lseek(fd,-(i)*sizeof(normalUser), SEEK_END);
			read(fd,&currUser,sizeof(normalUser));
			}
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	close(fd);
	return currUser;
}



jointUser getJointUser(int ID){
	int i=ID-1001;
	jointUser currUser;
	if(i<0){
	printf("Invalid userID\n");
	currUser.userID=-1;
	return currUser;
	}
	int fd=open("Database/Joint_user",O_RDONLY,0744);
	if(fd==-1){
		printf("Invalid userID\n");
		currUser.userID=-1;
	}
	else{
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    	     
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);

		int offset=lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  
		if(errno==EINVAL){
			printf("Invalid UserID\n");
			currUser.userID=-1;
		}
		else{
			read(fd,&currUser,sizeof(jointUser));
			if(currUser.userID<(i+1001)){
				printf("Invalid UserID\n");
				currUser.userID=-1;
			}
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	close(fd);
	return currUser;
}

admin getAdmin(int ID){
	int i=ID-1001;
	admin currUser;
	int fd=open("Database/Admin",O_RDONLY,0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     
	lock.l_len=sizeof(admin);	           
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	

	lseek(fd,(i)*sizeof(admin),SEEK_SET); 
	read(fd,&currUser,sizeof(admin));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}


bool depositMoney(int accountType,int ID,float amount){
	int i=ID-1001;
	if(accountType==1){
		int fd=open("Database/Normal_user",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET); 
		read(fd,&currUser,sizeof(normalUser));
		if(!strcmp(currUser.status,"ACTIVE")){
			currUser.balance+=amount;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
			updatepassbook(currUser, 0, amount);
		}
		else{	
			result=false;
		}
		
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	else if(accountType==2){
		int fd=open("Database/Joint_user",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);   
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET); 
		read(fd,&currUser,sizeof(jointUser));
		if(!strcmp(currUser.status,"ACTIVE")){
			currUser.balance+=amount;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
			updatepassbookJ(currUser, 0, amount);
		}
		else{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	return false;
}

bool withdrawMoney(int accountType,int ID,float amount){
	int i=ID-1001;
	if(accountType==1){
		int fd=open("Database/Normal_user",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    
		lock.l_len=sizeof(normalUser);	         
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
		read(fd,&currUser,sizeof(normalUser));
		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amount){
			currUser.balance-=amount;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
			updatepassbook(currUser, 1, amount);
		}
		else{	
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	else if(accountType==2){
		int fd=open("Database/Joint_user",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	         
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET); 
		read(fd,&currUser,sizeof(jointUser));
		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amount){
			currUser.balance-=amount;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
			updatepassbookJ(currUser, 1, amount);
		}
		else{	
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	return false;
}

float getBalance(int accountType,int ID){
	int i=ID-1001;
	float result;
	if(accountType==1){
		int i=ID-1001;
		int fd=open("Database/Normal_user",O_RDONLY,0744);
		normalUser temp;
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    	     
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
		read(fd,&temp,sizeof(normalUser));
		if(!strcmp(temp.status,"ACTIVE")){	
			result=temp.balance;
		}
		else{
			result=0;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	else if(accountType==2){
		int i=ID-1001;
		int fd=open("Database/Joint_user",O_RDONLY,0744);
		jointUser temp;

		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    	    
		lock.l_len=sizeof(jointUser);	             
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	

		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  
		read(fd,&temp,sizeof(jointUser));
		if(!strcmp(temp.status,"ACTIVE")){
			result=temp.balance;
		}
		else{					
			result=0;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	return 0;
}

bool alterPassword(int accountType,int ID,char newPwd[10]){
	int i=ID-1001;
	if(accountType==1){
		int fd=open("Database/Normal_user",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
		read(fd,&currUser,sizeof(normalUser));

		if(!strcmp(currUser.status,"ACTIVE")){
			strcpy(currUser.password,newPwd);
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else{	
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	else if(accountType==2){
		int fd=open("Database/Joint_user",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();
		fl1=fcntl(fd,F_SETLKW,&lock);	
		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  
		read(fd,&currUser,sizeof(jointUser));

		if(!strcmp(currUser.status,"ACTIVE")){
			strcpy(currUser.password,newPwd);
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else{
			result=false;
		}
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	return false;
}

normalUser addNormalUser(normalUser record){
	int fd=open("Database/Normal_user",O_RDWR|O_CREAT,0744);
	//bool result;
	int size=lseek(fd,0,SEEK_END);
	if(size==0){
		record.userID=1001;
		record.account_no=100001;
		strcpy(record.status,"ACTIVE");
		write(fd,&record,sizeof(normalUser));
	}
	else{
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_END;
		lock.l_start=(-1)*sizeof(normalUser);    
		lock.l_len=sizeof(normalUser);	             
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	

		normalUser endUser;
		lseek(fd,(-1)*sizeof(normalUser),SEEK_END); 
		read(fd,&endUser,sizeof(normalUser));

		record.userID=endUser.userID+1;
		record.account_no=endUser.account_no+1;
		strcpy(record.status,"ACTIVE");

		int j=write(fd,&record,sizeof(normalUser));
		//result=check(j);
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}	
	close(fd);
	return record;
}

jointUser addJointUser(jointUser record){
	int fd=open("Database/Joint_user",O_RDWR|O_CREAT,0744);
	int size=lseek(fd,0,SEEK_END);
	if(size==0){
		record.userID=1001;
		record.account_no=100001;
		strcpy(record.status,"ACTIVE");
		write(fd,&record,sizeof(jointUser));
	}
	else{
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_END;
		lock.l_start=(-1)*sizeof(jointUser);    
		lock.l_len=sizeof(jointUser);	            
		lock.l_pid=getpid();

		fl1=fcntl(fd,F_SETLKW,&lock);	

		jointUser endUser;
		lseek(fd,(-1)*sizeof(jointUser),SEEK_END);  
		read(fd,&endUser,sizeof(jointUser));

		record.userID=endUser.userID+1;
		record.account_no=endUser.account_no+1;
		strcpy(record.status,"ACTIVE");

		int j=write(fd,&record,sizeof(jointUser));
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
	}
	close(fd);
	return record;
}

int deleteNormalUser(int ID){
	int i=ID-1001;
	int result;
	if(i<0){
		return 0;
	}
	int fd=open("Database/Normal_user",O_RDWR,0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser); 
	lock.l_len=sizeof(normalUser); 
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock); 
	normalUser currUser;
	lseek(fd,-sizeof(normalUser),SEEK_END); 
	read(fd,&currUser,sizeof(normalUser));
	if(currUser.userID<ID){
		result=0;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		fflush(stdout);
		fflush(stdin);
		close(fd);
		return result;
	}
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET); 
	read(fd,&currUser,sizeof(normalUser));
	if(currUser.balance>0){
		result=-1;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		fflush(stdout);
		fflush(stdin);
		close(fd);
		return result;
	}
	else if(!strcmp(currUser.status,"ACTIVE")){
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR);
		int j=write(fd,&currUser,sizeof(normalUser));
		result=check(j);
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	fflush(stdout);
	fflush(stdin);
	close(fd);
	return result;
}

int deleteJointUser(int ID){
	int i=ID-1001;
	int result;
	if(i<0){
		return 0;
	}
	int fd=open("Database/Joint_user",O_RDWR,0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser); 
	lock.l_len=sizeof(jointUser); 
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock); 
	jointUser currUser;
	lseek(fd,-sizeof(jointUser),SEEK_END); 
	read(fd,&currUser,sizeof(jointUser));
	if(currUser.userID<ID){
		result=0;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);
		close(fd);
		return result;
	}
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET); 
	read(fd,&currUser,sizeof(jointUser));
	if(currUser.balance>0){
		result=-1;
	}
	else if(!strcmp(currUser.status,"ACTIVE")){
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR);
		int j=write(fd,&currUser,sizeof(jointUser));
		result=check(j);
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}



bool modifyNormalUser(normalUser modUser){
	int i=modUser.userID-1001;
	int fd=open("Database/Normal_user",O_RDWR,0744);
	bool result=false;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    
	lock.l_len=sizeof(normalUser);	            
	lock.l_pid=getpid();
	fl1=fcntl(fd,F_SETLKW,&lock);	
	normalUser currUser;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  
	read(fd,&currUser,sizeof(normalUser));
	if(!strcmp(currUser.status,"ACTIVE") && (modUser.account_no==currUser.account_no)){
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR);
		int j=write(fd,&modUser,sizeof(normalUser));
		result=check(j);
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	close(fd);
	return result;
}

bool modifyJointUser(jointUser modUser){
	int i=modUser.userID-1001;
	int fd=open("Database/Joint_user",O_RDWR,0744);
	bool result=false;

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    
	lock.l_len=sizeof(jointUser);	            
	lock.l_pid=getpid();

	fl1=fcntl(fd,F_SETLKW,&lock);	

	jointUser currUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET); 
	read(fd,&currUser,sizeof(jointUser));

	if(!strcmp(currUser.status,"ACTIVE")  && (modUser.account_no==currUser.account_no)){
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR);
		int j=write(fd,&modUser,sizeof(jointUser));
		result=check(j);
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

#define mode 0744
#define flags O_RDWR | O_CREAT

int getNewUserID(char* user, char* path){
	int fd=open(path,flags,mode);
	int readStatus;
	if(!strcmp(user,"Admin")){
		admin record;
		lseek(fd,-sizeof(admin),SEEK_END);
		readStatus=read(fd,&record,sizeof(admin));
		if(readStatus==-1){
			perror("Error while reading from the record");
			_exit(1);
		}
		close(fd);
		return record.userID+1;
	}
}

int addAdmin(admin newAdmin){
	int fd=open("Database/Admin",O_RDWR,mode);
	if(fd==-1){
		perror("Error while opening file");
		exit(0);
	}
	int size=lseek(fd,0,SEEK_END);
	if(size==0){
		newAdmin.userID=1001;
	}
	else{
		newAdmin.userID=getNewUserID("Admin", "Database/Admin");
	}
	int writeStatus=write(fd,&newAdmin,sizeof(admin));
	if(writeStatus==-1){
		perror("Error while adding new admin");
		_exit(1);
	}
	close(fd);
	return newAdmin.userID;
}

admin initAdmin(){
	admin newAdmin;
	int fd=open("Database/Admin",O_RDWR|O_CREAT,mode);
	if(fd==-1){
		perror("Error while opening file");
		exit(0);
	}
	int size=lseek(fd,0,SEEK_END);
	if(size==0){
		newAdmin.userID=1001;
		strcpy(newAdmin.username,"admin");
		strcpy(newAdmin.password,"admin");
		int writeStatus=write(fd,&newAdmin,sizeof(admin));
		if(writeStatus==-1){
			perror("Error while adding new admin");
			_exit(1);
		}
	}
	else{
		newAdmin.userID=-1;
	}
	close(fd);
	return newAdmin;
}


void serverInstance(int newSocketDesc){
	int msgLength,todo,type,option,accountType,currUserID;
	bool result;
	while(1){
		read(newSocketDesc,&option,sizeof(option));
		printf("Option : %d\n",option);

		if(option==1){
			normalUser currUser1;
			accountType=1;
			msgLength=read(newSocketDesc,&currUser1,sizeof(normalUser));
			printf("Username  : %d is active now\n",currUser1.userID);
			currUserID=currUser1.userID;
			result=checkNormalUser(currUser1);
			write(newSocketDesc,&result,sizeof(result));
		}
		else if(option==2){
			jointUser currUser2;
			accountType=2;
			msgLength=read(newSocketDesc,&currUser2,sizeof(jointUser));
			currUserID=currUser2.userID;
			printf("Username : %d is active now\n",currUser2.userID);
			result=checkJointUser(currUser2);
			write(newSocketDesc,&result,sizeof(result));
		}
		else if(option==3){
			admin currUser3;
			accountType=3;
			msgLength=read(newSocketDesc,&currUser3,sizeof(admin));
			currUserID=currUser3.userID;
			printf("Username : %d is active now\n",currUser3.userID);
			result=checkAdmin(currUser3);
			write(newSocketDesc,&result,sizeof(result));
		}
		else{
			result=false;
			write(newSocketDesc,&result,sizeof(result));
		}
		if(result)	break;
	}

	while(1){
		read(newSocketDesc,&todo,sizeof(int));
		if(option==1 || option==2){
			if(todo==1){
				float amount;
				read(newSocketDesc,&amount,sizeof(float));
				result=depositMoney(accountType,currUserID,amount);
				write(newSocketDesc,&result,sizeof(result));
			}
			else if(todo==2){
				float amount;
				read(newSocketDesc,&amount,sizeof(float));
				result=withdrawMoney(accountType,currUserID,amount);
				write(newSocketDesc,&result,sizeof(result));
			}
			else if(todo==3){
				float amount;
				amount=getBalance(accountType,currUserID);
				write(newSocketDesc,&amount,sizeof(float));
			}
			else if(todo==4){
				char pwd[10];
				read(newSocketDesc,pwd,sizeof(pwd));
				result=alterPassword(accountType,currUserID,pwd);
				write(newSocketDesc,&result,sizeof(result));
			}
			else if(todo==5){
				if(option==1){
					normalUser user1=getNormalUser(currUserID);
					write(newSocketDesc,&user1,sizeof(normalUser));
					char path[100],buf[10000];
					sprintf(path,"passbook/NormalUser/%d",user1.userID);
					int fd=open(path,O_CREAT|O_RDONLY,0744);
					int len=read(fd,buf,10000);
					if(len==-1||len==0)
					{
					write(newSocketDesc,"empty",6);	
					}
					else
					{
					write(newSocketDesc,buf,len);
					}
					close(fd);
					}
					else if(option==2){
					jointUser user2=getJointUser(currUserID);
					write(newSocketDesc,&user2,sizeof(jointUser));
					char path[100],buf[10000];
					sprintf(path,"passbook/JointUser/%d",user2.userID);
					int fd=open(path,O_CREAT|O_RDONLY,0744);
					int len=read(fd,buf,10000);
					if(len==-1||len==0)
					{
					write(newSocketDesc,"empty",6);	
					}
					else
					{
					write(newSocketDesc,buf,len);
					}
					close(fd);
					
					}
			}
			else if(todo==6)	break;
		}
		else if(option==3){
			read(newSocketDesc,&type,sizeof(int));
			if(todo==1){
				if(type==1){
					normalUser newUser1;
					read(newSocketDesc,&newUser1,sizeof(normalUser));
					newUser1=addNormalUser(newUser1);
					write(newSocketDesc,&newUser1,sizeof(newUser1));
				}
				else if(type==2){
					jointUser newUser2;
					read(newSocketDesc,&newUser2,sizeof(jointUser));
					newUser2=addJointUser(newUser2);
					write(newSocketDesc,&newUser2,sizeof(newUser2));
				}
			}
			else if(todo==2){
				if(type==1){
					int delUserID1;
					read(newSocketDesc,&delUserID1,sizeof(int));
					int res=deleteNormalUser(delUserID1);
					write(newSocketDesc,&res,sizeof(res));
				}
				else if(type==2){
					int delUserID2;
					read(newSocketDesc,&delUserID2,sizeof(int));
					int res=deleteJointUser(delUserID2);
					write(newSocketDesc,&res,sizeof(res));
				}
			}
			else if(todo==3){
				if(type==1){
					normalUser modUser1;
					read(newSocketDesc,&modUser1,sizeof(normalUser));
					result=modifyNormalUser(modUser1);
					write(newSocketDesc,&result,sizeof(result));
				}
				else if(type==2){
					jointUser modUser2;
					read(newSocketDesc,&modUser2,sizeof(jointUser));
					result=modifyJointUser(modUser2);
					write(newSocketDesc,&result,sizeof(result));
				}
			}
			else if(todo==4){
				if(type==1){
					normalUser searchUser1;
					int userID1;
					read(newSocketDesc,&userID1,sizeof(int));
					searchUser1=getNormalUser(userID1);
					write(newSocketDesc,&searchUser1,sizeof(normalUser));
				}
				else if(type==2){
					jointUser searchUser2;
					int userID2;
					read(newSocketDesc,&userID2,sizeof(int));
					searchUser2=getJointUser(userID2);
					write(newSocketDesc,&searchUser2,sizeof(jointUser));
				}
			}
			else if(todo==5){
				admin admin1;
				int rs=read(newSocketDesc,&admin1,sizeof(admin));
				if(rs==-1){
					perror("read failed");
					exit(0);
				}
				int adminId=addAdmin(admin1);
				write(newSocketDesc,&adminId,sizeof(adminId));
			}
			else if(todo==6)	break;
		}
	}
	close(newSocketDesc);
	printf("Ended client session.....\n");
	return;
}

int main(){
	admin adm=initAdmin();
	
	if(adm.userID==-1){
	printf("Credentials Hidden ! Ask Admin.\n");
	}
	else{
	printf("Crendentials\n");
	printf("User ID   : %d\n",adm.userID);
	printf("Password  : %s\n",adm.password);
	}
	struct sockaddr_in server,client;
	int socketDesc,newSocketDesc,clientLen;
	bool result;
	socketDesc=socket(AF_INET,SOCK_STREAM,0);

	server.sin_family=AF_INET;
	server.sin_addr.s_addr=INADDR_ANY;
	server.sin_port=htons(8080);

	bind(socketDesc,(struct sockaddr *)&server,sizeof(server));
	listen(socketDesc,15);

	printf("Waiting for the client.....\n");
	while(1){
		clientLen=sizeof(client);
		newSocketDesc=accept(socketDesc,(struct sockaddr *)&client,&clientLen);

		printf("Connected to the client.....\n");
		if(!fork()){
			close(socketDesc);	
			serverInstance(newSocketDesc);
			exit(0);
		}
		else{
			close(newSocketDesc);	
		}
	}
	return 0;
}
