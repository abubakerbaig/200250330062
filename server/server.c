#include <stdio.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<linux/in.h>

int sfd,cfd,check;
struct sockaddr_in saddr;
struct sockaddr_in caddr;
int len;
unsigned char buff[1024];

pthread_t temp_analysis, rotation_analysis, monitor;

void *Temp_Aanlysis(void *data) {
	while(1)	{

	}
}
void *Rotation_Analysis(void *data) {
	while(1)	{
		cfd = socket(AF_INET,SOCK_STREAM,0);

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	saddr.sin_port = htons(5678);

	bind(sfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));

	check=listen(sfd,10);
	if(check == -1)
		printf("error in listen\n");

	len = sizeof(struct sockaddr_in);
	cfd = accept(sfd,(struct sockaddr *)&caddr, &len);
	
	if(cfd == -1)
		printf("error while accept\n");

	while(1)	{
	read(cfd,buff,1024);
	if(buff[2]>16)	{
		
	}
	printf("client send:%s\n",buff);
	} 

	close(cfd);
	close(sfd);
	}

}
void *Monitor(void *data)   {
	
	
    
}

int main(int argc, char const *argv[])
{
    pthread_create(&temp_analysis, NULL, Temp_Analysis, NULL);
    pthread_create(&rotation_Analysis, NULL, Rotation_Analysis, NULL);
    pthread_create(&monitor, NULL, Monitor, NULL);

	return 0;
}