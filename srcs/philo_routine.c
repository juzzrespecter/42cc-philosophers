/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 18:26:22 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/08 21:41:49 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
 * mover acciones a actions.c
 *	mover aqui: metre y waiter (cambiar nombre a metre??)
 *
 *
 *
 */

static int	philo_thinks(int id, t_thread_info *ph_info)
{
	msg_lock(THINK_ID, id, ph_info);
	pthread_mutex_lock(&ph_info->waiter[id]);	/* test */
	ph_info->waiter_state[id] = 0;				/* test */
	pthread_mutex_lock(&ph_info->forks[(id + !(id % 2)) % ph_info->ph_count]);
	if (ph_info->finish_flag)
		return (1);
	msg_lock(FORK_ID, id, ph_info);
	pthread_mutex_lock(&ph_info->forks[(id + (id % 2)) % ph_info->ph_count]);
	if (ph_info->finish_flag)
		return (1);
	msg_lock(FORK_ID, id, ph_info);
	return (0);
}

static int	philo_eats(int id, t_thread_info *ph_info)
{
	ph_info->time_to_starve[id] = get_time();
	msg_lock(EAT_ID, id, ph_info);
	philo_waits(ph_info->time_to_eat);
	pthread_mutex_unlock(&ph_info->forks[id % ph_info->ph_count]);
	pthread_mutex_unlock(&ph_info->forks[(id + 1) % ph_info->ph_count]);
	ph_info->waiter_state[id] = 1 ; /* test */
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
	msg_lock(SLEEP_ID, id, ph_info);
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
	while (!thread_state)
	{
		thread_state = philo_status[status_id](id, ph_info);
		status_id = (status_id + 1) % 3;
	}
	return (NULL);
}
