/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:26 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/17 16:41:43 by ameskine         ###   ########.fr       */
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

#define SYNTAX_ERROR 1
#define INVALID_ARG 2


#define ERROR_SYNTAX "SYNTAX_ERROR : number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]\n"

typedef struct s_forks
{
    pthread_mutex_t fork;
} t_forks;

typedef struct philo_info t_philo_info ;

typedef struct prog_args
{
    int number_of_philosophers;
    t_forks **fork_;
    t_philo_info **philos;
    int num_times_to_eat;
    long time_to_sleep;
    long time_to_die;
    long time_to_eat;
    long start_time;
    int dead_philo;
    int is_simuation_ended;
    pthread_mutex_t lock_check_simulation;
    pthread_mutex_t last_meal_time_;
    pthread_mutex_t meal_eaten;
} t_prog_args;

typedef struct philo_info
{
    pthread_t philo_id;
    long last_meal_time;
    t_prog_args *init;
    int meals_eaten;
    int right_fork;
    int philo_num;
    int left_fork;
} t_philo_info;

void    print_status(t_philo_info *philo, char *status);
t_philo_info **philos_data_filling(t_prog_args *init);
t_prog_args *init_p_args(int *ac, char **av);
void    philo_eating(t_philo_info *philo);
long    current_time(void);
int ft_strlen(char *str);
long ft_atol(char *str);

#endif