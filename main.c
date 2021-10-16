#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <errno.h>

void averageMark();
void wordCount(char *av1);
void updateFile(char *av2);
void parent();

int main(int ac , char *av[])
{
	if(ac <= 2)
    {
    	printf("Not enough arguments, only two arguments allowed.\n" );
    	exit(1);
	}

    if (ac > 3)
    {
        printf( "Too many arguments, only two arguments allowed. \n" );
        exit(1);
    }

	int i = 0;
	for(i = 0; i < 3; i++) 
	{
		if ((fork())== 0) //create 3 child processes to do the tasks depending on i value	
		{
		    printf("#### STAND BY......\n");
	
		    switch(i)
			{
		        case 0:
				averageMark();
				break;
				
		        case 1:
				wordCount(av[1]);
				break;
				
		        case 2:
				updateFile( av[ 2 ] );
				break;
	   		}
	
		    exit( 0 );
	    }
	}

	parent();

  
	return 0;
}

void parent()
{
    int wait_rv; 
    int child_status;
    int high_8;
    while ((wait_rv = wait(&child_status)) != -1)
    {
        high_8 = child_status >> 8; 
        printf("Parent: child %d done. exit status: %d\n", wait_rv, high_8);
    }
    
    if (errno == ECHILD){ //if no child processes
    	printf( "\nAll children have been returned\n" );
    } else{
    	perror( "wait" );
    }
    printf( "#### Parent %d\n completed", getpid( ) );
}

void wordCount( char *av1 )
{
	printf( "Child for word count is running...\n" );

	if(execl( "/usr/bin/wc", "wc", "-w", av1, NULL ) == -1 ) //execute file path of wc
	{
	   perror( "Exec 2 Error" );
	   exit(1);
	}

}

void updateFile( char *av2 )
{
	printf( "Child for update is running...\n" );
	
	if( execl( "upFile" ,"upFile" , av2 , NULL ) == -1 )  //execute update program on file
	{
	   perror( "Exec 3 error" );
	   exit(1);
	}

}

void averageMark( )
{
	
	printf("Child for average mark is running...\n");
    double avg = 0.0; 	//initialise variables
    double mark;
	double total= 0.0;
	double highest=0.0;
    int i;
	for( i = 0 ; i < 8 ; i++ )  //loop 8 times
	{
		printf( "Enter student mark:\n" );   //get input from user and check validity
    	scanf( "%lf" , &mark );
    	while( mark < 0.0 || mark > 100.0 )
		{
			printf( "Invalid mark, please enter between 0 and 100\n" );
			scanf( "%lf", &mark );
		}
		if(mark > highest){
			highest = mark;
		}
		total= mark + total;   //store new mark to total
	}
	avg = total/ 8;
	printf( "Average mark is %lf and the highest is %lf\n" , avg, highest);

	
}
