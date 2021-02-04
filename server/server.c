#include <stdio.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/in.h>

int sfd, cfd, check;
struct sockaddr_in saddr;
struct sockaddr_in caddr;
int len;
sem_t sem1,sem2;
//buffers to store received data
long int buff_temp_cs[4];
long int buff_temp_th[4];
long int buff_acc[4];
long int buff_gyro[4];


pthread_t temp_analysis, rotation_analysis, monitor;

void *Temp_Aanlysis(void *data)
{
	while (1)
	{
	}
}
void *Rotation_Analysis(void *data)
{
	while (1)
	{
		cfd = socket(AF_INET, SOCK_STREAM, 0);

		saddr.sin_family = AF_INET;
		saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
		saddr.sin_port = htons(6604);

		bind(sfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));

		check = listen(sfd, 10);
		if (check == -1)
			printf("error in listen\n");

		len = sizeof(struct sockaddr_in);
		cfd = accept(sfd, (struct sockaddr *)&caddr, &len);

		if (cfd == -1)
			printf("error while accept\n");

		while (1)
		{
			read(cfd, buff, 1024);
			if (buff[2] > 16)
			{
				sem_post(&sem1);
				sleep(1);
				sem_wait(&sem2);
			}
		}

		close(cfd);
		close(sfd);
	}
}
void *Monitor(void *data)
{

	while (1)
	{
		//signaling to display when threshold is crossed from above two threads
		sem_wait(&sem1);
		if(buff_acc)
		printf("Rocket is on fire.\n");

		sem_post(&sem1);

	}
}

int main(int argc, char const *argv[])
{
	pthread_create(&temp_analysis, NULL, Temp_Analysis, NULL);
	pthread_create(&rotation_Analysis, NULL, Rotation_Analysis, NULL);
	pthread_create(&monitor, NULL, Monitor, NULL);

	return 0;
}