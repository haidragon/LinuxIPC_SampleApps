#Makefile to build all the linuxIPC sample apps
binaries = p1 mq1 mq2 smp smc

all: clean $(binaries)

mq1: mqprocess1.c
	gcc -o mq1 mqprocess1.c -lrt

mq2: mqprocess2.c
	gcc -o mq2 mqprocess2.c -lrt

p1: pipe_example.c
	gcc -o p1 pipe_example.c -lrt

smp: sm_parent.c
	gcc -o smp sm_parent.c -lrt

smc: sm_child.c
	gcc -o smc sm_child.c -lrt

.PHONY : clean

clean:
	rm -f $(binaries) 
