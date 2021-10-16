#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

char *findReplace(char *string, char *oldWord, char *newWord);
int main(int ac, char *av[ ])
{
	printf("Child 3 executed updating the file....\n");
	
	
	FILE *fp; //file pointer
	int file_size; //file size
    char *content;
    char *update; 
    char add[] = "This is the updated version.\n";
	fp = fopen(av[1], "r"); 	//open fp with reading permissions
	
	if(!fp)
	{
		 perror(av[1]);
		 exit(1);
	}
	
	if(fseek(fp, 0, SEEK_END) == -1) //move file position to 
	{
	   fclose(fp);
	   free(content);
	   free(update);
	   perror("File seek Error");
	} 			
		
	file_size = ftell(fp);   //Get file size
	
	rewind(fp);					//go back to beginning
						
	content = malloc(file_size+1 ); 	//allocate memory for content
	
	//read file content to content, if return value is not as nmemb, then error occurred
	if(fread(content,file_size*sizeof(char),1,fp) != 1)
	{
	   fclose(fp);
	   free(content);
	   free(update);
	   fprintf(stderr, "Read error");
	   exit(1);
	}
	

	fclose(fp); 

	//allocate space for original file and space for add
	update = (char*)malloc((strlen(add)+strlen(content)+1)*sizeof(char)); 

	strcat(update, add); 			//concatenate add[] to beginning
	strcat(update, content);			//concatenate the rest
	update = findReplace(update, "execute", "run"); //find and replace the words

 	fp = fopen(av[1], "w"); 		//create empty file with same name	
 	
 	if(!fp)
	{
		 perror(av[1]);
		 exit(1);
	}
	
	if(fputs(update, fp) == EOF) 	//write to file from updated string
	{	
		perror("write error");
		free(content); 						
		free(update); 			
		exit(1);
	} 		
		
	fclose(fp);  //close file pointer
	
	free(content); 				
	free(update); 						
	
	return 0;
}

char *findReplace(char *string, char *oldWord, char *newWord){
	char *result;
	int i;
	int count = 0;
	int oldLen = strlen(oldWord);
	int newLen = strlen(newWord);

	//count number of times old word occurred in the string
	for(i = 0; string[i] != '\0'; i++){
		//if loop to check if the oldword occurred.
		//parses in current string position, strstr returns a pointer
		//if the returned pointer is same as current string position, then the oldword is present
		if(strstr(&string[i], oldWord) == &string[i]){
			count++;
			//change position to after the oldword
			//offset is length of oldword - 1
			i = i + oldLen - 1;
		}
	}

	//make a new string of enough length for the result
	//i is the number of characters in the old string
	//then add the difference between old and new, + 1 character for the terminator
	result = (char*)malloc(i + count * (newLen - oldLen) + 1);

	i = 0;
	while(*string){
		//while loop through old string
		//if find occurence of old word, then that pointer position will need to be updated
		//copy new word to the result string position
		//change position of result by length of new word
		//change position of old string by length of old word
		if(strstr(string, oldWord) == string){
			strcpy(&result[i], newWord);
			i+= newLen;
			string+= oldLen;
		}
		//if occurence is not present, then go to next character
		else {
			result[i++] = *string++;
		}
	}
	//after while loop, i value is the last character of result, add the string terminator
	result[i] = '\0';
	return result;

}