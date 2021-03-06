#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/in.h>
#include <time.h>
#include <semaphore.h>
#include <fcntl.h>
#include "unique_id.h"

#define MIN_VALUE 2
#define MAX_VALUE 20

int random_gen(int ,int );


int cfd,sfd;
struct sockaddr_in saddr;

//structure init with Unique Program ID, Packet Number, data & timestamp
struct packet   {
    int Prgm_ID;
    int packet_num;
    int data;
    time_t time;
}acc_struct;

long int buffer[4];

int random_gen(int min_num, int max_num)    {
    {
        int result = 0, low_num = 2, hi_num = 20;

        if (min_num < max_num)
        {
            low_num = min_num;
            hi_num = max_num + 1; // include max_num in output
        } else {
            low_num = max_num + 1; // include max_num in output
            hi_num = min_num;
        }

        srand(time(NULL));
        result = (rand() % (hi_num - low_num)) + low_num;
        return result;
    }
    
}

int main(int argc, char const *argv[])  {
    //def init 
    time_t second;
    acc_struct.time = time(&second);
    acc_struct.Prgm_ID = ACC_RC; //1 for Acceleration
    acc_struct.packet_num = 1;
    acc_struct.data = 0;

  //  printf("timestamp:%ld\ndata-%d\n", acc_struct.time, acc_struct.data);//check

//sem init

    sem_t *sem_rocket = sem_open(SEM_NAMW1, 1);
    if(sem_rocket == SEM_FAILED)    {
        perror("Temp_control_system/sem_open(creating)");
        exit(EXIT_FAILURE);
    }

	cfd = socket(AF_INET, SOCK_STREAM,0);

    if(cfd < 0)   {
        perror("socket open/Acceleration_Rocket");
        return -1;  
    }

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(6666);
    sfd = inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr); //convert ipv4 and ipv6 from text to binary form
    if(sfd < 0) {
        perror("inet_pton() open/Acceleration_Rocket");
        return -1; 
    }
	sfd = connect(cfd,(struct sockaddr *)&saddr,sizeof(saddr)); //session create...
    if(sfd < 0)   {
        perror("Connect() open/Acceleration_Rocket");
        return -1;  
    }
    while(1)    {
        acc_struct.data = random_gen(MIN_VALUE, MAX_VALUE);
        
        buffer[0] = acc_struct.Prgm_ID;
        buffer[1] = acc_struct.packet_num;
        buffer[2] = acc_struct.data;
        buffer[3] = acc_struct.time;

        sem_wait(sem_rocket);
        write(cfd,&buffer,sizeof(buffer));
        sem_post(sem_rocket);        
        acc_struct.packet_num++;
        sleep(1);
    }

	close(cfd);
    return 0;
}