/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:26 by ameskine          #+#    #+#             */
/*   Updated: 2025/07/30 19:18:14 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>


#define ERROR_SYNTAX "SYNTAX_ERROR : number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"

typedef struct prog_args
{
    int number_of_philosophers;
    int num_times_to_eat;
    long time_to_sleep;
    long time_to_die;
    long time_to_eat;
} t_prog_args;

typedef struct thread_info
{
    pthread_t thread_id;
    int thread_num;
    t_prog_args *init;
} t_thread_info;

t_prog_args *init_p_args(int *ac, char **av, int *error);
t_thread_info *threads_initialization(t_prog_args *init);
long ft_atol(char *str, int *error);
int ft_strlen(char *str);

#endif