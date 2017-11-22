// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
Synopsis: handle the Control-C */
#include "signals.h"

//**************************************************************************************
// function name: SIGTSTP_act
// Description: send SIGTSTP signal to the process in the foreground, and insert the process to job list
// Parameters: the signal number
// Returns: None
//**************************************************************************************
//TODO change docs
void SIGTSTP_act(int sig_num)
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
		InsertJob(L_Fg_Cmd, pID_Fg, time(NULL), FALSE, job_list);
		pID_Fg = -1;
		strcpy(L_Fg_Cmd, "\0");
	}
}

//**************************************************************************************
// function name: SIGINT_act
// Description: send SIGINT signal to the process in the foreground
// Parameters: the signal number
// Returns: None
//**************************************************************************************
//TODO change docs
void SIGINT_act(int sig_num)
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

