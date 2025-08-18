/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_pars.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 14:52:46 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/18 15:46:12 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_simulation_end(t_prog_args *data)
{
	int	value;

	pthread_mutex_lock(&data->lock_check_simulation);
	value = data->is_simuation_ended;
	pthread_mutex_unlock(&data->lock_check_simulation);
	return (value);
}

void	error_printing(int i)
{
	if (i == SYNTAX_ERROR)
		write(1, ERROR_SYNTAX, ft_strlen(ERROR_SYNTAX));
	else if (i == INVALID_ARG)
		write(1, "ERROR : invalid arg\n", 20);
}

void	cleanup(t_philo_info **philo_infos, t_prog_args *init)
{
	int	i;

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

void	*init_p_args_continue(int *ac, char **av, t_prog_args *init, int i)
{
	if (*ac == 6)
	{
		init->n_to_eat = ft_atol(av[5]);
		if (init->n_to_eat == -1 || (init->n_to_eat == 0))
			return (free(init), NULL);
	}
	else
		init->n_to_eat = -1;
	init->is_simuation_ended = 0;
	pthread_mutex_init(&init->lock_check_simulation, NULL);
	init->fork_ = malloc((init->number_of_philosophers + 1)
			* sizeof(t_forks *));
	if (init->fork_ == NULL)
		return (free(init), NULL);
	while (i < init->number_of_philosophers)
	{
		init->fork_[i] = malloc(sizeof(t_forks));
		if (init->fork_[i] == NULL)
			return (free(init->fork_), NULL);
		pthread_mutex_init(&init->fork_[i]->fork, NULL);
		i++;
	}
	init->fork_[i] = NULL;
	return (init);
}

t_prog_args	*init_p_args(int *ac, char **av)
{
	t_prog_args	*init;
	t_prog_args	*init_;

	init = malloc(sizeof(t_prog_args));
	if (!init)
		return (NULL);
	init->start_time = current_time();
	init->dead_philo = 0;
	pthread_mutex_init(&init->meal_eaten, NULL);
	pthread_mutex_init(&init->last_meal_time_, NULL);
	init->number_of_philosophers = ft_atol(av[1]);
	if ((init->number_of_philosophers == -1)
		|| (init->number_of_philosophers == 0))
		return (free(init), NULL);
	init->time_to_sleep = ft_atol(av[4]);
	if ((init->time_to_sleep == -1) || (init->time_to_sleep == 0))
		return (free(init), NULL);
	init->time_to_die = ft_atol(av[2]);
	if ((init->time_to_die == -1) || (init->time_to_die == 0))
		return (free(init), NULL);
	init->time_to_eat = ft_atol(av[3]);
	if ((init->time_to_eat == -1) || (init->time_to_eat == 0))
		return (free(init), NULL);
	init_ = init_p_args_continue(ac, av, init, 0);
	return (init_);
}
