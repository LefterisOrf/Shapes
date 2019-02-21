#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <ctype.h>
#include "handler.h"

int main(int argc,char* argv[])
{
	int flagter=0,id,processnumber=0;
	char root[15],file[55],fifo[30],directory[25],color[15];
	int flag=0,pid,i,n,j=0,numofprocesses;
	char *arguments = malloc(200*sizeof(char));;
	int workerc=0;
	FILE *input;
	char *command[16]; // the number of different command in a line given to the cli are 10
	char **arg = malloc(16*sizeof(char*)); // the number of different arguments that a command can have is 7 at most, in our program at most 5 are needed	
	for (i = 0; i < 16; i++)
	{
		command[i]=NULL;
		arg[i]=NULL;
	}
	if(argc < 2)
	{
		fprintf(stderr,"Not enough arguments!\n");
		return -1;
	}
	while((j = getopt(argc,argv,":i:w:d:"))!= EOF )
	{
		switch(j)
		{
			case 'i':
				input = fopen(optarg,"rb");
				strcpy(file,optarg);
				break;
			case 'w':
				workerc= atoi(optarg);
				if (workerc == 0)
				{
					fprintf(stderr,"Invalid worker count number!\n");
				}
				break;
			case 'd':
				strcpy(directory,optarg);
				if(fork() == 0)
				{
					execlp("mkdir","mkdir",optarg,NULL);
					printf("exec failed\n");
					exit(1);
				}
				else wait(NULL);
				break;
			default:
				fprintf(stderr,"Invalid arguments in Master!\n");			
		}
	}
	struct pollfd *fdarray=malloc(workerc* sizeof(struct pollfd));
	fseek(input,0,SEEK_END);//go to the end of the file
	int filesize= ftell(input);// get the number of records in the file
	int numofrecords = filesize / (2*sizeof(float)); // each record has 2 float numbers
	fclose(input); // input is no longer needed in master
	int pointstoread =  (numofrecords/workerc); // number of records that each worker will read
	int modulo = (numofrecords % workerc ) ;
	int* numtoread = malloc(workerc * sizeof(int));
	for (i = 0; i < workerc; i++)
	{
		numtoread[i]= pointstoread;
		if(modulo > 0) 
		{
			numtoread[i]++;
			modulo --;
		}
	}
	while(flagter != 1)
	{
		for (i = 0; i < 16; i++)arg[i]= NULL;
		// split up the different commands (e.g: circle 40 33 1, square 3 4 1;)
		processnumber++;
		numofprocesses=0;
		flag=0;
		printf("prompt: ");
		fgets(arguments,100,stdin);
		j= strlen(arguments)-1; // get the lenth of the line
		if(arguments[j] == '\n')arguments[j]='\0'; // the \n is stored in the variable
		if((strcmp(arguments,"exit")== 0) || (strcmp(arguments,"exit;")== 0))break;
		for (i = 0; i < j; i++) 
		{
			if ((arguments[i]==',') || (arguments[i] == ';')) // here we split the line on 5 (at most) different commands e.g: circle 33 40 1, ring 12 40 2 3, ...
			{
				if(flag == 0)
				{
					command[flag]=malloc(50*sizeof(char)); // malloc space for the first command
					strcpy(command[flag],strtok(arguments,",;"));
					flag++;
				}
				else
				{
					command[flag]=malloc(50*sizeof(char));
					strcpy(command[flag],strtok(NULL,",;"));
					flag++;
				}
			}
		}
		// done spliting up
		for (i = 0; i < 16 ; i++)
		{
			if(command[i]!=NULL)numofprocesses++;
			else break;
		}
		// Setting up variable arg for execvp
		if(arg[0] == NULL)
		{
			for (i = 0; i <= 9; i++)
			{
				arg[i]=malloc(50 * sizeof(char));
			}
		}
		strcpy(arg[1],"-i");
		strcpy(arg[2],file);
		strcpy(arg[3],"-o");
		strcpy(arg[5],"-f");	
		strcpy(arg[7],"-n");
		strcpy(arg[9],"-a");
		// Done setting it up
		char **plot = malloc(numofprocesses * sizeof(char *));
		for (i = 0; i < numofprocesses; i++)
		{
			plot[i]=malloc(150 * sizeof(char));
		}
		for (i = 0; i < numofprocesses ; i++)
		{
/********************************************************************************************** Start of handler ******************************************************************************/	
			if((command_handling(arg,command[i],numofprocesses,i)) == -1)// start reading each the command variable-variable depending on the utility
			{
				printf("Error in command handling!\n");
				exit(1);
			}
			if((arg[15] == NULL) || (arg[14] == NULL))// in arg[15] we store the root for the exec
			{
				fprintf(stderr,"Command handling failed!\n");
				exit(1);
			}
			strcpy(root,arg[15]);
			strcpy(color,arg[14]);
			free(arg[15]);
			free(arg[14]);
			arg[15]=NULL;
			arg[14]=NULL;// done reading the command
			pid = fork(); // fork() for each process so every handler runs simultaneously
			if(pid == 0)
			{
				/* SET UP THE OUTPUTFILES							*/
				id = getpid();
				for (n = 0; n < workerc; n++)// in this loop i create all the fifo files for the communication between worker and handler
				{
					sprintf(fifo,"./%s/%d_w%d.fifo",directory,id,n);
					if((mkfifo(fifo,0666)) < 0)
					{
						fprintf(stderr,"Fifo could not be created!\n");
						exit(1);
					}
				}
				char *temp = malloc(50 * sizeof(char));
				sprintf(temp,"./%s/%d.txt",directory,id);//"./%s/%d.txt",directory,id
				FILE* handlerout=fopen(temp,"w+");
				if (handlerout == NULL)
				{
					printf("Could not open or create output file for handler!\n");
					exit(1);
				}
				if(temp != NULL)free(temp);
				// DONE SETTING IT UP
/************************************************************** Start running the workers *****************************************************************/
				int *fifos = malloc(workerc*sizeof(int));
				int offset = 0;
				for (n = 0; n <workerc ; n++)
				{
					if((pid=fork()) == 0)
					{
						sprintf(fifo,"./%s/%d_w%d.fifo",directory,id,n);
						strcpy(arg[4],fifo);
						sprintf(arg[8],"%d",numtoread[n]);
						sprintf(arg[6],"%d",offset); 
						execvp(root,arg);
						printf("Exec failed\n");
						exit(1);
					}
					else 
					{
						sprintf(fifo,"./%s/%d_w%d.fifo",directory,id,n);
						fifos[n] = open(fifo,O_RDWR);
					}
					offset+= (numtoread[n]*(2*sizeof(float)));
				}
/************************************************************* End of workers ****************************************************************************/
				/******************** Start processing the fifo files ****************/
				for (n = 0; n < workerc; n++)
				{
					fdarray[n].fd=fifos[n];
					fdarray[n].events=POLLIN;
					fdarray[n].revents=0;
				}
				int rc;
				int bufc= 2000;
				char* buffer = malloc(bufc* sizeof(char));
				while(1)
				{
					for (n = 0; n < workerc; n++)
					{
						fdarray[n].events = POLLIN;
						fdarray[n].revents = 0;
					}
					rc = poll(fdarray,workerc,300);
					if (rc < 0)
					{
						printf("Poll failed!\n");
						exit(1);
					}
					else if(rc == 0)break;
					else if (rc >= 1)
					{
						for (n = 0; n < workerc; n++)
						{
							if (fdarray[n].revents & POLLIN)
							{
								int numread;
								numread= read(fdarray[n].fd,buffer,(bufc-1)); 
								buffer[numread]='\0';
								fprintf(handlerout,"%s",buffer);
							}
						}
					}
				}
				/****************** Finish processing the fifo files *****************/
				for (n = 0; n < workerc; n++)
				{
					close(fdarray[n].fd);
				}
				free(buffer);
				if(handlerout != NULL)fclose(handlerout);
				for (n = 0; n < 16; n++) // make them NULL again
				{
					if(command[n] != NULL)free(command[n]);
					command[n]=NULL;
					if(arg[n] != NULL)free(arg[n]);
					arg[n]=NULL;
				}
				for (n = 0; n < numofprocesses ; n++)free(plot[n]);
				free(plot);
				free(numtoread);
				if(arg != NULL)free(arg);
				if(arguments != NULL)free(arguments);
				free(fdarray);
				free(fifos);
				exit(0);
			}
			else	sprintf(plot[i],"\"./%s/%d.txt\" notitle with points pointsize 0.5 linecolor rgb \"%s\"",directory,pid,color);
/***************************************************************************** End of handler ********************************************************************************************/
		}
		for (n = 0; n < numofprocesses; n++) wait(0); // wait for all the handlers to finish
		/*********************** Start running gnuplot *************************/	
		char* text = malloc(30*sizeof(char));
		sprintf(text,"./%s/%d_script.gnuplot",directory,i);
		FILE* gnu = fopen(text,"w");
		fprintf(gnu,"set terminal png\nset size ratio -1\nset output \"./%d_image.png\"\nplot \\\n",processnumber);
		for (n = 0; n < numofprocesses; n++)
		{
			fprintf(gnu,"%s",plot[n]);
			if(n != (numofprocesses-1))fprintf(gnu,",\\\n");
			else fprintf(gnu,"\n");
		}
		fclose(gnu);
		if(fork() == 0)
		{
			execlp("gnuplot","gnuplot","<",text,NULL);
			printf("Exec gnuplot failed!\n");
			exit(1);
		}
		else wait(NULL);
		free(text);
		/***************** Gnuplot has finished *************/
		for (n = 0; n < 16; n++) // make them NULL again
		{
			if(command[n] != NULL)free(command[n]);
			command[n]=NULL;
			if(arg[n] != NULL)free(arg[n]);
			arg[n]=NULL;
		}
		for (n = 0;  n< numofprocesses ; n++)
			free(plot[n]);
		free(plot);
	}
	for (n = 0; n < 16; n++) // make them NULL again
	{
		if(command[n] != NULL)free(command[n]);
		command[n]=NULL;
		if(arg[n] != NULL)free(arg[n]);
		arg[n]=NULL;
	}
	free(numtoread);
	free(fdarray);
	if(arg != NULL)free(arg);
	if(arguments != NULL)free(arguments);
	if(fork() == 0)
	{
		execlp("rm","rm","-rf",directory,NULL);
	}
	else wait(NULL);
	return 0;
}
