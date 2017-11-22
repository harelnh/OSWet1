/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
#define MAXARGS 20
#define HISTORY_SIZE 50
#define MAX_PROCESSES_NUMBER 100



extern char* L_Fg_Cmd;
extern int pID_Fg;
extern job job_list[MAX_PROCESSES];

//void* jobs = NULL; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
char lineSize[MAX_LINE_SIZE];
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char *argv[])
{
	pID_Fg = -1;
	char cmdString[MAX_LINE_SIZE];
	char prevdir[MAX_LINE_SIZE];
	char history[HISTORY_SIZE][MAX_LINE_SIZE];


	signal(SIGINT, &SIGINT_act);
	signal(SIGTSTP, &SIGTSTP_act);

	strcpy(prevdir, "\0"); //initiallize prevdir
 int i;
	for ( i = 0; i<HISTORY_SIZE; i++) //initiallize history
	{
		strcpy(history[i], "\0");
	}

	for ( i = 0; i<MAX_PROCESSES; i++) //initiallize jobs list
	{
		strcpy(job_list[i].name, "\0");
		job_list[i].pid = -1;
		job_list[i].start_time = -1;
		job_list[i].is_running = FALSE;
	}
	pID_Fg = -1;
	//signal declaretions
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	/* add your code here */

	/************************************/
	//NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
	//set your signal handlers here
	/* add your code here */

	/************************************/

	/************************************/
	// Init globals 



	L_Fg_Cmd = (char*)malloc(sizeof(char)*(MAX_LINE_SIZE + 1));
	if (L_Fg_Cmd == NULL)
		exit(-1);
	L_Fg_Cmd[0] = '\0';

	while (1)
	{
		printf("smash > ");
		fgets(lineSize, MAX_LINE_SIZE, stdin);
		strcpy(cmdString, lineSize);
		int i;
		
		
		cmdString[strlen(lineSize) - 1] = '\0';
		// perform a complicated Command
		if (!ExeComp(lineSize, FALSE, job_list))
		{
			for ( i = HISTORY_SIZE - 1; i>0; i--)
			{
				strcpy(history[i], history[i - 1]);
			}
			strcpy(history[0], cmdString);
			continue;
		}
		// background command
		if (!BgCmd(lineSize, job_list))
		{
			for ( i = HISTORY_SIZE - 1; i>0; i--)
			{
				strcpy(history[i], history[i - 1]);
			}
			strcpy(history[0], cmdString);
			continue;
		}
		 
		// built in commands
		ExeCmd(lineSize, cmdString, prevdir, history, job_list);
		//Enter command to history
		for ( i = HISTORY_SIZE - 1; i>0; i--)
		{
			strcpy(history[i], history[i - 1]);
		}
			strcpy(history[0], cmdString);
			continue;

		/* initialize for next line read*/
		lineSize[0] = '\0';
		cmdString[0] = '\0';
	}
	return 0;
}
