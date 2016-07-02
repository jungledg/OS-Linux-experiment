#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_ARRAY 100
#define MAX_NUM 10000

void *selectsort(void *_argc)
{
	printf("test2\n");
	int *argv = (int *)_argc;
	int i,j,min,record = -1,temp;
	for(i = 0;i < MAX_ARRAY/2; ++i)
	{
		min = argv[i];
		for(j = i; j < MAX_ARRAY/2; ++j)
		{
			if(argv[j] < min)
			{
				record = j;
				min = argv[j];
			}
		}
		temp = argv[i];
		argv[i] = argv[record];
		argv[record] = temp;
	}
	return NULL;
}

void Merge(int *arg1,int *arg2)
{
	int i = 0,j = 0, k = 0;
	for(i = 0,j = MAX_ARRAY/2; i < MAX_ARRAY/2 && j < MAX_ARRAY; ++k)
	{
		if(arg1[i] < arg1[j])
		{
			arg2[k] = arg1[i++];
		}else {
			arg2[k] = arg1[j++];
		}
	}
	while(i < MAX_ARRAY/2)
	{
		arg2[k++] = arg1[i++];
	}
	while(j < MAX_ARRAY)
	{	
		arg2[k++] = arg1[j++];
	}
}


int main(int argc, char const *argv[])
{
	int array[MAX_ARRAY],i,result[MAX_ARRAY];
	FILE *file,*fp1,*fp2,*re;
	file = fopen("rand.txt","w+");
	fp1 = fopen("array1.txt","w+");
	fp2 = fopen("array2.txt","w+");
	re = fopen("sort-result.txt","w+");
	for(i = 0;i < MAX_ARRAY; ++i)
	{
		array[i] = (rand() % MAX_NUM); 
		fprintf(file, "%d\t", array[i]);

	}
	fclose(file);
	pthread_t tid;
	int *arg = &array[MAX_ARRAY/2];
	selectsort(array);
	pthread_create(&tid, NULL, selectsort, (void *)arg);
	if(pthread_join(tid, NULL) == 0)
	{
		for(i = 0;i < MAX_ARRAY/2; ++i)
		{
			fprintf(fp1, "%d\t", array[i]);
			fprintf(fp2, "%d\t", array[i+MAX_ARRAY/2]);
		}
		Merge(array,result);
	}
	fclose(fp1);
	fclose(fp2);
	for(i = 0;i < MAX_ARRAY; ++i)
		fprintf(re, "%d\t", result[i]);
	fclose(re);
	return 0;
}
