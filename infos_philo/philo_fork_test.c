#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_PHILOSOPHERS 5

pthread_mutex_t forks[NUM_PHILOSOPHERS];
pthread_mutex_t print_mutex;

typedef struct s_philosopher {
    int id;
    pthread_t thread;
    int left_fork_id;
    int right_fork_id;
} t_philosopher;

void log_message(int philo_id, const char *msg) {
    pthread_mutex_lock(&print_mutex);
    printf("Philosopher %d %s\n", philo_id, msg);
    pthread_mutex_unlock(&print_mutex);
}

void *philosopher_routine(void *arg) {
    t_philosopher *philo = (t_philosopher *)arg;

    log_message(philo->id, "is thinking.");

    // Odd/Even algorithm for fork acquisition
    if (philo->id % 2 != 0) { // Odd philosopher
        // Acquire left fork
        pthread_mutex_lock(&forks[philo->left_fork_id]);
        log_message(philo->id, "has taken left fork.");
        // Acquire right fork
        pthread_mutex_lock(&forks[philo->right_fork_id]);
        log_message(philo->id, "has taken right fork.");
    } else { // Even philosopher
        // Acquire right fork
        pthread_mutex_lock(&forks[philo->right_fork_id]);
        log_message(philo->id, "has taken right fork.");
        // Acquire left fork
        pthread_mutex_lock(&forks[philo->left_fork_id]);
        log_message(philo->id, "has taken left fork.");
    }

    log_message(philo->id, "is eating.");
    usleep(100000); // Simulate eating

    // Release forks
    pthread_mutex_unlock(&forks[philo->left_fork_id]);
    pthread_mutex_unlock(&forks[philo->right_fork_id]);
    log_message(philo->id, "has put down forks.");

    log_message(philo->id, "is sleeping.");
    usleep(100000); // Simulate sleeping

    return NULL;
}

int main() {
    t_philosopher philosophers[NUM_PHILOSOPHERS];

    // Initialize mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }
    pthread_mutex_init(&print_mutex, NULL);

    // Initialize philosophers
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosophers[i].id = i + 1;
        philosophers[i].left_fork_id = i;
        philosophers[i].right_fork_id = (i + 1) % NUM_PHILOSOPHERS;
    }

    printf("Starting philosopher fork acquisition test...\n");

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosophers[i].thread, NULL, philosopher_routine, &philosophers[i]);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i].thread, NULL);
    }

    // Destroy mutexes
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    pthread_mutex_destroy(&print_mutex);

    printf("Fork acquisition test finished.\n");

    return 0;
}


