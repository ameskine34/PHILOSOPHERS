/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 09:42:15 by ameskine          #+#    #+#             */
/*   Updated: 2025/08/18 14:56:01 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	get_sign(char *str, int *i, int *error)
{
	int	sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*error = 1;
		(*i)++;
	}
	return (sign);
}

static void	skip_whitespace(char *str, int *i)
{
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
}

long	ft_atol(char *str)
{
	long	res;

	int (i), (sign), (digit), (error);
	(1) && (i = 0, error = 0);
	res = 0;
	skip_whitespace(str, &i);
	sign = get_sign(str, &i, &error);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			digit = str[i] - '0';
			if (sign > 0 && (res > ((LLONG_MAX - digit) / 10)))
				error = 1;
			if (sign < 0 && (res < ((LLONG_MIN + digit) / 10)))
				error = 1;
			res = res * 10 + digit;
		}
		else
			error = 1;
		i++;
	}
	if (error == 1)
		return (-1);
	return ((res * sign));
}

long	current_time(void)
{
	struct timeval	tp;
	long			total_time;

	gettimeofday(&tp, NULL);
	total_time = (tp.tv_sec * 1000) + (tp.tv_usec / 1000);
	return (total_time);
}
