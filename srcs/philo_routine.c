/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 18:26:22 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/07 20:46:24 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// use detach

static int	get_id(void)
{
	static int	id_counter = -1;

	id_counter++;
	return (id_counter); 
}

static int	philo_thinks(int id, t_thread_info *ph_info)
{
	pthread_mutex_lock(&ph_info->lock);
	if (!ph_info->finish_flag)
		print_status(THINK_ID, get_time() - ph_info->time_start, id);
	pthread_mutex_unlock(&ph_info->lock);
	pthread_mutex_lock(&ph_info->forks[(id + !(id % 2)) % ph_info->ph_count]);
	ph_info->fork_state[(id + (id % 2)) % ph_info->ph_count] = 1;
	if (ph_info->finish_flag)
		return (1);
	print_status(FORK_ID, get_time() - ph_info->time_start, id);
	pthread_mutex_lock(&ph_info->forks[(id + (id % 2)) % ph_info->ph_count]);
	ph_info->fork_state[(id + !(id % 2)) % ph_info->ph_count] = 1;
	if (ph_info->finish_flag)
		return (1);
	print_status(FORK_ID, get_time() - ph_info->time_start, id);
	return (0);
}

static int	philo_eats(int id, t_thread_info *ph_info)
{
	pthread_mutex_lock(&ph_info->lock);
	ph_info->time_to_starve[id] = get_time();
	if (!ph_info->finish_flag)
		print_status(EAT_ID, get_time() - ph_info->time_start, id);
	pthread_mutex_unlock(&ph_info->lock);
	philo_waits(ph_info->time_to_eat);
	ph_info->fork_state[id] = 0;
	ph_info->fork_state[id + 1] = 0;
	pthread_mutex_unlock(&ph_info->forks[id % ph_info->ph_count]);
	pthread_mutex_unlock(&ph_info->forks[(id + 1) % ph_info->ph_count]);
	ph_info->meals[id] += (ph_info->times_must_eat != -1);
	if (ph_info->meals[id] == ph_info->times_must_eat)
	{
		pthread_mutex_lock(&ph_info->lock);
		ph_info->finished_meals++;
		pthread_mutex_unlock(&ph_info->lock);
	}
	return (ph_info->finish_flag);
}

static int	philo_sleeps(int id, t_thread_info *ph_info)
{
	pthread_mutex_lock(&ph_info->lock);
	if (!ph_info->finish_flag)
		print_status(SLEEP_ID, get_time() - ph_info->time_start, id);
	pthread_mutex_unlock(&ph_info->lock);
	philo_waits(ph_info->time_to_sleep);
	return (ph_info->finish_flag);
}

void	*routine(void *args)
{
	static			int (*philo_status[])(int, t_thread_info *) = {
		philo_thinks,
		philo_eats,
		philo_sleeps
	};
	t_thread_info	*ph_info;
	int				id;
	int				status_id;
	int				thread_state;

	ph_info = (t_thread_info *)args;
	pthread_mutex_lock(&ph_info->lock);
	id = get_id();
	pthread_mutex_unlock(&ph_info->lock);
	thread_state = 0;
	status_id = 0;
	if (!(id % 2))
		pthread_mutex_lock(&ph_info->start_lock); /* to test */
	while (!thread_state)
	{
		thread_state = philo_status[status_id](id, ph_info);
		status_id = (status_id + 1) % 3;
	}
	return (NULL);
}
