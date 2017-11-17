//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
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
	for (i=1; i<MAX_ARG; i++)
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
	if (!strcmp(cmd, "cd") ) 
	{
		cmdCD();
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		
	}
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
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
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;

    	switch(pID = fork()) 
	{
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

               		if(execvp(args[0], args) == -1){
						//in case exec failed
						perror("failed to execute external command");//TODO maybe add the cmdString to the log error
					}

					break;
					/* 
			r		your code
					*/
			
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
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
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
int BgCmd(char* lineStr, Job jobs[MAX_PROCESSES])//todo fix signature
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineStr[strlen(lineStr)-2] == '&' && lineStr[strlen(lineStr)-3] != '|')
	{
		lineStr[strlen(lineStr)-2] = '\0';
		// Add your code here (execute a in the background)
		int pID;

		    	switch(pID = fork())
			{
		    		case -1:
							// Add your code here (error)
							//TODO err msg fork failed
		    				 perror("failed to create background process for background command");
		    				 break;
							/*
							your code
							*/
		        	case 0:
		                	// Child Process
		               		setpgrp(); //TODO why like this

					        // Add your code here (execute an external command)

		               		if(execvp(args[0], args) == -1){
								//in case exec failed
								perror("failed to execute external command");//TODO maybe add the cmdString to the log error
								exit(-1);
							}

							break;
							/*
					r		your code
							*/
					
					default:
		                	// Add your code here

						insertNewJob(jobs,pID,lineStr);
							break;


							/*
							your code
							*/
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
// Returns: 0- BG command -1- if not
//**************************************************************************************
int insertNewJob(Job jobs[MAX_PROCESSES],processID,lineStr){

	bool isValidParams = (processID > 0) && (lineStr[0] != '\0'); //TODO maybe there are more conditions
	if(!isValidParams) return -1;

	for(int i=0;i<MAX_PROCESSES;++i){
		//insert the new job in the first empty place in the list
		if(jobs[i].pid == -1){

			return pid;
		}
	}
	//got here in case there was no place for the new job
	return -1;
}















