/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:40:43 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/17 17:07:39 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_status(t_philo_info *philo, char *status)
{
    pthread_mutex_lock(&philo->init->lock_check_simulation);
    if (philo->init->is_simuation_ended)
    {
        pthread_mutex_unlock(&philo->init->lock_check_simulation);  
        return ;
    }
    long current = current_time() - philo->init->start_time;
    printf("%ld %d %s\n", current, philo->philo_num, status);
    pthread_mutex_unlock(&philo->init->lock_check_simulation);
}

t_philo_info **philos_data_filling(t_prog_args *init)
{
    t_philo_info **philo_infos;
    int j;

    j = 0;
    philo_infos = malloc((init->number_of_philosophers + 1) * sizeof(t_philo_info *));
    if (!philo_infos)
        return (NULL); 
    while (j < init->number_of_philosophers)
    {
        philo_infos[j] = malloc(sizeof(t_philo_info));
        if (!philo_infos[j])
            return (free(philo_infos), NULL);
        philo_infos[j]->philo_num = j + 1;
        philo_infos[j]->init = init;
        philo_infos[j]->meals_eaten = 0;
        //  pthread_mutex_init(&init->meal_eaten, NULL);
        philo_infos[j]->last_meal_time = init->start_time;
        philo_infos[j]->right_fork = j;
        philo_infos[j]->left_fork = (j + 1) % init->number_of_philosophers;
        if ((j % 2) == 0)
        {
            philo_infos[j]->left_fork = j;
            philo_infos[j]->right_fork = (j + 1) % init->number_of_philosophers;
        }
        j++;
    }
    philo_infos[j] = NULL;
    return (philo_infos);
}

t_prog_args *init_p_args(int *ac, char **av)
{
    t_prog_args *init;
    int i;

    init = malloc(sizeof(t_prog_args));
    if (!init)
        return (NULL);
    init->start_time = current_time();
    init->dead_philo = 0;
    pthread_mutex_init(&init->meal_eaten, NULL);
    pthread_mutex_init(&init->last_meal_time_, NULL);
    if ((init->number_of_philosophers = ft_atol(av[1])) == -1 || (init->number_of_philosophers == 0))
        return (free(init),NULL);
    if ((init->time_to_sleep = ft_atol(av[4])) == -1 || (init->time_to_sleep == 0))
        return (free(init),NULL);
    if ((init->time_to_die = ft_atol(av[2])) == -1 || (init->time_to_die == 0 ))
        return (free(init),NULL);
    if ((init->time_to_eat = ft_atol(av[3])) == -1 || (init->time_to_eat == 0 ))
        return (free(init),NULL);
    if (*ac == 6)
    {
        if((init->num_times_to_eat = ft_atol(av[5])) == -1 || (init->num_times_to_eat == 0 ))
        return (free(init),NULL);
    }
    else
        init->num_times_to_eat = -1;
    init->is_simuation_ended = 0;
    pthread_mutex_init(&init->lock_check_simulation, NULL);
    i = 0;
    if ((init->fork_ = malloc((init->number_of_philosophers + 1) * sizeof(t_forks *))) == NULL)
        return (free(init), NULL);
    while (i < init->number_of_philosophers)
    {
        if ((init->fork_[i] = malloc(sizeof(t_forks))) == NULL)
            return (free(init->fork_), NULL);
        pthread_mutex_init(&init->fork_[i]->fork, NULL);
        i++;
    }
    init->fork_[i] = NULL;
    return (init);
}

