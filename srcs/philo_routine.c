/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 18:26:22 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/09 17:51:18 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	crowd_ctrl_starts_new_turn(int index, t_thread_info *ph_info)
{
	int	count;

	count = 0;
	while (count < ph_info->ph_count / 2)
	{
		pthread_mutex_unlock(&ph_info->crowd_ctrl[index % ph_info->ph_count]);
		index += 2;
		count++;
	}
}

static void	crowd_ctrl_waits_for_turn_to_finish(int index, t_thread_info *ph_info)
{
	int	count;

	count = 0;
	while (count < ph_info->ph_count / 2)
	{
		if (ph_info->crowd_ctrl_state[index % ph_info->ph_count])
		{
			count += ph_info->crowd_ctrl_state[index % ph_info->ph_count];
			index += 2;
		}
		usleep(100);
	}
}

void	*crowd_ctrl_th(void *arg)
{
	t_thread_info	*ph_info;
	int				index;

	ph_info = (t_thread_info *)arg;
	index = 0;
	pthread_mutex_lock(&ph_info->crowd_ctrl_start);
	while (!ph_info->finish_flag)
	{
		crowd_ctrl_starts_new_turn(index, ph_info);
		crowd_ctrl_waits_for_turn_to_finish(index, ph_info);
		index = (index + 1) % ph_info->ph_count;
	}
	return (NULL);
}

void	*metre_th(void *metre_args)
{
	t_thread_info	*ph_info;

	ph_info = (t_thread_info *)metre_args;
	while (!ph_info->finish_flag)
	{
		if (ph_info->finished_meals == ph_info->ph_count)
		{
			pthread_mutex_lock(&ph_info->lock);
			if (!ph_info->finish_flag)
				print_status(FINISH_ID, get_time() - ph_info->time_start, -1);
			ph_info->finish_flag = 1;
			pthread_mutex_unlock(&ph_info->lock); // check this
		}
	}
	return (NULL);
}

void	*philo_th(void *args)
{
	static		int (*philo_status[])(int, t_thread_info *) = {
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
