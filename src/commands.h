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
#define HISTORY_SIZE 50
#define MAX_PROCESS_NAME_SIZE 50
#define MAX_PROCESSES_NUMBER 100

typedef enum { FALSE, TRUE } bool;
typedef struct JOB_ {
	char  process_name[MAX_PROCESS_NAME_SIZE];
	int process_pid;
	time_t start_running_time;
	bool is_running;
} job, *pjob;
int pID_Fg;
char* L_Fg_Cmd;

int ExeComp(char* lineSize, bool isbg, job job_list[MAX_PROCESSES_NUMBER]);
int BgCmd(char* lineSize, job job_list[MAX_PROCESSES_NUMBER]);
int ExeCmd(char* lineSize, char* cmdString, char* prevdir, char history[HISTORY_SIZE][MAX_LINE_SIZE], job job_list[MAX_PROCESSES_NUMBER]);
void ExeExternal(char *args[MAX_ARG], char* cmdString, bool isbg, job job_list[MAX_PROCESSES_NUMBER]);
void UpdateJobsList(job job_list[MAX_PROCESSES_NUMBER]);
bool InsertJobToJobList(char  process_name[MAX_PROCESS_NAME_SIZE], int process_pid, time_t start_running_time, bool is_running, job job_list[MAX_PROCESSES_NUMBER]);
bool DeleteJobFromJobList(int process_pid, job job_list[MAX_PROCESSES_NUMBER]);
#endif

