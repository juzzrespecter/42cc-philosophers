/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 16:22:11 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/09 17:47:34 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_id(void)
{
	static int	id_counter = -1;

	id_counter++;
	return (id_counter);
}

void	msg_lock(int status_id, int philo_id, t_thread_info *ph_info)
{
	pthread_mutex_lock(&ph_info->lock);
	print_status(status_id, get_time() - ph_info->time_start, philo_id);
	pthread_mutex_unlock(&ph_info->lock);
}

int	philo_thinks(int id, t_thread_info *ph_info)
{
	msg_lock(THINK_ID, id, ph_info);
	pthread_mutex_lock(&ph_info->waiter[id]);
	ph_info->waiter_state[id] = 0;
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

int	philo_eats(int id, t_thread_info *ph_info)
{
	ph_info->time_to_starve[id] = get_time();
	msg_lock(EAT_ID, id, ph_info);
	philo_waits(ph_info->time_to_eat);
	pthread_mutex_unlock(&ph_info->forks[id % ph_info->ph_count]);
	pthread_mutex_unlock(&ph_info->forks[(id + 1) % ph_info->ph_count]);
	ph_info->waiter_state[id] = 1 ;
	ph_info->meals[id] += (ph_info->times_must_eat != -1);
	if (ph_info->meals[id] == ph_info->times_must_eat)
	{
		pthread_mutex_lock(&ph_info->lock);
		ph_info->finished_meals++;
		pthread_mutex_unlock(&ph_info->lock);
	}
	return (ph_info->finish_flag);
}

int	philo_sleeps(int id, t_thread_info *ph_info)
{
	msg_lock(SLEEP_ID, id, ph_info);
	philo_waits(ph_info->time_to_sleep);
	return (ph_info->finish_flag);
}
