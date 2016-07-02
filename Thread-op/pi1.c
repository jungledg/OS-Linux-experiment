#include <unistd.h> 
#include <pthread.h> 
#include <stdio.h>

#define MAX 1000000
#define RENO 2
double result[RENO];

void *thread(void *argc)
{
	int *arg = (int *)argc;
	double add = 0,number = 0;
	int flag = 0;
	for(int i = arg[0]; i <= arg[1]; ++i)
	{
		flag = flag ^ 1;
		number = 1.0/(2*i - 1);
		add += flag ? number : -number;
	}
	result[arg[2]] = add;
}

int main()
{
	int n = MAX,arra[3],arrb[3];
	arra[0] = 1;
	arra[1] = n/2;
	arra[2] = 0;
	arrb[0] = n/2 + 1;
	arrb[1] = n;
	arrb[2] = 1;
	pthread_t main_tid;
	pthread_create(&main_tid, NULL, &thread, (void *)arrb);
	thread((void *)arra);
	pthread_join(main_tid, NULL);
	printf("value of pi = %.8lf\n", 4 * (result[0] + result[1]));
	return 0;
}	
