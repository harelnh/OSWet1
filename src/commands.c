//		commands.c
//********************************************
#include "commands.h"


int cmdJobs(Job jobs[MAX_PROCESSES],char *cmd,char* args[MAX_ARG]);

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString) {
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0;
	args[0] = cmd;
	for (i = 1; i < MAX_ARG; i++) {
		args[i] = strtok(NULL, delimiters);
		if (args[i] != NULL)
			num_arg++;

	}
	/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
	/*************************************************/
	if (!strcmp(cmd, "cd")) {
//		cmdCD();
	}

	/*************************************************/
	else if (!strcmp(cmd, "pwd")) {

	}

	/*************************************************/
	else if (!strcmp(cmd, "mkdir")) {

	}
	/*************************************************/

	else if (!strcmp(cmd, "jobs")) {
		if(cmdJobs(jobs,cmd,args) == -1){
			//TODO handle error
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) {

	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) {

	}
	/*************************************************/
	else if (!strcmp(cmd, "bg")) {

	}
	/*************************************************/
	else if (!strcmp(cmd, "quit")) {

	}
	/*************************************************/
	else // external command
	{
		ExeExternal(args, cmdString);
		return 0;
	}
	if (illegal_cmd == TRUE) {
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
	return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString) {
	int pID;

	switch (pID = fork()) {
	case -1:
		// Add your code here (error)
		//TODO err msg fork failed
		perror("failed to create process for external command");
		break;
		/*
		 your code
		 */
	case 0:
		// Child Process
		setpgrp(); //TODO why like this

		// Add your code here (execute an external command)

		if (execvp(args[0], args) == -1) {
			//in case exec failed
			perror("failed to execute external command"); //TODO maybe add the cmdString to the log error
		}

		kill(getpid(), SIGKILL);
		break;

	default:
		// Add your code here
		waitpid(pID, NULL, 0);
		break;

		/*
		 your code
		 */
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 1- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize) {
	char ExtCmd[MAX_LINE_SIZE + 2];
	char *args[MAX_ARG];
	if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*"))
			|| (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&"))) {
		// Add your code here (execute a complicated command)

		/* 
		 your code
		 */
		//identify that is Exe Complicated
		return 1;
	}
	//identify that is NOT Exe Complicated
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 1- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineStr, Job jobs[MAX_PROCESSES]) //todo fix signature
{
	if (lineStr[strlen(lineStr) - 2] == '&' && lineStr[strlen(lineStr) - 3] != '|') {
		lineStr[strlen(lineStr) - 2] = '\0';
		// Add your code here (execute a in the background)
		int pID;
		switch (pID = fork()) {
		case -1:
			// Add your code here (error)
			//TODO err msg fork failed
			perror("failed to create background process for background command");
			break;

		case 0:
			// Child Process
			setpgrp(); //TODO why like this
			if (!ExeComp(lineStr)) {
				//prepare cmdString for ExeCmd()
				char cmdString[MAX_LINE_SIZE];
				strcpy(cmdString, lineStr);
				cmdString[strlen(lineStr) - 1] = '\0';
				// built in commands
				ExeCmd(jobs, lineStr, cmdString);
			}
			kill(getpid(), SIGKILL);
			break;

		default:
			//main process insert new Job to the jobs list
			if (insertNewJob(jobs, pID, lineStr) == -1) {
				printf("failed to create background process for background command.\n");
				//cancel the process we started
				kill(pID, SIGKILL);
			}
			break;
		}
		//identify that is BG cmd
		return 1;

	}
	//identify that is NOT BG cmd
	return -1;
}
//**************************************************************************************
// function name: insertNewJob
// Description: insert the new job in the first empty place in the list
// Parameters: Jobs list, process id, string of the entire command line
// Returns: process id - if insert success otherwise 0
//**************************************************************************************
int insertNewJob(Job jobs[MAX_PROCESSES], int processID, char *lineStr) {

	bool isValidParams = (processID > 0) && (lineStr[0] != '\0'); //TODO maybe there are more conditions
	if (!isValidParams) {
		return -1;
	}

	for (int i = 0; i < MAX_PROCESSES; ++i) {
		//insert the new job in the first empty place in the list
		if (jobs[i].pid == -1) {
			jobs[i].pid = processID;
			strcpy(jobs[i].cmdStr,lineStr);
			jobs[i].startTime = time(NULL);
			return processID;
		}
	}
	//got here in case there was no place for the new job
	return -1;
}

//TODO Docs
int removeJob(int processID,Job jobs[MAX_PROCESSES]) {
	bool isValidParams = (processID > 0); //TODO maybe there are more conditions
	if (!isValidParams) {
		return -1;
	}

	for (int i = 0; i < MAX_PROCESSES; ++i) {
		//insert the new job in the first empty place in the list
		if (jobs[i].pid == processID) {
			jobs[i].pid = -1;
			jobs[i].cmdStr[0] = '\0';
			jobs[i].startTime = -1;
			return processID;
		}
	}
	//got here in case there was no process with this id
	return -1;
}

int cmdJobs(Job jobs[MAX_PROCESSES],char *cmd,char* args[MAX_ARG]){
	int liveCnt = 0;
	//update job list - remove dead processes
	for(int i=0;i<MAX_PROCESSES;++i){
		//skip empty jobs5
		if(jobs[i].pid == -1){
			continue;
		}
		//check if process of the job finished and then remove from list
		if(waitpid(jobs[i].pid,NULL,WNOHANG) != 0){ //TODO verify if return -1 still mean that process is dead
			if(removeJob(jobs[i].pid,jobs) == -1){
				//TODO print err msg
				return -1;
			}
		}else{
			//print the job info
			liveCnt++;
			int runtime = time(NULL) - jobs[i].startTime;
			printf("[%d] %s : %d %d secs\n",liveCnt,cmd,jobs[i].pid,runtime);
		}

	}
	//got here if success to show job list
	return 1;
}

