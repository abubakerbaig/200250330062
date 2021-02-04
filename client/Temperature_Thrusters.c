#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/in.h>
#include <time.h>
#include "unique_id.h"

#define MIN_VALUE -100
#define MAX_VALUE 100

int random_gen(int ,int );


int cfd,sfd;
struct sockaddr_in saddr;

//structure init with Unique Program ID, Packet Number, data & timestamp
struct packet   {
    int Prgm_ID;
    int packet_num;
    int data;
    time_t time;
}temp_th_struct;

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
    temp_th_struct.time = time(&second);
    temp_th_struct.Prgm_ID = TEMP_TH; //1 for Acceleration
    temp_th_struct.packet_num = 1;
    temp_th_struct.data = 0;


    //printf("timestamp:%ld\ndata-%d\n", temp_th_struct.time, temp_th_struct.data);//check

	cfd = socket(AF_INET, SOCK_STREAM,0);

    if(cfd == -1)   {
        perror("socket open/temp_th_Rocket");
        return -1;  
    }

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(6604);

	sfd = connect(cfd,(struct sockaddr *)&saddr,sizeof(struct sockaddr_in)); //session create...
    if(sfd == -1)   {
        perror("Connect() open/temp_th_Rocket");
        return -1;  
    }

    while(1)    {

        temp_th_struct.data = random_gen(MIN_VALUE, MAX_VALUE);

        buffer[0] = temp_th_struct.Prgm_ID;
        buffer[1] = temp_th_struct.packet_num;
        buffer[2] = temp_th_struct.data;
        buffer[3] = temp_th_struct.time;

        write(cfd,&buffer,sizeof(buffer));
        temp_th_struct.packet_num++;
        sleep(1);
    }

	close(cfd);
    return 0;
}