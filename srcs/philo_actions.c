/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_actions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 16:22:11 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 16:33:18 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_id(void)
{
	static int	id_counter = -1;

	id_counter++;
	return (id_counter);
}

static int ask_for_permission(int id, t_thread_info *ph_info)
{
	int	ticket;

	pthread_mutex_lock(&ph_info->crowd_ctrl[id]);
	ticket = ph_info->crowd_ctrl_id[id];
	pthread_mutex_unlock(&ph_info->crowd_ctrl[id]);
	if (!ticket)
		usleep(500);
	return ticket;
}

int	philo_thinks(int id, t_thread_info *ph_info)
{
	msg_lock(THINK_ID, id, ph_info); 
	while (ask_for_permission(id, ph_info)) { }
	pthread_mutex_lock(&ph_info->forks[FIRST_FORK]);
	if (finish_status(ph_info))
		return (1);
	msg_lock(FORK_ID, id, ph_info);
	pthread_mutex_lock(&ph_info->forks[SECOND_FORK]);
	if (finish_status(ph_info))
		return (1);
	msg_lock(FORK_ID, id, ph_info);
	return (0);
}

int	philo_eats(int id, t_thread_info *ph_info)
{
	pthread_mutex_lock(&ph_info->starve_lock);
	ph_info->time_to_starve[id] = get_time();
	pthread_mutex_unlock(&ph_info->starve_lock);
	msg_lock(EAT_ID, id, ph_info);
	philo_wait(ph_info->time_to_eat);
	pthread_mutex_unlock(&ph_info->forks[FIRST_FORK]);
	pthread_mutex_unlock(&ph_info->forks[SECOND_FORK]);
	ph_info->meals[id] += (ph_info->times_must_eat != -1);
	if (ph_info->meals[id] == ph_info->times_must_eat)
	{
		pthread_mutex_lock(&ph_info->lock);
		ph_info->finished_meals++;
		pthread_mutex_unlock(&ph_info->lock);
	}
	return (finish_status(ph_info));
}

int	philo_sleeps(int id, t_thread_info *ph_info)
{
	msg_lock(SLEEP_ID, id, ph_info);
	philo_wait(ph_info->time_to_sleep);
	return (finish_status(ph_info));
}
