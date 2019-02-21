#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int command_handling(char** arg,char* command,int numofprocesses,int curprocess)// arg is the standard 16 space array(of char*) we use in the master.c file
{
	char utility[15];
	int n;
	
				arg[15]=malloc(15*sizeof(char)); // in arg[15] we store the root string
				arg[14]=malloc(15*sizeof(char)); // in arg[14] we store the colour 
				strcpy(utility,strtok(command," ;"));
				if((strcmp(utility,"circle")==0) || (strcmp(utility," circle")==0)) // Handler for circle utilities
				{
					strcpy(arg[15],"./circle");
					strcpy(arg[0],"circle");
					for (n = 10; n <= 12; n++)
					{
						if(arg[n] == NULL)arg[n]=malloc(15*sizeof(char));
					}
					strcpy(arg[10],strtok(NULL," "));// the string we have till the first space character is stored in arg[10]
					strcpy(arg[11],strtok(NULL," "));
					strcpy(arg[12],strtok(NULL," "));
					if(curprocess == (numofprocesses-1))strcpy(arg[14],strtok(NULL,";")); // if its the last process the the last argument is followed by a semicolon (;)
					else strcpy(arg[14],strtok(NULL," "));
					if((arg[10] == NULL) || (arg[11] == NULL) || (arg[12] == NULL) || (arg[14]== NULL))
					{
						fprintf(stderr,"Invalid arguments in circle!\n");
						return -1;
					}
				}
				else if((strcmp(utility,"ring") == 0) || (strcmp(utility," ring")==0))
				{
					strcpy(arg[15],"./ring");
					strcpy(arg[0],"ring");
					for (n = 10; n <= 13; n++)
					{
						if(arg[n] == NULL)arg[n]=malloc(15*sizeof(char));
					}
					strcpy(arg[10],strtok(NULL," "));
					strcpy(arg[11],strtok(NULL," "));
					strcpy(arg[12],strtok(NULL," "));
					strcpy(arg[13],strtok(NULL," "));
					if(curprocess == (numofprocesses-1))strcpy(arg[14],strtok(NULL,";")); // if its the last process the the last argument is followed by a semicolon (;)
					else strcpy(arg[14],strtok(NULL," "));
					if((arg[10] == NULL) || (arg[11] == NULL) || (arg[12] == NULL) || (arg[13] == NULL) || (arg[14] == NULL))
					{
						fprintf(stderr,"Invalid points in ring!\n");
						return -1;
					}
				}
				else if((strcmp(utility,"square")==0) || (strcmp(utility," square")==0))
				{
					strcpy(arg[15],"./square");
					strcpy(arg[0],"square");
					for (n = 10; n <= 12; n++)
					{
						if(arg[n] == NULL)arg[n]=malloc(15*sizeof(char));
					}
					strcpy(arg[10],strtok(NULL," "));
					strcpy(arg[11],strtok(NULL," "));
					strcpy(arg[12],strtok(NULL," "));
					if(curprocess == (numofprocesses-1))strcpy(arg[14],strtok(NULL,";")); // if its the last process the the last argument is followed by a semicolon (;)
					else strcpy(arg[14],strtok(NULL," "));
					if((arg[10] == NULL) || (arg[11] == NULL) || (arg[12] == NULL) || (arg[14] == NULL))
					{
						fprintf(stderr,"Invalid points in square!\n");
						return -1;
					}
				}
				else if((strcmp(utility,"elipse")==0) || (strcmp(utility," elipse")==0))
				{
					strcpy(arg[15],"./elipse");
					strcpy(arg[0],"elipse");
					for (n = 10; n <= 13; n++)
					{
						if(arg[n] == NULL)arg[n]=malloc(15*sizeof(char));
					}
					strcpy(arg[10],strtok(NULL," "));
					strcpy(arg[11],strtok(NULL," "));
					strcpy(arg[12],strtok(NULL," "));
					strcpy(arg[13],strtok(NULL," "));
					if(curprocess == (numofprocesses-1))strcpy(arg[14],strtok(NULL,";")); // if its the last process the the last argument is followed by a semicolon (;)
					else strcpy(arg[14],strtok(NULL," "));
					if((arg[10] == NULL) || (arg[11] == NULL) || (arg[12] == NULL) || (arg[13] == NULL) || (arg[14]== NULL))
					{
						fprintf(stderr,"Invalid points in elipse!\n");
						return -1;
					}
				}
				else if((strcmp(utility,"semicircle")==0) || (strcmp(utility," semicircle")==0))
				{
					strcpy(arg[15],"./semicircle");
					strcpy(arg[0],"semicircle");
					for (n = 10; n <= 13; n++)
					{
						if(arg[n] == NULL)arg[n]=malloc(15*sizeof(char));
					}
					strcpy(arg[0],"semicircle");
					strcpy(arg[10],strtok(NULL," "));
					strcpy(arg[11],strtok(NULL," "));
					strcpy(arg[12],strtok(NULL," "));
					strcpy(arg[13],strtok(NULL," "));
					if(curprocess == (numofprocesses-1))strcpy(arg[14],strtok(NULL,";")); // if its the last process the the last argument is followed by a semicolon (;)
					else strcpy(arg[14],strtok(NULL," "));
					if((arg[10] == NULL) || (arg[11] == NULL) || (arg[12] == NULL) || (arg[13] == NULL) || (arg[14] == NULL))
					{
						fprintf(stderr,"Invalid points in semicircle!\n");
						return -1;
					}
				}
				else
				{
					free(arg[15]);
					printf("Invalid utility name!\n");
					return -1;
				}
				return 0;// the root is stored in arg[15] (last array position which isnt used by any utility
}
