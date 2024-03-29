/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddantas- <ddantas-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 10:40:18 by ddantas-          #+#    #+#             */
/*   Updated: 2023/03/10 09:32:30 by ddantas-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	meal_handler(t_data *data, int philo, int check)
{
	long long	time;
	t_philo		*temp;

	temp = data->philo_lst;
	while (temp && temp->philo != philo)
		temp = temp->next;
	time = (timeinmilliseconds() - temp->init_time) - temp->last_ate;
	if (data->status == ALIVE && check > 0)
	{
		if (temp->last_ate > 0 && time > data->t_die)
		{
			usleep(data->t_die * 1000);
			print_action(data, temp, philo, DIED);
			return (1);
		}
		return (0);
	}
	if (data->status == ALIVE && time >= data->t_die)
	{
		print_action(data, temp, philo, DIED);
		return (1);
	}
	return (0);
}

int	philo_prep(t_data *data, int philo, int type, t_philo *phil)
{
	if (type == EATING)
		phil->last_ate = (timeinmilliseconds() - phil->init_time);
	if (type == EATING && data->status == ALIVE && data->t_die < data->t_eat)
	{
		usleep(data->t_die * 1000);
		put_fork_back(data, phil->philo);
		print_action(data, phil, philo, DIED);
		return (1);
	}
	else if (type == SLEEPING && data->status == ALIVE)
	{
		if (data->t_die >= data->t_eat + data->t_sleep)
			return (0);
		if (data->t_die - data->t_eat > 0)
			usleep((data->t_die - data->t_eat) * 1000);
		print_action(data, phil, philo, DIED);
		return (1);
	}
	return (0);
}

int	ate_enough(t_data *data, int current_philo, t_philo *phil)
{
	t_philo	*temp;

	if (data->status == DIED)
		return (1);
	if (data->must_eat < 0)
		return (0);
	phil->ate++;
	temp = data->philo_lst;
	while (temp)
	{
		if (temp->ate < data->must_eat)
			return (0);
		temp = temp->next;
	}
	put_fork_back(data, current_philo);
	data->status = DIED;
	return (1);
}
