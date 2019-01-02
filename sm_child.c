/******************************************************************
/*
/* This is a test application that demonstrates the use of POSIX 
/* shared memory for IPC communication.
/* This application run by the sm_parent.c module as a child process
/*
/* 
/* NOTE:
/* Build this application before running the sm_parent.c app.
/*
/* To build : gcc -o smc sm_child.c
/*****************************************************************/

#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <sys/mman.h>

#define SIZE 4096

int main() 
{ 

   const char* name = "SMO"; // shared memory object name
   const char* sharedMessage = "This is the shared message"; //shared data
   int shmFd; // shared memory file descriptor 
   void* sharedMemoryPtr; // pointer to shared memory obect 

   sleep(1); //intentionally long wait for parent to write to shared memory

   shmFd = shm_open(name, O_RDONLY, 0666); // opend
   sharedMemoryPtr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shmFd, 0); /* memory map the shared memory object */
   printf("sm_child: %s", (char*)sharedMemoryPtr); // read from the shared memory object 
   shm_unlink(name); // remove the shared memory object 
	
   return 0; 
} 

