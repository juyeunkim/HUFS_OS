#include "mqheader.h"
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
	int qid,n,len,delete;
	int input,output;
	char buf[10];
	umsg mymsg;
	
    //message queue생성 호출
	if((qid = msgget(MSGQKEY,PERMISSION | IPC_CREAT))==-1){
		perror("msgget");
		exit(1);
	}

	
    // mtype must be >0
	mymsg.mtype=1;	

    //client process
	if(fork()==0){
		input = open("input.txt",0);
		if(input==-1)
			exit(1);
        
        //input.txt의 내용을 10byte씩 읽어옴
		while((n= read(input,buf,10))!=0){
            //buf에 내용을 mymsg.mtext로 copy
			strncpy(mymsg.mtext,buf,n);
            //message queue에 읽어온 파일을 send
			if(msgsnd(qid, &mymsg, n ,0)==-1){
				perror("msgsnd");
				exit(2);
			}

		}

		close(input);
        // child process 종료
		exit(0);
	}
    //server process
	else{
		output = creat("output.txt",PERMISSION);
	
		if(output==-1)
			error_exit("output");
			
        // sender에서 받은 내용을 ouput에 write
		while((n=msgrcv(qid, &mymsg,sizeof(umsg), 0,MSG_NOERROR)) !=-1){
			write(output,mymsg.mtext,n);
            
            // mtext의 마지막 부분을 write한 후, break;
			if( n >=0 && mymsg.mtext[n-1]=='\n') break;
		}

		
		close(output);
        // child process 를 기다린다
		wait(0);
	}
	
    // message queue의 통신 종료
	if((delete=msgctl(qid,IPC_RMID,0))==-1){
			perror("msgq remove failed");
			exit(1);
	}

	return 0;

}




