/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:40:43 by ameskine          #+#    #+#             */
/*   Updated: 2025/07/31 00:16:05 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_thread_info *threads_initialization(t_prog_args *init)
{
    t_thread_info *thread_init;

    thread_init = calloc((init->number_of_philosophers), sizeof(t_thread_info)); // to change it to ft_calloc
    if (!thread_init)
        return (NULL);
    thread_init->init = init;
    return (thread_init);
}
t_prog_args *init_p_args(int *ac, char **av, int *error)
{
    t_prog_args *init;

    init = malloc(sizeof(t_prog_args));
    if (!init)
        return (NULL);
    init->number_of_philosophers = ft_atol(av[1], error);
    init->time_to_sleep = ft_atol(av[4], error);
    init->time_to_die = ft_atol(av[2], error);
    init->time_to_eat = ft_atol(av[3], error);
    if (*ac == 6)
        init->num_times_to_eat = ft_atol(av[5], error);
    return (init);
}

void *start_routine(void *arg)
{
    while (1)
    {
        int id = *(int *)arg;
        printf ("%d\n", id);
        usleep(100);
    }
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
    while (j < n_of_philo)
    {
        thread_init[j].thread_num = j + 1;
        s = pthread_create(&thread_init[j].thread_id, NULL, &start_routine, &thread_init[j]);
        if (s)
            return ;
        j++;
    }   
}

int main(int ac, char **av)
{
    t_thread_info *thread_init;
    t_prog_args *init;
    int error;

    error = 0;
    if (ac < 5 || ac > 6)
    {
        write (1,ERROR_SYNTAX,ft_strlen(ERROR_SYNTAX));
        return (1);
    }
    init = init_p_args(&ac, av, &error);
    if (error == 1)
    {
        write(1,"ERROR : invalid arg\n",20);
        return (1);
    }
    thread_init = threads_initialization(init);
    thread_creation(thread_init);
}