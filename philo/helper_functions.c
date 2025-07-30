/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ameskine <ameskine@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 09:42:15 by ameskine          #+#    #+#             */
/*   Updated: 2025/07/30 11:13:06 by ameskine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int ft_strlen(char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

static long num_len(long res)
{
	long i;

	i = 0;
	while (res != 0)
	{
		res = res / 10;
		i++;
	}
	return (i);
}

static int get_sign(char *str, int *i, int *error)
{
	int sign;

	sign = 1;
	if (str[*i] == '-' || str[*i] == '+')
	{
		if (str[*i] == '-')
			*error = 1;
		(*i)++;
	}
	return (sign);
}

static void skip_whitespace(char *str, int *i)
{
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		(*i)++;
}

long ft_atol(char *str, int *error)
{
	long res;

	int(i), (j), (sign);
	(1) && (i = 0, j = 0);
	res = 0;
	skip_whitespace(str, &i);
	sign = get_sign(str, &i, error);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			if (num_len(res) == 18)
			{
				if (sign == 1 && (res > (LLONG_MAX - (str[i] - '0')) / 10))
					*error = 1;
				else if (sign == -1 && (res > (LLONG_MIN + (str[i] - '0')) / 10))
					*error = 1;
			}
			res = res * 10 + (str[i] - '0');
		}
		else
			*error = 1;
		i++;
	}
	return ((res * sign));
}