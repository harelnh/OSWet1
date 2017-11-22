//		commands.c
//********************************************
#include "commands.h"

//**************Functions Declarations***************
int cmdCd(char* args[MAX_ARG],char* prevdir,char pwd[MAX_LINE_SIZE],char* curpwdcheck);

int cmdPwd(char pwd[MAX_LINE_SIZE]) ;

int cmdHistory(char history[HISTORY_SIZE]);

int cmdJobs(job job_list[MAX_PROCESSES]);

int cmdKill(char* args[MAX_ARG],job job_list[MAX_PROCESSES]);

int cmdFg(char* args[MAX_ARG],job job_list[MAX_PROCESSES],int num_arg,int last_stopped_process);

int cmdBg(char* args[MAX_ARG],job job_list[MAX_PROCESSES],int num_arg,int last_stopped_process);

void cmdQuit(job job_list[MAX_PROCESSES]);

int getLastSuspendJob(job job_list[MAX_PROCESSES]);
//***************************************************
//**************************************************************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string, previous directory, hoistory
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(char* lineSize, char* cmdString, char* prevdir, char history[HISTORY_SIZE][MAX_LINE_SIZE], job job_list[MAX_PROCESSES])
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* curpwdcheck = getcwd(pwd, MAX_LINE_SIZE);
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
	args[0] = cmd;
	for (i = 1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
	/*************************************************/
	// Built in Commands PLEASE NOTE NOT ALL REQUIRED
	// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
	// MORE IF STATEMENTS AS REQUIRED
	/*************************************************/
	if (!strcmp(cmd, "cd"))
	{
		if (num_arg != 1)
			illegal_cmd = TRUE;
		else
		{
			return cmdCd(args,prevdir,pwd,curpwdcheck);
		}

	}

		/*************************************************/
	else if (!strcmp(cmd, "pwd"))
	{
		if (num_arg)
			illegal_cmd = TRUE;
		else
		{
			return cmdPwd(pwd);
		}
	}

		/*************************************************/
	else if (!strcmp(cmd, "history"))
	{
		if (num_arg)
			illegal_cmd = TRUE;
		else
		{
			return cmdHistory(history);
		}
	}
		/*************************************************/
	else if (!strcmp(cmd, "jobs"))
	{
		if (num_arg)
			illegal_cmd = TRUE;
		else
		{
			return cmdJobs(job_list);
		}
	}
		/*************************************************/
	else if (!strcmp(cmd, "kill"))
	{
		if (num_arg != 2)
			illegal_cmd = TRUE;
		else
		{
			return cmdKill(args,job_list);
		}
	}
		/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
		if (num_arg)
			illegal_cmd = TRUE;
		else
		{
			printf("smash pid is %d \n", getpid());
			return 0;
		}
	}
		/*************************************************/
	else if (!strcmp(cmd, "fg"))
	{
		if (num_arg != 0 && num_arg != 1){
			illegal_cmd = TRUE;
		}
		else {
			UpdateJobsList(job_list);
			int last_stopped_process = getLastSuspendJob(job_list);
			return cmdFg(args,job_list,num_arg,last_stopped_process);
		}

	}
		/*************************************************/
	else if (!strcmp(cmd, "bg"))
	{
		if(num_arg!=0 && num_arg!=1) {
			illegal_cmd=TRUE;
		}
		else {
			UpdateJobsList(job_list);
			int last_stopped_process = getLastSuspendJob(job_list);
			return cmdBg(args,job_list,num_arg,last_stopped_process);
		}
	}
		/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
		if(num_arg!=0 && num_arg!=1)
		{
			illegal_cmd=TRUE;
		}
		else if(num_arg==0)
		{
			exit(0); //TODO not sure if we allowed to use exit(0)
		}
		else if(num_arg==1)
		{
			if(strcmp("kill",args[1]) != 0 )
				illegal_cmd = TRUE;
			else
			{
				cmdQuit(job_list);
			}
		}
	}
		/*************************************************/
	else // external command
	{
		ExeExternal(args, cmdString, FALSE, job_list);
		return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
	return 0;
}

