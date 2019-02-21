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
	float a=0.0,b=0.0,r1=0.0,r2=0.0,temp=0.0;
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
				r1= atof(argv[optind+1]);
				r2= atof(argv[optind+2]);
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
	if(r2>r1) // if r2 is bigger than r1 swap them because in the for loop we assume that r1 is the outside circle ( the one with the bigger radius
	{
		temp=r1;
		r1=r2;
		r2=temp;
	}
	for(i=0 ; i< numofrecords; i++)
	{
		fread(&x,sizeof(float),1,input);
		fread(&y,sizeof(float),1,input);
		if( ((x-a)*(x-a) + (y-b)*(y-b)) <= r1*r1  ) // x,y points of the circle ([a,b],r)
		{
			if(((x-a)*(x-a) + (y-b)*(y-b)) >= r2*r2 )
			{
				fprintf(output,"%.2f\t%.2f\n",x,y);
			}
		}
	}
	if (input != NULL)fclose(input);
	if (output != NULL) fclose(output);
	
	return 0;
}
