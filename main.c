/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 23:40:43 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/18 15:41:32 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_status(t_philo_info *philo, char *status)
{
	long	current;

	pthread_mutex_lock(&philo->init->lock_check_simulation);
	if (philo->init->is_simuation_ended)
	{
		pthread_mutex_unlock(&philo->init->lock_check_simulation);
		return ;
	}
	current = current_time() - philo->init->start_time;
	pthread_mutex_lock(&philo->init->print_mutex);
	printf("%ld %d %s\n", current, philo->philo_num, status);
	pthread_mutex_unlock(&philo->init->print_mutex);
	pthread_mutex_unlock(&philo->init->lock_check_simulation);
}

void	*malloc_check(t_prog_args *init, t_philo_info **philo_infos)
{
	philo_infos = malloc((init->number_of_philosophers + 1)
			* sizeof(t_philo_info *));
	if (!philo_infos)
		return (NULL);
	return (philo_infos);
}

t_philo_info	**philos_data_filling(t_prog_args *init)
{
	t_philo_info	**philo_infos;
	int				j;

	j = 0;
	philo_infos = malloc_check(init, NULL);
	while (j < init->number_of_philosophers)
	{
		philo_infos[j] = malloc(sizeof(t_philo_info));
		if (!philo_infos[j])
			return (free(philo_infos), NULL);
		philo_infos[j]->philo_num = j + 1;
		philo_infos[j]->init = init;
		philo_infos[j]->meals_eaten = 0;
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
	return (philo_infos[j] = NULL, philo_infos);
}

int	philos_creation(t_prog_args *data)
{
	int	n_of_philo;
	int	j;

	j = 0;
	n_of_philo = data->number_of_philosophers;
	while (j < n_of_philo)
	{
		pthread_create(&data->philos[j]->philo_id, NULL, &start_routine,
			data->philos[j]);
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

int	main(int ac, char **av)
{
	t_philo_info	**philo_init;
	t_prog_args		*init;

	init = NULL;
	philo_init = NULL;
	if (ac < 5 || ac > 6)
		return (error_printing(SYNTAX_ERROR), 1);
	init = init_p_args(&ac, av);
	if (!init)
		return (error_printing(INVALID_ARG), 1);
	philo_init = philos_data_filling(init);
	if (philo_init == NULL)
		return (free(init), 1);
	init->philos = philo_init;
	if (philos_creation(init) == -1)
		return (cleanup(philo_init, init), 1);
	cleanup(philo_init, init);
	return (0);
}
