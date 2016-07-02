#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LENGTH 256

int getParamKind(char* commond)
{
    if(strcmp(commond,"cd") == 0)
    	return 1;
    if(strcmp(commond,"pwd") == 0)
    	return 2;
    if(strcmp(commond,"exit") == 0)
    	return 3;
    if(strcmp(commond,"echo") == 0)
    	return 4;
    return 0;
}

int main(int argc,char *argv[])
{
	char commond[MAX_LENGTH];
	int kind = 0;
	char dir[MAX_LENGTH];
	char echo[MAX_LENGTH];
	while(1)
	{
		printf("gzd>");
		scanf("%s",commond);
		kind = getParamKind(commond);
		switch(kind)
		{
			case 1:
				scanf("%s",dir);
				chdir(dir);
				break;
			case 2:
				printf("%s\n", getcwd(NULL,0));
				break;
			case 3:
				printf("exit\n");
				return 0;
			case 4:
				gets(echo);
				printf("%s\n", echo);
				break;
			default:
				printf("Error commond\n");
				break;
		}
	}
	return 0;
    
}
