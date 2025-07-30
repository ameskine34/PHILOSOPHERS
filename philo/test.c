/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 09:41:45 by ameskine          #+#    #+#             */
/*   Updated: 2025/07/30 22:00:38 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *my_turn(void *arg)
{
    int i =0;
    (char *)arg;
    while (i < 5)
    {
        sleep(1);
        printf("%s\n", arg);
        i++;
    }
    return (NULL);
}

void *my_turn1(void *arg)
{
    int i =0;
    (char *)arg;
    while (i < 8)
    {
        sleep(1);
        printf("%s\n",arg);
        i++;
    }
    return (NULL);
}

void your_turn()
{
    int j = 0;   
    while (j < 3)
    {
        // sleep(2);
        printf("Your Turn!\n");
        j++;
    }
}
int main()
{
    pthread_t newthread; // thread id that the function populate 
    pthread_t newthread1;
    char *s = "amine,meskine";
    
    pthread_create(&newthread, NULL, my_turn, s);
    pthread_create(&newthread1, NULL, my_turn1, s);
    your_turn();
    pthread_join(newthread, NULL);
    pthread_join(newthread1, NULL);
}

// Here's a step-by-step explanation:
// Main Thread Starts: When you run your program, the main() function begins executing in a single, default thread. This is often called the "main thread."
// Thread Creation:
// pthread_create(&newthread, ...): You create a new thread. The operating system starts executing the my_turn function in parallel.
// pthread_create(&newthread1, ...): You create a second thread, which starts executing the my_turn1 function in parallel.
// Concurrent Execution: Now, you have three threads running concurrently:
// The main thread (executing main())
// newthread (executing my_turn())
// newthread1 (executing my_turn1())
// The Race Condition: The main thread continues its own execution and calls your_turn(). The your_turn() function takes about 6 seconds to complete (3 iterations * 2 seconds sleep).
// Meanwhile, the other two threads are also running, each taking 8 seconds.
// Termination of main: This is the key part. After your_turn() finishes, what happens next in the main function? Without the pthread_join calls, 
// the main function simply reaches its end. When the main function of a C program terminates, it signals the operating system to clean up the entire process. 
// This includes terminating all threads associated with that process, regardless of whether they have finished their work.
// Why pthread_join Solves This
// The pthread_join() function is a synchronization call that essentially says: "Pause the execution of the current thread until the specified thread has completed."
// pthread_join(newthread, NULL);: This line tells the main thread to stop and wait until newthread (running my_turn) has finished its 8-second execution and returned.
// pthread_join(newthread1, NULL);: This line does the same for newthread1.
// By adding these calls, you guarantee that the main function will not exit until both of its created threads have completed their tasks, allowing you to see all of their output as intended.
// To put it simply: Without pthread_join, the main thread doesn't wait for its children threads to finish, and its own termination causes the entire program, 
// including the other threads, to end prematurely.