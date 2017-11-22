// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
Synopsis: handle the Control-C */
#include "signals.h"

//**************************************************************************************
// function name: SIGINT_handler
// Description: send SIGINT signal to a foreground process.
// Parameters: signal number
// Returns: Nada :-)
//**************************************************************************************
void SIGINT_handler(int sig_num)
{
	if(pID_Fg!=-1)
	{
		if(kill(pID_Fg, SIGINT)==0)
		{
			printf("smash > signal SIGINT was sent to pid %d\n", pID_Fg);
		}
		else
		{
			printf("smash error: > cannot send signal\n");
		}
		pID_Fg = -1;
		strcpy(L_Fg_Cmd, "\0");
	}
}

//**************************************************************************************
// function name: SIGTSTP_handler
// Description: send SIGTSTP signal to a foreground process, and insert it to the job list
// Parameters: signal number
// Returns: Nada
//**************************************************************************************
void SIGTSTP_handler(int sig_num)
{
	if(pID_Fg!=-1)
	{
		if(kill(pID_Fg, SIGTSTP)==0)
		{
			printf("smash > signal SIGTSTP was sent to pid %d\n", pID_Fg);
		}
		else
		{
			printf("smash error: > cannot send signal\n");
		}
		InsertJob(L_Fg_Cmd, pID_Fg, time(NULL), FALSE, jobs);
		pID_Fg = -1;
		strcpy(L_Fg_Cmd, "\0");
	}
}



