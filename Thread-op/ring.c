#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N 50

void *add(void *arg)
{
	int *num = (int *)arg;
	num[0]++;
	int *result = num;
	return (void *)result;
}

void init(int a[N][2])
{
	for(int i=0; i < N; ++i)
	{
		a[i][0] = 0;
		a[i][1] = i;
	}
}

int main(int argc, char const *argv[])
{
	int i = 0;
	pthread_t pid[N];
	int array[N][2];
	init(array);
	int *result;
	pthread_create(&pid[0], NULL, add, (void *)array[0]);
	pthread_join(pid[0], (void *)&result);
	
	while(i < N)
	{
		printf("from T[%d]", i+1);
		i = (i + 1) % N;
		printf("to T[%d] send %d\n", i+1, result[0]);
		pthread_create(&pid[i], NULL, add, result);
		pthread_join(pid[i], (void *)&result);
		sleep(1);
	}

	return 0;
}

