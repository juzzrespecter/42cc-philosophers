/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_setup_shutdown.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 19:39:41 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:07:20 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*free_data(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	while (ph_info->forks && index < ph_info->ph_count)
	{
		pthread_mutex_destroy(&ph_info->forks[index]);
		pthread_mutex_destroy(&ph_info->crowd_ctrl[index]);
		index++;
	}
	free(ph_info->time_to_starve);
	free(ph_info->crowd_ctrl);
	free(ph_info->meals);
	free(ph_info->forks);
	free(ph_info->threads);
	free(ph_info->crowd_ctrl_id);
	pthread_mutex_destroy(&ph_info->lock);
	pthread_mutex_destroy(&ph_info->finish_lock);
	pthread_mutex_destroy(&ph_info->starve_lock);
	pthread_mutex_destroy(&ph_info->crowd_ctrl_start);
	free(ph_info);
	return (NULL);
}

static t_thread_info	*thread_info_shared_state(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	ph_info->time_to_starve = malloc(sizeof(long) * ph_info->ph_count);
	ph_info->meals = malloc(sizeof(int) * ph_info->ph_count);
	ph_info->forks = malloc(sizeof(pthread_mutex_t) * ph_info->ph_count);
	ph_info->threads = malloc(sizeof(pthread_t) * ph_info->ph_count);
	ph_info->crowd_ctrl = malloc(sizeof(pthread_mutex_t) * ph_info->ph_count);
	ph_info->crowd_ctrl_id = malloc(sizeof(int) * ph_info->ph_count);
	if (!ph_info->time_to_starve || !ph_info->forks || !ph_info->threads
		   || !ph_info->meals || !ph_info->crowd_ctrl)
		return (free_data(ph_info));
	while (index < ph_info->ph_count)
		pthread_mutex_init(&ph_info->crowd_ctrl[index++], NULL);
	pthread_mutex_init(&ph_info->crowd_ctrl_start, NULL);
	pthread_mutex_init(&ph_info->finish_lock, NULL);
	pthread_mutex_init(&ph_info->starve_lock, NULL);
	return (ph_info);
}

t_thread_info	*thread_info_setup(int argc, char **argv)
{
	t_thread_info	*ph_info;	
	int				index;

	index = 0;
	ph_info = malloc(sizeof(t_thread_info));
	if (!ph_info)
		return (NULL);
	memset(ph_info, 0, sizeof(t_thread_info));
	ph_info->ph_count = ft_atoi(argv[1]);
	if (!thread_info_shared_state(ph_info))
		return (NULL);
	memset(ph_info->time_to_starve, 0, ph_info->ph_count * sizeof(long));
	memset(ph_info->meals, 0, ph_info->ph_count * sizeof(int));
	memset(ph_info->crowd_ctrl_id, 0, ph_info->ph_count * sizeof(int));
	while (index < ph_info->ph_count)
		pthread_mutex_init(&ph_info->forks[index++], NULL);
	pthread_mutex_init(&ph_info->lock, 0);
	ph_info->time_to_die = ft_atoi(argv[2]);
	ph_info->time_to_eat = ft_atoi(argv[3]);
	ph_info->time_to_sleep = ft_atoi(argv[4]);
	ph_info->times_must_eat = -1;
	if (argc == 6)
		ph_info->times_must_eat = ft_atoi(argv[5]);
	ph_info->time_start = get_time();
	return (ph_info);
}
