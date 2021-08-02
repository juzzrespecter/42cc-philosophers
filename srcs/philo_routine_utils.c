/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:32:49 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/02 21:05:10 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	msg_lock(int status_id, t_philo *data)
{
	pthread_mutex_lock(data->lock);
	if (*(data->end_simulation_flag) == 0)
		print_status(status_id, get_time() - data->time_start, data->id);
	pthread_mutex_unlock(data->lock);
}

int	philo_checks_if_died(t_philo *data)
{
	int		thread_state;
	long	time_to_starve;

	thread_state = 0;
	time_to_starve = get_time() - data->time_to_starve;
	pthread_mutex_lock(data->lock);
	if (time_to_starve > data->time.time_to_die)
	{
		thread_state = 1;
		if (*(data->end_simulation_flag) == 0)
		{
			print_status(DEAD_ID, get_time() - data->time_start, data->id);
		}
		*(data->end_simulation_flag) = 1;
	}
	thread_state = *(data->end_simulation_flag);
	pthread_mutex_unlock(data->lock);
	return (thread_state);
}

int	philo_waits(long time_to_wait, t_philo *data)
{
	int		thread_state;
	long	time_start_waiting;
	long	time_waiting;

	thread_state = 0;
	time_start_waiting = get_time();
	time_waiting = get_time() - time_start_waiting;
	while (time_waiting < time_to_wait && !thread_state)
	{
		if (time_to_wait - time_waiting < 10)
			usleep((time_to_wait - time_waiting) * 1000);
		else
			usleep (10 * 1000);
		time_waiting = get_time() - time_start_waiting;
		thread_state = philo_checks_if_died(data);
	}
	return (thread_state);
}
