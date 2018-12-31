/******************************************************************
/*
/* This is a test application that demonstrates the use of pipes for
/* bi-directional IPC communication.
/* First pipe is used to send data from parent to child
/* Second pipe is to receive confirmation from child to parent.
/*
/* To build : gcc -o p1 pipe_example.c
/*****************************************************************/

#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 
  	int counter=0;
	int fileDescriptor1[2]; // [0] = read-end of pipe [1] = write end of pipe 
	int fileDescriptor2[2]; //  
	char inputString[50]; 
	pid_t processId; 

	if (-1 == pipe(fileDescriptor1) ) //create 1st pipe to write parent to child communication
	{ 
		fprintf(stderr, "Pipe 1  Failed\n" ); 
		return 1; 
	} 

	if (-1 == pipe(fileDescriptor2) ) //create 2nd pipe to read from child to parent
	{ 
		fprintf(stderr, "Pipe2 Failed \n" ); 
		return 1; 
	}
 
	processId = fork(); // creates a child process by duplicating the calling process

	if (processId < 0) //failure; no process child created
	{ 
		fprintf(stderr, "Fork Failed. Exiting...." ); 
		return 1; 
	} 
	else if (processId > 0) //This is the Parent Process. processId of child is returned to parent 
	{ 
		char str[100]; 

		close(fileDescriptor1[0]); // Close reading end of first pipe, since this pipe is to write to child 

		//read string from user, and send to child via the pipe
		for(counter=0; counter < 10; counter++)
		{
		  printf("parent process requests a string:");
		  scanf("%s", inputString); 
		  write(fileDescriptor1[1], inputString, strlen(inputString)+1); 
		  usleep(500000); //wait for child to send response
		  read(fileDescriptor2[0], str, 100); 
		  printf("Parent Pipe Reads : %s\n", str); 
		}

		close(fileDescriptor1[1]); 

		// Wait for child to send a string 
		wait(NULL); 

		close(fileDescriptor2[1]); // Close writing end of second pipe 

		// Read string from child, print it and close 
		// reading end. 
		close(fileDescriptor2[0]); 
	} 
	else // child process since processId == 0
	{ 
		close(fileDescriptor1[1]); // Close writing end of first pipe 
		// Read a string using first pipe and confirm receipt by sendin message on 2nd pipe
		char str[50]; 
		
		while(counter < 9)
		{
			char confirmReceipt[100] = "Received:";
			read(fileDescriptor1[0], str, 100); //read from parent on pipe 1
			//printf("p2: %s\n",str);
			counter++;
			strcat(confirmReceipt,str);
			write(fileDescriptor2[1],confirmReceipt,strlen(confirmReceipt)+1); //write confirmation to parent on pipe 2
		}

		// Close both reading ends 
		close(fileDescriptor1[0]); 
		close(fileDescriptor2[1]); 
		close(fileDescriptor2[0]); // Close reading end of 2nd pipe

		exit(0); 
	} 
} 

