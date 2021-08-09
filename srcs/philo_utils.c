/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 19:40:00 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/09 17:53:35 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_waits(long time_to_wait)
{
	long	time_start;

	time_start = get_time();
	while (get_time() - time_start < time_to_wait)
		usleep(500);
}

void	*free_data(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	while (ph_info->forks && index < ph_info->ph_count)
	{
		pthread_mutex_destroy(&ph_info->forks[index]);
		pthread_mutex_destroy(&ph_info->waiter[index]);
		index++;
	}
	free(ph_info->time_to_starve);
	free(ph_info->waiter);
	free(ph_info->waiter_state);
	free(ph_info->meals);
	free(ph_info->forks);
	free(ph_info->threads);
	pthread_mutex_destroy(&ph_info->lock);
	pthread_mutex_destroy(&ph_info->waiter_start);
	free(ph_info);
	return (NULL);
}
