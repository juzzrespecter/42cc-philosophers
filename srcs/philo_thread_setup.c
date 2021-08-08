/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_setup_shutdown.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 19:39:41 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/08 21:41:51 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_thread_info	*thread_info_shared_state(t_thread_info *ph_info)
{
	ph_info->time_to_starve = malloc(sizeof(long) * ph_info->ph_count);
	ph_info->meals = malloc(sizeof(int) * ph_info->ph_count);
	ph_info->forks = malloc(sizeof(pthread_mutex_t) * ph_info->ph_count);
	ph_info->threads = malloc(sizeof(pthread_t) * ph_info->ph_count);
	if (!ph_info->time_to_starve || !ph_info->forks || !ph_info->threads ||
		   	!ph_info->meals)
		return (free_data(ph_info));
	/* test */

	ph_info->waiter = malloc(sizeof(pthread_mutex_t) * ph_info->ph_count);
	ph_info->waiter_state = malloc(sizeof(int) * ph_info->ph_count);
	memset(ph_info->waiter_state, 1, sizeof(int) * ph_info->ph_count);
	int i = 0;
	while (i < ph_info->ph_count)
		pthread_mutex_init(&ph_info->waiter[i++], NULL);
	pthread_mutex_init(&ph_info->waiter_start, NULL);

	/* ---- */
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
