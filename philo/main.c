/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:40:43 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/12 23:29:09 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_thread_info *threads_initialization(t_prog_args *init)
{
    t_thread_info *thread_init;
    int j;

    j = 0;
    thread_init = calloc((init->number_of_philosophers), sizeof(t_thread_info));
    if (!thread_init)
        return (NULL);
    while (j < init->number_of_philosophers)
    {
        thread_init[j].thread_num = j + 1;
        thread_init[j].init = init;
        j++;
    }
    return (thread_init);
}

t_prog_args *init_p_args(int *ac, char **av)
{
    t_prog_args *init;

    init = malloc(sizeof(t_prog_args));
    if (!init)
        return (NULL);
    init->start_time = 0;
    init->number_of_philosophers = ft_atol(av[1]);
    init->time_to_sleep = ft_atol(av[4]);
    init->time_to_die = ft_atol(av[2]);
    init->time_to_eat = ft_atol(av[3]);
    if (*ac == 6)
        init->num_times_to_eat = ft_atol(av[5]);
    return (init);
}

void *start_routine(void *arg)
{
    t_thread_info *ph;

    ph = (t_thread_info*)arg;
    if (ph->thread_num % 2 == 0)
    {
        printf("%ld %d is eating\n", current_time() - ph->init->start_time, ph->thread_num);
        usleep(ph->init->time_to_eat);
    }
    else
        printf("%ld %d is eating\n", current_time() - ph->init->start_time, ph->thread_num);
    return (NULL);
}

void    thread_creation(t_thread_info *thread_init)
{
    int n_of_philo;
    int j;
    int s;
    
    j = 0;
    s = 0;
    n_of_philo = thread_init->init->number_of_philosophers;
    thread_init->init->start_time = current_time();
    while (j < n_of_philo)
    {
        thread_init[j].thread_num = j + 1;
        s = pthread_create(&thread_init[j].thread_id, NULL, start_routine, &thread_init[j]);
        if (s)
            return ;
        j++;
    }
    j = 0;
    while (j < n_of_philo)
    {
        pthread_join(thread_init[j].thread_id, NULL);
        j++;
    }
}

long    current_time(void)
{
    struct timeval tp;

    gettimeofday(&tp, NULL);
    long total_time = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
    return (total_time);
}

int main(int ac, char **av)
{
    t_thread_info *thread_init;
    t_prog_args *init;

    if (ac < 5 || ac > 6)
    {
        write (1,ERROR_SYNTAX,ft_strlen(ERROR_SYNTAX));
        return (0);
    }
    init = init_p_args(&ac, av);
    thread_init = threads_initialization(init);
    thread_creation(thread_init);
    long i = current_time();
    printf("%ld\n", i);
}