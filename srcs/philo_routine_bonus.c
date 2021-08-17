/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 19:22:55 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:29:31 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	*supervisor(void *routine_args)
{
	long	time_last_meal;
	t_data	*data;

	data = (t_data *)routine_args;
	while (1)
	{
		time_last_meal = get_time() - data->time_last_meal;
		if (time_last_meal > data->time_to_die)
		{
			sem_wait(data->process_lock);
			print_status(DEAD_ID, get_time() - data->time_start, data->id);
			exit(data->id);
		}
		usleep(500);
	}
}

void	philosopher_process_starts(int id, t_data *data)
{
	pthread_t	supervisor_t;

	data->id = id;
	sem_wait(data->start_lock);
	pthread_create(&supervisor_t, 0, supervisor, (void *)data);
	pthread_detach(supervisor_t);
	while (1)
	{
		philosopher_thinks(data);
		philosopher_eats(data);
		philosopher_sleeps(data);
	}
	exit(EXIT_SUCCESS);
}