void philo_eating(t_philo_info *philo)
{
    pthread_mutex_lock(&philo->init->fork_[philo->right_fork]->fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(&philo->init->fork_[philo->left_fork]->fork);
    print_status(philo, "has taken a fork");
    pthread_mutex_lock(&philo->init->last_meal_time_);    
    philo->last_meal_time = current_time();
    pthread_mutex_unlock(&philo->init->last_meal_time_);    
    print_status(philo, "is eating");
    usleep(philo->init->time_to_eat * 1000);
    pthread_mutex_lock(&philo->init->meal_eaten);
    philo->meals_eaten++;
    pthread_mutex_unlock(&philo->init->meal_eaten);
    pthread_mutex_unlock(&philo->init->fork_[philo->right_fork]->fork);
    pthread_mutex_unlock(&philo->init->fork_[philo->left_fork]->fork);
}

int is_simulation_end(t_prog_args *data)
{
    int value;
    pthread_mutex_lock(&data->lock_check_simulation);
    value = data->is_simuation_ended;
    pthread_mutex_unlock(&data->lock_check_simulation);
    return (value);
}

void *routine_monitor(t_prog_args *data)
{
    int all_eaten;
    int i;

    while (!is_simulation_end(data))
    {
        i = 0;
        while (i < data->number_of_philosophers)
        {
            pthread_mutex_lock(&data->last_meal_time_);    
            if ((current_time() - data->philos[i]->last_meal_time) >= data->time_to_die)
            {
                print_status(data->philos[i], "died");
                pthread_mutex_lock(&data->lock_check_simulation);
                data->is_simuation_ended = 1;
                pthread_mutex_unlock(&data->lock_check_simulation);
                pthread_mutex_unlock(&data->last_meal_time_);
                return (NULL);
            }
            pthread_mutex_unlock(&data->last_meal_time_);
            i++;
        }
        i = 0;
        all_eaten = 1;
        if (data->num_times_to_eat != -1)
        {
            while (i < data->number_of_philosophers)
            {
                pthread_mutex_lock(&data->meal_eaten);
                if (data->philos[i]->meals_eaten < data->num_times_to_eat)
                {
                    all_eaten = 0;
                    pthread_mutex_unlock(&data->meal_eaten);
                    break;
                }
                pthread_mutex_unlock(&data->meal_eaten);
                i++;
            }
            if (all_eaten)
            {
                pthread_mutex_lock(&data->lock_check_simulation);
                data->is_simuation_ended = 1;
                pthread_mutex_unlock(&data->lock_check_simulation);
                return (NULL);
            }
        }
        usleep(100);
    }
    return (NULL);
}

void *start_routine(void *arg)
{
    t_philo_info *philo;

    philo = (t_philo_info *)arg;
    if (philo->philo_num % 2 != 0)
        usleep(1000);
    while (!is_simulation_end(philo->init))
    {
        philo_eating(philo);
        print_status(philo, "is sleeping");
        usleep(philo->init->time_to_sleep * 1000);
        print_status(philo, "is thinking");
        if ((philo->init->number_of_philosophers % 2 != 0) && (philo->philo_num % 2 != 0))
        {
            usleep(2000);
            if (philo->init->time_to_eat > philo->init->time_to_sleep)
                usleep((philo->init->time_to_eat - philo->init->time_to_sleep) * 1000);
        }
    }
    return (NULL);
}

int    philos_creation(t_prog_args *data)
{
    int n_of_philo;
    int j;
    
    j = 0;
    n_of_philo = data->number_of_philosophers;
    while (j < n_of_philo)
    {
        pthread_create(&data->philos[j]->philo_id, NULL, &start_routine, data->philos[j]);
        j++;
    }
    j = 0;
    routine_monitor(data);
    while (j < n_of_philo)
    {
        pthread_join(data->philos[j]->philo_id, NULL);
        j++;
    }
    return (0);
}

long    current_time(void)
{
    struct timeval tp;

    gettimeofday(&tp, NULL);
    long total_time = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
    return (total_time);
}

void error_printing(int i)
{
    if (i == SYNTAX_ERROR)
        write (1,ERROR_SYNTAX,ft_strlen(ERROR_SYNTAX));
    else if (i == INVALID_ARG)
        write(1, "ERROR : invalid arg\n", 20);
}

void    cleanup(t_philo_info **philo_infos, t_prog_args *init)
{
    int i;

    i = 0;
    while (i < init->number_of_philosophers)
    {
        pthread_mutex_destroy(&init->fork_[i]->fork);
        free(init->fork_[i]);
        free(philo_infos[i]);
        i++;
    }
    free(init->fork_);    
    free(philo_infos);
	free(init);
}


int main(int ac, char **av)
{
    t_philo_info **philo_init;
    t_prog_args *init;

    init = NULL;
    philo_init = NULL;
    if (ac < 5 || ac > 6)
        return (error_printing(SYNTAX_ERROR),1);
    init = init_p_args(&ac, av);
    if (!init)
        return (error_printing(INVALID_ARG),1);
    philo_init = philos_data_filling(init);
    if (philo_init == NULL)
        return (free(init) ,1);
    init->philos = philo_init;
    if (philos_creation(init) == -1)
        return (cleanup(philo_init, init), 1);
    cleanup(philo_init, init);
    return (0);
}
