/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:58:20 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/18 15:53:07 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eating(t_philo_info *philo)
{
	pthread_mutex_lock(&philo->init->fork_[philo->right_fork]->fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->init->fork_[philo->left_fork]->fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&philo->init->last_meal_time_);
	philo->last_meal_time = current_time();
	pthread_mutex_unlock(&philo->init->last_meal_time_);
	print_status(philo, "is eating");
	if (philo->init->time_to_eat >= philo->init->time_to_die)
		usleep(philo->init->time_to_die * 1000);
	else
		usleep(philo->init->time_to_eat * 1000);
	pthread_mutex_lock(&philo->init->meal_eaten);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->init->meal_eaten);
	pthread_mutex_unlock(&philo->init->fork_[philo->right_fork]->fork);
	pthread_mutex_unlock(&philo->init->fork_[philo->left_fork]->fork);
}

void	*routine_monitor_continue(t_prog_args *data, int i, int all_eaten)
{
	if (data->n_to_eat != -1)
	{
		while (i < data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->meal_eaten);
			if (data->philos[i]->meals_eaten < data->n_to_eat)
			{
				all_eaten = 0;
				pthread_mutex_unlock(&data->meal_eaten);
				break ;
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
	usleep(50);
	return (NULL);
}

void	*routine_monitor(t_prog_args *data)
{
	void	*return_;
	int		i;

	return_ = NULL;
	while (!is_simulation_end(data))
	{
		i = -1;
		while (++i < data->number_of_philosophers)
		{
			pthread_mutex_lock(&data->last_meal_time_);
			if ((current_time()
					- data->philos[i]->last_meal_time) >= data->time_to_die)
			{
				print_status(data->philos[i], "died");
				pthread_mutex_lock(&data->lock_check_simulation);
				data->is_simuation_ended = 1;
				pthread_mutex_unlock(&data->lock_check_simulation);
				pthread_mutex_unlock(&data->last_meal_time_);
				return (NULL);
			}
			pthread_mutex_unlock(&data->last_meal_time_);
		}
		return_ = routine_monitor_continue(data, 0, 1);
	}
	return (NULL);
}

void	*one_philo_case(t_philo_info *philo)
{
	if (philo->init->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->init->print_mutex);
		printf("%ld %d has taken a fork\n", current_time()
			- philo->init->start_time, philo->philo_num);
		pthread_mutex_unlock(&philo->init->print_mutex);
		return (usleep(philo->init->time_to_die * 1000), NULL);
	}
        return (NULL);
}

void	*start_routine(void *arg)
{
	t_philo_info	*philo;

	philo = (t_philo_info *)arg;
	if (philo->philo_num % 2 != 0)
		usleep(1000);
	while (!is_simulation_end(philo->init))
	{
		one_philo_case(philo);
		(void)((1) && (philo_eating(philo), print_status(philo, "is sleeping"),
				1));
		if (philo->init->time_to_sleep >= philo->init->time_to_die)
			usleep(philo->init->time_to_die * 1000);
		else
			usleep(philo->init->time_to_sleep * 1000);
		print_status(philo, "is thinking");
		if ((philo->init->number_of_philosophers % 2 != 0) && (philo->philo_num
				% 2 != 0))
			if (usleep(2000)
				&& philo->init->time_to_eat > philo->init->time_to_sleep)
				usleep((philo->init->time_to_eat - philo->init->time_to_sleep)
					* 1000);
	}
	return (NULL);
}
