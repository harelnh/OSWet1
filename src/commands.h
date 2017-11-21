#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_PROCESSES 100
typedef enum { FALSE , TRUE } bool;
typedef struct _Job{
	int pid;
	time_t startTime;
	char cmdStr[MAX_LINE_SIZE];
} Job;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, Job jobs[MAX_PROCESSES]);
int ExeCmd(void* jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
int insertNewJob(Job jobs[MAX_PROCESSES], int processID, char *lineStr);
int removeJob(int processID, Job jobs[MAX_PROCESSES]);
#endif