int getLastSuspendJob(job job_list[MAX_PROCESSES]) {
	for (int i = MAX_PROCESSES - 1; i >= 0; i--)  // looking for the last suspended job
	{
		if (!job_list[i].is_running && job_list[i].pid!=-1)
		{
			return i;
		}
	}
	return -1;
}
//******* built-in commands implementaions *******//
int cmdPwd(char pwd[MAX_LINE_SIZE]) {
	char* pwdcheck;
	pwdcheck = getcwd(pwd, MAX_LINE_SIZE);
	if (pwdcheck == NULL)
	{
		perror("pwd failed :-(\n");
		return 1;
	}
	printf("%s\n", pwd);
	return 0;
}

int cmdCd(char* args[MAX_ARG],char* prevdir,char pwd[MAX_LINE_SIZE],char* curpwdcheck) {
	if (strcmp("-", args[1]) == 0) // change to previous dir
	{
		if (strcmp("\0", prevdir) == 0)
			printf("there is no previous dir \n");
		else
		{
			int check = chdir(prevdir);
			if (check != -1)
			{
				printf("%s \n", prevdir);
				strcpy(prevdir, pwd);
				return 0;
			}
			else
			{
				perror("failed to cd \n");
				return 1;
			}
		}
	}
	else
	{
		// if first argument is a specific path
		if (curpwdcheck != NULL)
		{
			int check = chdir(args[1]);
			if (check != -1)
			{
				strcpy(prevdir, pwd);
				return 0;
			}
			else
			{
				printf("smash error: > \"%s\" - path not found\n", args[1]);
				return 1;
			}
		}
		else
		{
			perror("failed executing cd \n");
			return 1;
		}
	}
}

int cmdHistory(char history[HISTORY_SIZE]){
	for (int i = HISTORY_SIZE - 1; i >= 0; i--)
	{
		if (strcmp(history[i], "\0") != 0)
			printf("%s \n", history[i]);
	}
	return 0;
}

int cmdJobs(job job_list[MAX_PROCESSES]) {
	UpdateJobsList(job_list);
	int time_in_jobs;
	for (int i = 0; i<MAX_PROCESSES; i++)
	{
		time_t current_time = time(NULL);
		char* status = (job_list[i].is_running) ? "" : "(Stopped)";
		if (strcmp(job_list[i].name, "\0") != 0)
		{
			time_in_jobs= current_time - job_list[i].start_time;
			printf("[%d] %s : %d %d secs %s\n", i + 1, job_list[i].name, job_list[i].pid, time_in_jobs, status);
		}
	}
	return 0;
}

int cmdKill(char* args[MAX_ARG],job job_list[MAX_PROCESSES]){
	UpdateJobsList(job_list);
	// check if the job is valid
	int job_number = atoi(args[2]);
	if (job_number == 0)
	{
		printf("smash error: >  kill %s – job does not exist \n", args[2]);
		return 1;
	}
	else if (job_number>MAX_PROCESSES || job_number<1)
	{
		printf("smash error: >  kill %d – job does not exist \n", job_number);
		return 1;
	}
	if (strcmp(job_list[job_number - 1].name, "\0") == 0)
	{
		printf("smash error: >  kill %d – job does not exist  \n", job_number);
		return 1;
	}
	// check if the signal is valid
	int signal_number = atoi(strtok(args[1], "-"));
	if (signal_number == 0)
	{
		printf("smash error: >  kill %d – cannot send signal  \n", job_number);
		return 1;
	}
	if (kill(job_list[job_number - 1].pid, signal_number) != 0)
	{
		printf("smash error: >  kill %d – cannot send signal  \n", job_number);
		return 1;
	}
	else
	{
		printf("smash > signal %d was sent to pid %d\n", signal_number, job_list[job_number-1].pid);
		if(signal_number == SIGTSTP)
			job_list[job_number-1].is_running= FALSE;
		if(signal_number == SIGCONT)
			job_list[job_number-1].is_running= TRUE;
		return 0;
	}
}

