#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc,char* argv[])
{
	int i ;
	float a=0,b=0,r=0;
	char dir;
	float x,y ;
	int offset=0, pointstoread= 0;
	FILE *input;
	FILE *output;
	if(argc < 2)
	{
		fprintf(stderr,"Not enough arguments!\n");
		return -1;
	}
	while((i = getopt(argc,argv,":i:o:a:f:n:"))!= EOF )
	{
		switch(i)
		{
			case 'i':
				input = fopen(optarg,"rb");
				break;
			case 'a':
				a= atof(optarg);
				b= atof(argv[optind]);
				r= atof(argv[optind+1]);
				dir = argv[optind+2][0];
				break;
			case 'o':
				output=fopen(optarg,"w+");
				if (output == NULL)
				{
					printf("Output file could not be opened or created in circle!\n");
					exit(1);
				}
				break;
			case 'f':
				offset = atoi(optarg);
				break;
			case 'n':
				pointstoread = atoi(optarg);
				break;
			default:
				fprintf(stderr,"Invalid flag\n");
				return -1;
		}
	}
	fseek(input,0,SEEK_END);//go to the end of the file
	int filesize= ftell(input);// get the number of records in the file
	int numofrecords = filesize / 2*sizeof(float); // each record has 2 float numbers
	rewind(input); // return the file to the start
	if(offset > 0)
	{
		fseek(input,offset,SEEK_SET); // offset must be dividable by 8 because a float is 4 bytes long. So a point is 8 bytes long ( 2 float numbers)
		numofrecords -= offset/(2*sizeof(float)) ; // we skip the first offset/8 records
	}
	if(pointstoread > 0)numofrecords = pointstoread;
	for(i=0 ; i< numofrecords; i++)
	{
		fread(&x,sizeof(float),1,input);
		fread(&y,sizeof(float),1,input);
		if( ((x-a)*(x-a) + (y-b)*(y-b)) <= r*r  ) // x,y points of the circle ([a,b],r)
		{
			switch(dir)
			{
				case 'W':
					if(x<= a)
					{
						fprintf(output,"%.2f\t%.2f\n",x,y);
					}
					break;
				case 'N':
					if(y >= b)
					{
						fprintf(output,"%.2f\t%.2f\n",x,y);
					}
					break;
				case 'E':
					if (x>= a)
					{
						fprintf(output,"%.2f\t%.2f\n",x,y);
					}
					break;
				case 'S':
					if (y <= b)
					{
						fprintf(output,"%.2f\t%.2f\n",x,y);
					}
					break;
				default:
					printf("Invalid semicircle direction.\n");
					return -1;
			}
		}
	}
	if (input != NULL)fclose(input);
	if (output != NULL) fclose(output);
	return 0;
}
