#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
int counter = 0;

void *thread_function(void *arg) {
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        printf("Thread %ld: Counter = %d\n", (long)arg, counter);
        pthread_mutex_unlock(&mutex);
        usleep(100000); // Simulate some work
    }
    return NULL;
}

int main() {
    pthread_t tid[2];

    pthread_mutex_init(&mutex, NULL);

    printf("Starting threads...\n");

    pthread_create(&tid[0], NULL, thread_function, (void *)1);
    pthread_create(&tid[1], NULL, thread_function, (void *)2);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    pthread_mutex_destroy(&mutex);

    printf("Final counter value: %d\n", counter);
    return 0;
}


