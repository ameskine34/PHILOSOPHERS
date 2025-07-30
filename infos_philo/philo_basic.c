#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];

typedef struct s_philosopher {
    int id;
    pthread_t thread;
    int left_fork_id;
    int right_fork_id;
} t_philosopher;

void *philosopher_routine(void *arg) {
    t_philosopher *philo = (t_philosopher *)arg;

    printf("Philosopher %d is thinking.\n", philo->id);

    // Odd/Even algorithm for fork acquisition
    if (philo->id % 2 != 0) { // Odd philosopher
        pthread_mutex_lock(&forks[philo->left_fork_id]);
        printf("Philosopher %d has taken left fork %d.\n", philo->id, philo->left_fork_id);
        pthread_mutex_lock(&forks[philo->right_fork_id]);
        printf("Philosopher %d has taken right fork %d.\n", philo->id, philo->right_fork_id);
    } else { // Even philosopher
        pthread_mutex_lock(&forks[philo->right_fork_id]);
        printf("Philosopher %d has taken right fork %d.\n", philo->id, philo->right_fork_id);
        pthread_mutex_lock(&forks[philo->left_fork_id]);
        printf("Philosopher %d has taken left fork %d.\n", philo->id, philo->left_fork_id);
    }

    printf("Philosopher %d is eating.\n", philo->id);
    usleep(100000); // Simulate eating

    pthread_mutex_unlock(&forks[philo->left_fork_id]);
    pthread_mutex_unlock(&forks[philo->right_fork_id]);
    printf("Philosopher %d has put down forks.\n", philo->id);

    printf("Philosopher %d is sleeping.\n", philo->id);
    usleep(100000); // Simulate sleeping

    return NULL;
}

int main() {
    t_philosopher philosophers[NUM_PHILOSOPHERS];

    // Initialize forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Initialize philosophers
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].id = i + 1;
        philosophers[i].left_fork_id = i;
        philosophers[i].right_fork_id = (i + 1) % NUM_PHILOSOPHERS;
    }

    printf("Starting philosopher simulation...\n");

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i].thread, NULL, philosopher_routine, &philosophers[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i].thread, NULL);
    }

    // Destroy forks
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    printf("Simulation finished.\n");

    return 0;
}


