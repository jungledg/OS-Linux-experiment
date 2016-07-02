#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int getParamKind(char* commond)
{
    if(strcmp(commond,"cd") == 0)
    	return 1;
    if(strcmp(commond,"pwd") == 0)
    	return 2;
    if(strcmp(commond,"exit") == 0)
    	return 3;
    return 0;
}




int main(int argc,char *argv[])
{
	char commond[256];
	int kind = 0;
	char dir[10];
	char *cwd;
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
			default:
				printf("Error commond\n");
		}
	}
	return 0;
}



void do_cd(char *argv[])
{

	chdir(argv[1]);
}
