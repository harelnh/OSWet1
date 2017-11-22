#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
int pID_Fg;
char* L_Fg_Cmd;
job jobs[MAX_PROCESSES];

void SIGTSTP_handler(int sig_num);
void SIGINT_handler(int sig_num);

#endif

