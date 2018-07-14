/* The coverage file generated shall be in format:
line:times
*/

/* Usage: ecov xxx.c MARKER xxx.txt result.txt. This supports files that are as large as 1000000 lines */
#include "stdio.h"
#include "string.h"

#define MAX_FILE_LINES             1000000
typedef unsigned int ptr_t;

ptr_t Marker_Pos[MAX_FILE_LINES];
ptr_t Marker_Cnt[MAX_FILE_LINES];
/* One line can have maximum 4096 characters */
char Buffer[4096];

int main(int argc, char* argv[])
{
    FILE* File;
    ptr_t Count;
	ptr_t Pos_Iter;
    ptr_t Pos_Cnt;
    ptr_t Pos_Temp;
    ptr_t Times_Temp;

    if(argc!=5)
    {
        printf("Usage: ecov xxx.c MARKER xxx.txt result.txt");
        return -1;
    }

    /* Open the source file as read-only */
    File=fopen(argv[1], "r");
    if(File==0)
    {
        printf("Failed to open the source file!\n");
        return -1;
    }

    /* Scan the list of markers */
    Pos_Cnt=0;
    for(Count=1;Count<MAX_FILE_LINES;Count++)
    {
        if(fgets(Buffer,4096,File)==0)
            break;
        if(strstr(Buffer,argv[2])==0)
            continue;
        
        Marker_Pos[Pos_Cnt]=Count;
        Marker_Cnt[Pos_Cnt]=0;
        Pos_Cnt++;
    }

    fclose(File);

    /* If there are no markers in this file, we return immediately */
    if(Marker_Cnt==0)
    {
        printf("No designated marker found in the source file!\n");
        return -1;
    }

    File=fopen(argv[3], "r");
    if(File==0)
    {
        printf("Failed to open the coverage data file!\n");
        return -1;
    }

	Pos_Iter=0;
    for(Count=0;Count<MAX_FILE_LINES;Count++)
    {
		if(fgets(Buffer, 4096, File)==0)
			break;

		if((Buffer[0]>'9')||(Buffer[0]<'0'))
			continue;
		
		/* If what we got is not the format that we want, exit */
		if(sscanf(Buffer, "%d:%d\n", &Pos_Temp, &Times_Temp)<0)
			break;

		/* Find the slot that has the same line number */
		while(Marker_Pos[Pos_Iter]!=Pos_Temp)
		{
			Pos_Iter++;
			if(Pos_Iter>=Pos_Cnt)
			{
				printf("An error occurred in the coverage data file: marker not found!\n");
				return -1;
			}
		}

		Marker_Cnt[Pos_Iter]=Times_Temp;
    }

    fclose(File);

    /* Print to the file - currently just to the ...*/
	File=fopen(argv[4], "w");
	if(File==0)
	{
		printf("Failed to open the result file!\n");
		return -1;
	}

    for(Count=0;Count<Pos_Cnt;Count++)
    {
        fprintf(File,"%d : %d\n",Marker_Pos[Count],Marker_Cnt[Count]);
    }

	fclose(File);
	return 0;
}