int cmdFg(char* args[MAX_ARG],job job_list[MAX_PROCESSES],int num_arg,int last_stopped_process) {
	if (num_arg == 1) // fg to specific job
	{
		int job_number = atoi(args[1]);
		if (job_number == 0 || job_number<1 || job_number > MAX_PROCESSES)
		{
			perror("job number=0 invalid / fail to get job number \n");
			return 1;
		}
		if (strcmp(job_list[job_number - 1].name, "\0") != 0)
		{
			pID_Fg = job_list[job_number - 1].pid;
			strcpy(L_Fg_Cmd, job_list[job_number - 1].name);
			if(!job_list[job_number-1].is_running)
			{
				if(kill(job_list[job_number-1].pid, SIGCONT)==0)
				{
					printf("%s\n",job_list[job_number-1].name);
					printf("smash > signal SIGCONT was sent to pid %d\n", job_list[job_number-1].pid);
				}
				else
				{
					printf("smash error: >  fg %d – cannot send signal\n", job_number);
					return 1;
				}
			}
			if (waitpid(job_list[job_number - 1].pid, NULL, WUNTRACED) == -1)
			{
				strcpy(L_Fg_Cmd, "\0");
				pID_Fg = -1;
				perror("wait pid failed!\n");
			}
			DeleteJob(job_list[job_number - 1].pid, job_list);
			pID_Fg = -1;
			strcpy(L_Fg_Cmd, "\0");
			return 0;
		}
		else
		{
			perror("job not exist! \n");
			return 1;
		}
	}
	else if (num_arg == 0)
	{
		int i;
		if(last_stopped_process==-1) // no suspended processes
		{
			int last_bg_process = -1;
			for ( i = MAX_PROCESSES - 1; i >= 0; i--)
			{
				if (strcmp(job_list[i].name, "\0") != 0)
				{
					last_bg_process = i;
					break;
				}
			}
			if (last_bg_process == -1)
			{
				printf("no processes in bg!\n");
				return 0;
			}
			pID_Fg = job_list[last_bg_process].pid;
			strcpy(L_Fg_Cmd, job_list[last_bg_process].name);
			printf("%s\n",job_list[last_bg_process].name);
			if (waitpid(job_list[last_bg_process].pid, NULL, WUNTRACED) == -1)
			{
				perror("wait pid failed!\n");
				pID_Fg = -1;
				strcpy(L_Fg_Cmd, "\0");
			}
			DeleteJob(job_list[last_bg_process].pid, job_list);
			pID_Fg = -1;
			strcpy(L_Fg_Cmd, "\0");
			return 0;
		}
		else // fg to last suspend command
		{
			pID_Fg = job_list[last_stopped_process].pid;
			strcpy(L_Fg_Cmd, job_list[last_stopped_process].name);
			if(kill(job_list[last_stopped_process].pid, SIGCONT)==0)
			{
				printf("%s\n",job_list[last_stopped_process].name);
				printf("smash > signal SIGCONT was sent to pid %d\n", job_list[last_stopped_process].pid);
			}
			else
			{
				printf("smash error: >  fg %d – cannot send signal\n", last_stopped_process);
				return 1;
			}
			if (waitpid(job_list[last_stopped_process].pid, NULL, WUNTRACED) == -1)
			{
				perror("wait pid failed!\n");
				pID_Fg = -1;
				strcpy(L_Fg_Cmd, "\0");
			}
			DeleteJob(job_list[last_stopped_process].pid, job_list);
			pID_Fg = -1;
			strcpy(L_Fg_Cmd, "\0");
			return 0;
		}
	}
}

