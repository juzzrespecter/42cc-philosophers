/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:42:21 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/09 17:46:06 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_health_check(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	usleep(ph_info->time_to_die * 1000);
	while (!ph_info->finish_flag)
	{
		if (get_time() - ph_info->time_to_starve[index] > ph_info->time_to_die)
		{
			pthread_mutex_lock(&ph_info->lock);
			if (!ph_info->finish_flag)
				print_status(DEAD_ID, get_time() - ph_info->time_start, index);
			ph_info->finish_flag = 1;
		}
		index = (index + 1) % ph_info->ph_count;
	}
}

void	philo_init_waiter(t_thread_info *ph_info)
{
	pthread_t	waiter;
	int			index;

	index = 0;
	pthread_mutex_lock(&ph_info->waiter_start);
	while (index < ph_info->ph_count)
		pthread_mutex_lock(&ph_info->waiter[index++]);
	pthread_create(&waiter, 0, waiter_th, (void *)ph_info);
	pthread_detach(waiter);
}

void	philo_init_threads(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	while (index < ph_info->ph_count)
	{
		ph_info->time_to_starve[index] = ph_info->time_start;
		pthread_create(&ph_info->threads[index], 0, philo_th, (void *)ph_info);
		pthread_detach(ph_info->threads[index]);
		index++;
	}
	pthread_mutex_unlock(&ph_info->waiter_start);
}

void	philo_init_metre(t_thread_info *ph_info)
{
	pthread_t		metre;

	if (ph_info->times_must_eat != -1)
	{
		pthread_create(&metre, 0, metre_th, (void *)ph_info);
		pthread_detach(metre);
	}
}

int	main(int argc, char **argv)
{
	t_thread_info	*ph_info;

	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	ph_info = thread_info_setup(argc, argv);
	if (!ph_info)
		return (EXIT_FAILURE);
	philo_init_waiter(ph_info);
	philo_init_threads(ph_info);
	philo_init_metre(ph_info);
	philo_health_check(ph_info);
	free_data(ph_info);
	return (EXIT_SUCCESS);
}
