
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> // read
#include <fcntl.h> // open
#include <ctype.h> // tolower
void* p1 (void *arg);
void* p2 (void *arg);
void* p3 (void *arg);

char buf1[100];
char buf2[100];
int c1 = 0, c2 = 0; //shared variable
int rear = -1, front = -1; // buf1
int in = -1, out = -1; // buf2
int flag = -1, flag2 = -1; // while을 빠져나가는 조건

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_space2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data2 = PTHREAD_COND_INITIALIZER;

int main(void){
	int i;

	pthread_t threads[3];
	
	pthread_create(&threads[0],NULL,p1,NULL);
	pthread_create(&threads[1],NULL,p2,NULL);
	pthread_create(&threads[2],NULL,p3,NULL);


	for(i=0;i<3;i++)
		pthread_join(threads[i],NULL);

	return 0;
	
}


void* p1(void *arg){
	int n, f, input;
	char s[2]={0,};


	input = open("input.txt", 0);
	if(input ==-1)
		exit(1);

	while((n = read(input,s,sizeof(char)))!=0 ){ // EOF
		pthread_mutex_lock(&mutex1);
		if(c1 == 100){ 
		pthread_cond_wait(&buffer_has_space1,&mutex1);}

		rear++; rear%=100; 
		buf1[rear] = s[0]; // file에서 읽어온 1byte를 buf1에 저장
		c1++;

		pthread_cond_signal(&buffer_has_data1);
		pthread_mutex_unlock(&mutex1);
			
	}
	flag = c1; // file을 다 읽어, while문을 빠져나온 후 c1
	close(input);	
}

void* p2(void *arg){
	int i;
	char s;
	while(flag != 0){
		// mutex1			
		pthread_mutex_lock(&mutex1);
		if(c1 == 0){ 
            pthread_cond_wait(&buffer_has_data1,&mutex1);}

		front++; front%=100;
		s = buf1[front]; // buf1의 내용을 가져옴
        s = tolower(s); // 대문자 -> 소문자
		c1--;
		if(flag != -1) flag--; // flag의 값 감소
		
		pthread_cond_signal(&buffer_has_space1);
		
		pthread_mutex_unlock(&mutex1);

        //mutex2
		pthread_mutex_lock(&mutex2);
		
        if(c2 == 100){
            pthread_cond_wait(&buffer_has_space2,&mutex2);}
		in++; in%=100;
		buf2[in] = s; // 소문자의 내용을 buf2에 저장
		c2++;
		
		pthread_cond_signal(&buffer_has_data2);

		pthread_mutex_unlock(&mutex2);

	}
	flag2 = c2; // buf1의 내용을 다 읽고, while문을 빠져나온 후 c2
}

void* p3(void *arg){
	int i;
	char s;
	while(flag2 != 0){
		pthread_mutex_lock(&mutex2);
		if(c2 == 0) {
            pthread_cond_wait(&buffer_has_data2,&mutex2);}

		out++; out%=100;
		s = buf2[out]; // buf2 내용 추출
		c2--;
		if(flag2 !=-1) flag2 --; // flag의 값 감소
		
		pthread_cond_signal(&buffer_has_space2);
		
		pthread_mutex_unlock(&mutex2);
		
		printf("%c",s); // buf2의 내용 출력
		
	}
	
}
