/******************************************************************
/*
/* This is a test application that demonstrates the use of POSIX 
/* message queues for IPC communication.
/* This application forks a new child process that sends and
/* receives messages from the same queue
/* 
/* NOTE:
/* Before running this app, make sure the mqprocess2.c app is built and
/* name "mq2".  This application will execute it as the child process
/*
/* To build : gcc -o mq1 mqprocess1.c
/*****************************************************************/

#define ITERATIONS	10
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mqueue.h>
#include <errno.h>
#include <string.h>

#define ONE_THIRD_SEC   333000
#define MQ_NAME "/mqtest"

static pthread_t p2Thread;  


int main(int argc, char *argv[])
{
   int count = 0;
   int send_status = -1;
   int bytes_read = 0;
   mqd_t mq = -1; 		//message queue descriptor


   //launch a child process
   pid_t process2 = fork();

   if(process2 == 0)
   {//this is the child process
	if(-1 == execv("./mq2",NULL) )
	{
           int number = errno;
           printf("execl errno=[%d] \n", number);
           return;
	}
   }

   // initialize MQ attribute struct
   struct mq_attr attr;
   char buffer[512];

   // initialize attribute struct
   attr.mq_flags = 0;
   attr.mq_maxmsg = 10;
   attr.mq_msgsize=512; 
   attr.mq_curmsgs = 0;
	
   mq = mq_open(MQ_NAME, O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);
   if(-1 == mq) 
   {
	  printf("mq1 : Error opening msg queue\n");
	  exit(1);
   }

   usleep(ONE_THIRD_SEC);

   //receive message packets
   while(count < ITERATIONS)
   {

       bytes_read =  mq_receive(mq,buffer,512,NULL);

       if (0 < bytes_read)
       {
	       printf("Process1 Received [%s]\n",buffer);
       }
       else
	       printf("Byte count %d\n",bytes_read);
           usleep(ONE_THIRD_SEC);
	       count++;
   }

   count = 0;
   strcpy(buffer,"data from process1");
   while(count < ITERATIONS)
   {
	  send_status = mq_send(mq,(const char *)&buffer,strlen(buffer) + 1,0);
	  printf("Sending... [%s]  [%d] \n",buffer, send_status);
	  usleep(ONE_THIRD_SEC);
	  count++;
   }

  mq_close(mq);  // close mq
  mq_unlink(MQ_NAME);  //delete mq
  return;
}







