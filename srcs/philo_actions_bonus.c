/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:39:43 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:06:09 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	print_lock(int status_id, t_data *data)
{
	sem_wait(data->process_lock);
	print_status(status_id, get_time() - data->time_start, data->id);
	sem_post(data->process_lock);
}

void	philosopher_thinks(t_data *data)
{
	print_lock(THINK_ID, data);
	sem_wait(data->waiter);
	sem_wait(data->forks);
	print_lock(FORK_ID, data);
	sem_wait(data->forks);
	print_lock(FORK_ID, data);
}

void	philosopher_eats(t_data *data)
{
	print_lock(EAT_ID, data);
	data->time_last_meal = get_time();
	data->finished_meals += (data->times_must_eat != -1);
	philo_wait(data->time_to_eat);
	sem_post(data->waiter);
	sem_post(data->forks);
	sem_post(data->forks);
	if (data->finished_meals == data->times_must_eat)
		sem_post(data->meals);
}

void	philosopher_sleeps(t_data *data)
{
	print_lock(SLEEP_ID, data);
	philo_wait(data->time_to_sleep);
}
