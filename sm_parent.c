/******************************************************************
/*
/* This is a test application that demonstrates the use of POSIX 
/* shared memory for IPC communication.
/* This application fork a new child process that reads the content
/* of the shared memory region.
/* 
/* NOTE:
/* Before running this app, make sure the sm_child.c app is built and
/* name "smc".  This application will execute it as the child process
/*
/* To build : gcc -o smp sm_parent.c
/*****************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <sys/mman.h>
#include <errno.h>
   
#define SIZE 4096

int main() 
{ 


   pid_t process2 = fork(); //launch a child process

   if(process2 == 0)//this is the child process
   {
	if(-1 == execv("./smc",NULL) )
	{
           int number = errno;
           printf("execl errno=[%d] \n", number);
           return;
	}
   }


   const char* name = "SMO"; // shared memory object name
   const char* sharedMessage = "This is the shared message"; //shared data
   int shmFd; // shared memory file descriptor 
   void* sharedMemoryPtr; // pointer to shared memory obect 
	
   shmFd = shm_open(name, O_CREAT | O_RDWR, 0666); // create the shared memory object 
   ftruncate(shmFd, SIZE); // configure the size of the shared memory object 
   sharedMemoryPtr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shmFd, 0); // memory map the shared memory object 
   sprintf(sharedMemoryPtr, "%s", sharedMessage); // write to the shared memory object 
   sharedMemoryPtr += strlen(sharedMessage); 

   return 0; 
} 

