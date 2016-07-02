#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 4
#define ASC_a 97
#define ASC_A 65

int buffer_a[BUFFER_SIZE];
int buffer_b[BUFFER_SIZE];
int in_a;
int in_b;
int out_a;
int out_b;



typedef struct
{
	int value;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}semaphore;

void semaphore_init(semaphore *sema, int value)
{
	sema->value = value;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);
}

void semaphore_wait(semaphore *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value--;
	while(sema->value < 0)
		pthread_cond_wait(&sema->cond, &sema->mutex);
	pthread_mutex_unlock(&sema->mutex);
}

void semaphore_signal(semaphore *sema)
{
	pthread_mutex_lock(&sema->mutex);
	sema->value++;
	pthread_cond_signal(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
}


int buffer_is_empty(int input,int output)
{
    return input == output;
}

int buffer_is_full(int input,int output)
{
    return (input + 1) % BUFFER_SIZE == output;
}

int get_item_a()
{
    int item;

    item = buffer_a[out_a];
    out_a = (out_a + 1) % BUFFER_SIZE;
    return item;
}

int get_item_b()
{
    int item;

    item = buffer_b[out_b];
    out_b = (out_b + 1) % BUFFER_SIZE;
    return item;
}

void put_item_a(int item)
{
    buffer_a[in_a] = item;
    in_a = (in_a + 1) % BUFFER_SIZE;
}

void put_item_b(int item)
{
    buffer_b[in_b] = item;
    in_b = (in_b + 1) % BUFFER_SIZE;
}

semaphore mutex_sema_a;
semaphore mutex_sema_b;
semaphore empty_buffer_sema_a;
semaphore empty_buffer_sema_b;
semaphore full_buffer_sema_a;
semaphore full_buffer_sema_b;

#define COUNT (BUFFER_SIZE * 2)

void *consume(void *arg)
{
	int i;
	int item;

	for(i = 0; i < COUNT; ++i)
	{
		semaphore_wait(&full_buffer_sema_b);
		semaphore_wait(&mutex_sema_b);

		item = get_item_b();

		semaphore_signal(&mutex_sema_b);
		semaphore_signal(&empty_buffer_sema_b);

		printf("\t\tconsume item: %c\n", item);
	}
	return NULL;
}

void *calculater(void *arg)
{
	int i;
	int item;

	for(i = 0; i < COUNT; ++i)
	{
		semaphore_wait(&full_buffer_sema_a);
		semaphore_wait(&mutex_sema_a);

		item = get_item_a();
		printf("\ttest: %c\n", item);
		semaphore_signal(&mutex_sema_a);
        semaphore_signal(&empty_buffer_sema_a);

        semaphore_wait(&empty_buffer_sema_b);
        semaphore_wait(&mutex_sema_b);

        item = item - (ASC_a - ASC_A);
        put_item_b(item);
    
        semaphore_signal(&mutex_sema_b);
        semaphore_signal(&full_buffer_sema_b);

        printf("\tcalculator item: %c\n", item);

	}
	return NULL;
}

void produce()
{
    int i;
    int item;

    for (i = 0; i < COUNT; i++) {
        semaphore_wait(&empty_buffer_sema_a);
        semaphore_wait(&mutex_sema_a);

        item = i + 'a';
        put_item_a(item);
    
        semaphore_signal(&mutex_sema_a);
        semaphore_signal(&full_buffer_sema_a);

        printf("produce item: %c\n", item);
    }
}


int main(int argc, char const *argv[])
{
	/* code */
	//semaphore pro,cal,con;
	pthread_t consume_tid,calculater_pid;

	semaphore_init(&mutex_sema_a, 1);
	semaphore_init(&mutex_sema_b, 1);

	semaphore_init(&empty_buffer_sema_b, BUFFER_SIZE - 1);
    semaphore_init(&empty_buffer_sema_a, BUFFER_SIZE - 1);
    
    semaphore_init(&full_buffer_sema_a, 0);
    semaphore_init(&full_buffer_sema_b, 0);

	pthread_create(&consume_tid, NULL, consume, NULL);
	pthread_create(&calculater_pid, NULL, calculater, NULL);
	produce();

	sleep(1);
	return 0;
}