int cmdBg(char* args[MAX_ARG],job job_list[MAX_PROCESSES],int num_arg,int last_stopped_process){
	if(num_arg==1) // bg to specific process
	{
		int job_number = atoi(args[1]);
		if (job_number == 0 || job_number<1 || job_number > MAX_PROCESSES)
		{
			perror("job number=0 invalid / fail to get job number \n");
			return 1;
		}
		if (strcmp(job_list[job_number - 1].name, "\0") != 0)
		{
			if(job_list[job_number - 1].is_running)
				return 0;
			if(kill(job_list[job_number - 1].pid, SIGCONT)==0)
			{
				printf("smash > signal SIGCONT was sent to pid %d\n", job_list[job_number-1].pid);
				printf("%s\n", job_list[job_number - 1].name);
				//char  name[MAX_PROCESS_NAME_SIZE];
				//strcpy(name,job_list[job_number-1].name);
				//int process_pid = job_list[job_number-1].process_pid;
				//time_t process_start_time= job_list[job_number-1].start_running_time;
				//InsertJob(job_list[job_number-1].name,job_list[job_number-1].start_running_time,job_list[job_number-1].process_pid,TRUE,job_list);
				//DeleteJob(job_list[job_number-1].process_pid,job_list);
				//InsertJob(name,process_start_time,process_pid,TRUE,job_list);
				job_list[job_number-1].is_running=TRUE;
				return 0;
			}
			else
			{
				printf("smash error: >  bg %d – cannot send signal\n", job_number);
				return 1;
			}
		}
		else
		{
			printf("job number %d does'nt exist \n", job_number);
			return 1;
		}
	}
	else if(num_arg==0)
	{
		if(last_stopped_process==-1)
		{
			printf("no suspend jobs \n");
			return 0;
		}
		if(kill(job_list[last_stopped_process].pid, SIGCONT)==0)
		{
			printf("smash > signal SIGCONT was sent to pid %d\n", job_list[last_stopped_process].pid);
			printf("%s\n", job_list[last_stopped_process].name);
			//char  name[MAX_PROCESS_NAME_SIZE];
			//strcpy(name,job_list[last_stopped_process].name);
			//int process_pid = job_list[last_stopped_process].process_pid;
			//time_t process_start_time= job_list[last_stopped_process].start_running_time;
			//InsertJob(job_list[last_stopped_process].name,job_list[last_stopped_process].start_running_time,job_list[last_stopped_process].process_pid,TRUE,job_list);
			//DeleteJob(job_list[last_stopped_process].process_pid,job_list);

			job_list[last_stopped_process].is_running =TRUE;
			return 0;
		}
		else
		{
			printf("smash error: >  bg  – cannot send signal\n");
			return 1;
		}
	}
}

void cmdQuit(job job_list[MAX_PROCESSES]){
	UpdateJobsList(job_list);
	int i;
	for (i = 0; i < MAX_PROCESSES; i++)
	{ // going through all the job to kill each child
		if (job_list[i].pid != -1)
		{ // there is a job in index i
			printf("[%d] %s - Sending SIGTERM... ", i + 1, job_list[i].name);
			fflush(stdout);
			if (kill(job_list[i].pid,SIGTERM) == 0)
			{
				time_t sigSentTime = time(NULL);
				while (difftime(time(NULL), sigSentTime) < 5)
				{ // 5 sec hassn't passed
					if (waitpid(job_list[i].pid, NULL, WNOHANG) != 0)
					{ //finished
						printf("Done.\n");
						fflush(stdout);
						break;
					}
				}
				if (waitpid(job_list[i].pid, NULL, WNOHANG) == 0)
				{ // job didn't finish
					if (kill(job_list[i].pid,SIGKILL) == 0)
					{ // sent second sig
						printf("(5 sec passed) Sending SIGKILL... Done.\n");
						fflush(stdout);
					}
					else
					{// second sig wan't sent
						perror("quit kill - second SIGKILL error- was not sent");
					}
				}
			}
			else
			{// first sig wan't sent
				perror("quit kill - first SIGTERM error- was not sent");
			}
		}
	}
	exit(0);
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string, is it background and pointer
// to job list
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString, bool isbg, job job_list[MAX_PROCESSES])
{
    int pID;
    char* errmsg_fork;
	switch (pID = fork())
	{
		case -1:
			perror((isbg) ? "ExeExternal - failed to create new bg process\n" : "ExeExternal - failed to create new process\n");
			return;
		case 0:
			// Child Process
			setpgrp();
			execvp(args[0], args);
			perror((isbg) ? "ExeExternal - execvp failed (bg) :-(\n" : "ExeExternal: execvp failed :-(\n");
			exit(-1);
		default:
			// Father process
			if (isbg)
				InsertJob(args[0], pID, time(NULL), TRUE, job_list);
			else
			{
				// update the current process in foreground
				pID_Fg = pID;
				strcpy(L_Fg_Cmd, args[0]);
				if (waitpid(pID, NULL, WUNTRACED) == -1)
					perror("ExeExternal: waitpid failed!\n");
				pID_Fg = -1;
				strcpy(L_Fg_Cmd, "\0");
			}
	}
}

