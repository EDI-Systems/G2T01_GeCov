/******************************************************************************
Filename    : coverage.c
Author      : pry
Date        : 16/06/2018
Licence     : LGPL v3+; see COPYING for details.
Description : The EDI general-purpose white-box testing coverage generator. 
              It takes in a coverage report which is often produced through serial 
			  output, and generates a coverage report w.r.t which branches or markers
			  are covered and which ones are not.
			  The format of the input file shall be:
			  LINE NUMBER:HIT TIMES.
			  This tool supports files as large as 1000000 lines.
			  Usage: coverage input.c MARKER_NAME input.txt report.txt
			  An example is provided in the MSVC project. The usage is
			  coverage kernel.c RMP_COVERAGE_MARKER kernel.txt report.txt
******************************************************************************/

/* Includes ******************************************************************/
#include "stdio.h"
#include "string.h"
/* End Includes **************************************************************/

/* Defines & Typedefs ********************************************************/
#define MAX_FILE_LINES 1000000
typedef unsigned int ptr_t;
/* End Defines & Typedefs ****************************************************/

/* Global Variables **********************************************************/
ptr_t Marker_Pos[MAX_FILE_LINES];
ptr_t Marker_Cnt[MAX_FILE_LINES];
/* One line can have maximum 4096 characters */
char Buffer[4096];
/* End Global Variables ******************************************************/

/* Begin Function:main ********************************************************
Description : The entry of the program.
Input       : int argc - The number of arguments, should always be 5.
			  char* argv[] - The array containing the arguments.
Output      : None.
Return      : int - If successful, 0; or an error code.
******************************************************************************/
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
        printf("Usage: coverage input.c MARKER_NAME input.txt report.txt");
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

    /* Print to the file */
	File=fopen(argv[4], "w");
	if(File==0)
	{
		printf("Failed to open the result file!\n");
		return -1;
	}

	Times_Temp=0;
    for(Count=0;Count<Pos_Cnt;Count++)
    {
        fprintf(File,"%d : %d\n",Marker_Pos[Count],Marker_Cnt[Count]);

		if (Marker_Cnt[Count] == 0)
		{
			printf("Marker at line %d not covered!\n",Marker_Pos[Count]);
			Times_Temp++;
		}
			
	}

	if(Times_Temp==0)
		printf("All markers covered!\n", Marker_Pos[Count]);
	else
		printf("%d markers not covered, coverage rate %.2f%%!\n", Times_Temp, (double)(Pos_Cnt-Times_Temp)/(double)Pos_Cnt*100);

	fclose(File);
	return 0;
}
/* End Function:main *********************************************************/

/* End Of File ***************************************************************/

/* Copyright (C) Evo-Devo Instrum. All rights reserved ***********************/

