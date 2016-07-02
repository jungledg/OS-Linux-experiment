#include <stdio.h>
#include <pthread.h>

#define MAX_THREAD_NUM 500
#define MAX 1000000
#define SINGLE_THREAD 2000

double result[MAX_THREAD_NUM];

void *thread(void *argc)
{

	int *argv = (int *)argc;
	double add = 0,number = 0;
	int flag = 0;
	for(int i = argv[0];i < (SINGLE_THREAD + argv[0]); ++i)
	{
		flag = flag ^ 1;
		number = 1.0 / (2 * i - 1);
		add += flag ? number : -number;
	}
	printf("%.2lf\t",add);
	result[argv[1]] = add;
	return NULL;
}

int main(int argc, char const *argv[])
{
	/* code */
	int i = 0;
	int args[MAX_THREAD_NUM][2];
	double output = 0;
	pthread_t threads[MAX_THREAD_NUM];
	for(i = 0; i < MAX_THREAD_NUM; ++i)
	{
		args[i][0] = i * 2000 + 1;
		args[i][1] = i;
		pthread_create(&threads[i] , NULL, thread, (void *)args[i]);
	}
	for(i = 0; i < MAX_THREAD_NUM; ++i)
	{
		pthread_join(threads[i],NULL);
		output += result[i];
	}
	printf("value of pi is %.8lf\n", 4 * (output));
	return 0;
}