//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string, is it background and pointer to job list
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize, bool isbg, job job_list[MAX_PROCESSES])
{
	char *args[MAX_ARG];
	int i;
	if (((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
		&& lineSize[strlen(lineSize) - 2] != '&')
	{
		args[0] = (char*)"csh";
		args[1] = (char*)"-f";
		args[2] = (char*)"-c";
		args[3] = lineSize;
		for ( i = 4; i<MAX_ARG; i++)
		{
			args[i] = NULL;
		}
		int pID;
		switch (pID = fork())
		{
			case -1:
				perror((isbg) ? "ExeComp - failed to create new bg process\n" : "ExeComp - failed to create new process\n");
				return 0;
			case 0:
				// Child Process
				setpgrp();
                execvp(args[0], args);
                perror((isbg) ? "ExeComp - execvp failed (bg) :-(\n" : "ExeComp - execvp failed :-(\n");
				exit(-1);
			default:
				//Father process
				if (isbg)
				{
					InsertJob(args[0], pID, time(NULL), TRUE, job_list);
				}
				else
				{
					// update the current process in foreground
					pID_Fg = pID;
					strcpy(L_Fg_Cmd, lineSize);
					if (waitpid(pID, NULL, WUNTRACED) == -1)
						perror("ExeComp: waitpid failed!\n");
					pID_Fg = -1;
					strcpy(L_Fg_Cmd, "\0");
				}
				return 0;
		}
	}
	return -1;
}

//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, job job_list[MAX_PROCESSES])
{
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	int i;
	if (lineSize[strlen(lineSize) - 2] == '&')
	{
		lineSize[strlen(lineSize) - 2] = '\0';
		if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
			ExeComp(lineSize, TRUE, job_list);
		else
		{
			char * cmd = strtok(lineSize, delimiters);
			if (cmd == NULL)
				return 0;
			args[0] = cmd;
			for (i = 1; i<MAX_ARG; i++)
			{
				args[i] = strtok(NULL, delimiters);
			}
			ExeExternal(args, lineSize, TRUE, job_list);
		}
		return 0;
	}
	return -1;
}

//**************************************************************************************
// function name: InsertJob
// Description: insert a new job to the job list
// Parameters: pointer to jobs, process name, process pid, start running time, is it running or suspend
// Returns: bool (TRUE or FALSE)
//**************************************************************************************
bool InsertJob(char *process_name, int pid, time_t start_time, bool is_running, job *job_list)
{
	int i;
	for (i = 0; i<MAX_PROCESSES; i++)
	{
		if (strcmp(job_list[i].name, "\0") == 0)
		{
			job_list[i].is_running = is_running;
			job_list[i].pid = pid;
			job_list[i].start_time = start_time;
			strcpy(job_list[i].name, process_name);
			return TRUE;
		}
	}
	return FALSE;// job list is full
}

//**************************************************************************************
// function name: DeleteJob
// Description: delete a process from the job list
// Parameters: pointer to jobs, and the pid process
// Returns: bool (TRUE or FALSE)
//**************************************************************************************
bool DeleteJob(int process_pid, job *job_list)
{
	int i;
	int j;
	for (i = 0; i<MAX_PROCESSES; i++)
	{
		if (job_list[i].pid == process_pid)
		{
			for ( j = i; j<MAX_PROCESSES - 1; j++)
			{
				job_list[j] = job_list[j + 1];
			}

			job_list[MAX_PROCESSES - 1].is_running = FALSE;
			job_list[MAX_PROCESSES - 1].start_time = -1;
			job_list[MAX_PROCESSES - 1].pid = -1;
			strcpy(job_list[MAX_PROCESSES - 1].name, "\0");
			return TRUE;
		}
	}
	return FALSE; //job not found
}

//**************************************************************************************
// function name: UpdateJobsList
// Description: pass on the job list and delete the died processes
// Parameters: pointer to jobs
// Returns: None
//**************************************************************************************
void UpdateJobsList(job job_list[MAX_PROCESSES])
{
	int i;
	for ( i = 0; i < MAX_PROCESSES; i++)
	{
		if (job_list[i].pid != -1 && waitpid(job_list[i].pid, NULL, WNOHANG) != 0) {
			// if the process died
			DeleteJob(job_list[i].pid, job_list);
		}
	}
}
