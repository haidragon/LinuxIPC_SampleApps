/* Process 2 of a message queue example.  This should be built before running mqprocess1 */
/* gcc -o mq2 mqprocess2.c -lrt */

#define ITERATIONS	10
#define ONE_THIRD_SEC   333000

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include  <mqueue.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
   int count = 0; int send_status = -1;
   mqd_t mq = -1;
   char buffer[512];
   int bytes_read = 0;

   mq = mq_open("/mqtest",O_RDWR);
   if(-1 == mq) 
   {
	printf("mq2 : Error opening msg queue\n");
	exit(1);
   }


   strcpy(buffer,"data from process2");
   while(count < ITERATIONS)
   {
	send_status = mq_send(mq,(const char *)&buffer,strlen(buffer)+1,0);
	printf("Sending... [%s]  [%d] \n",buffer, send_status);
	usleep(ONE_THIRD_SEC);
	count++;
   }

   printf("-----------------------------------\n");
   sleep(1); //wait before receiving

   //receive message packets
   count = 0;
   while(count < ITERATIONS)
   {
       bytes_read =  mq_receive(mq,buffer,512,NULL);

       if (0 < bytes_read)
       {
    	   printf("Process2 Received [%s]\n",buffer);
       }
       else
    	   printf("Byte count %d\n",bytes_read);
       usleep(ONE_THIRD_SEC);
       count++;
   }

   mq_close(mq);
   mq_unlink("/mqtest");
}

