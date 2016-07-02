#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define CHAR_NUM 8
#define ASC_a 97
#define ASC_A 65
#define FLUSH_SIZE 4

pthread_cond_t wait_empty_buffer_a;
pthread_cond_t wait_full_buffer_a;
pthread_cond_t wait_empty_buffer_b;
pthread_cond_t wait_full_buffer_b;
pthread_mutex_t mutex_a;
pthread_mutex_t mutex_b;
int buffer_a[FLUSH_SIZE];
int buffer_b[FLUSH_SIZE];
int in_a;
int in_b;
int out_a;
int out_b;

int buffer_is_empty(int input,int output)
{
    return input == output;
}

int buffer_is_full(int input,int output)
{
    return (input + 1) % FLUSH_SIZE == output;
}

int get_item_a()
{
    int item;

    item = buffer_a[out_a];
    out_a = (out_a + 1) % FLUSH_SIZE;
    return item;
}

int get_item_b()
{
    int item;

    item = buffer_b[out_b];
    out_b = (out_b + 1) % FLUSH_SIZE;
    return item;
}

void put_item_a(int item)
{
    buffer_a[in_a] = item;
    in_a = (in_a + 1) % FLUSH_SIZE;
}

void put_item_b(int item)
{
    buffer_b[in_b] = item;
    in_b = (in_b + 1) % FLUSH_SIZE;
}

void *consume(void *_argc)
{
	int i;
	int item;

	for(i = 0; i < CHAR_NUM; ++i)
	{
		pthread_mutex_lock(&mutex_b);
		while(buffer_is_empty(in_b,out_b))
			pthread_cond_wait(&wait_full_buffer_b, &mutex_b);

		item = get_item_b();
		printf("\t\tconsume item: %c\n", item);
		pthread_cond_signal(&wait_empty_buffer_b);
		pthread_mutex_unlock(&mutex_b);

	}
	return NULL;

}

void *calculater(void *_argc)
{
	int i;
	int item;
	
	for(i = 0;i < CHAR_NUM; ++i)
	{
		pthread_mutex_lock(&mutex_a);
		pthread_mutex_lock(&mutex_b);
		while(buffer_is_full(in_b,out_b) || buffer_is_empty(in_a,out_a))
		{
			pthread_cond_wait(&wait_empty_buffer_b, &mutex_b);
			pthread_cond_wait(&wait_full_buffer_a, &mutex_a);
			
		}

		item = get_item_a();
		item = item - (ASC_a - ASC_A);
		put_item_b(item);
		printf(" \tcalculate item: %c\n", item);
		
		pthread_cond_signal(&wait_full_buffer_b);
		pthread_mutex_unlock(&mutex_b);
		pthread_cond_signal(&wait_empty_buffer_a);
		pthread_mutex_unlock(&mutex_a);
	}
	return NULL;
}



void proceducer()
{
	int i;
	int item;

	for (i = 0; i < CHAR_NUM; i++) {
        pthread_mutex_lock(&mutex_a);
        while (buffer_is_full(in_a,out_a)) 
            pthread_cond_wait(&wait_empty_buffer_a, &mutex_a);

        item = i + 'a';
        printf("produce item: %c\n", item);
        put_item_a(item);

        pthread_cond_signal(&wait_full_buffer_a);
        pthread_mutex_unlock(&mutex_a);
    }
}


int main(int argc, char const *argv[])
{
	/* code */
	pthread_t con,cal;
	
	pthread_mutex_init(&mutex_a, NULL);
	pthread_mutex_init(&mutex_b, NULL);
	pthread_cond_init(&wait_empty_buffer_a, NULL);
	pthread_cond_init(&wait_full_buffer_a, NULL);
	pthread_cond_init(&wait_empty_buffer_b, NULL);
	pthread_cond_init(&wait_full_buffer_b, NULL);
	
	pthread_create(&con,NULL,consume,NULL);

	pthread_create(&cal,NULL,calculater,NULL);

	proceducer();

	sleep(0.2);
	return 0;
}
