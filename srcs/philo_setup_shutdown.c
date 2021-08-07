/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_setup_shutdown.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 19:39:41 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/07 20:30:04 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_thread_info	*thread_info_shared_state(t_thread_info *ph_info)
{
	ph_info->time_to_starve = malloc(sizeof(long) * ph_info->ph_count);
	ph_info->meals = malloc(sizeof(int) * ph_info->ph_count);
	ph_info->fork_state = malloc(sizeof(int) * ph_info->ph_count);;
	ph_info->forks = malloc(sizeof(pthread_mutex_t) * ph_info->ph_count);
	ph_info->threads = malloc(sizeof(pthread_t) * ph_info->ph_count);
	if (!ph_info->time_to_starve || !ph_info->fork_state ||
			!ph_info->forks || !ph_info->threads || !ph_info->meals)
		return (free_data(ph_info));
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
	memset(ph_info->fork_state, 0, ph_info->ph_count * sizeof(int));
	while (index < ph_info->ph_count)
		pthread_mutex_init(&ph_info->forks[index++], 0); /* !! */
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

static void	unlock_forks(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	while (index < ph_info->ph_count)
	{
		if (ph_info->fork_state[index] == 1)
			pthread_mutex_unlock(&ph_info->forks[index]);
		index++;
	}
}

static void	wait_for_threads_to_finish(int ph_count, pthread_t *threads)
{
	int	index;

	index = 0;
	while (index < ph_count)
	{
		pthread_join(threads[index], 0);
		index++;
	}
}

void	philo_health_check(t_thread_info *ph_info)
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
			pthread_mutex_unlock(&ph_info->lock);
		}
		index = (index + 1) % ph_info->ph_count;
	}
	unlock_forks(ph_info);
	wait_for_threads_to_finish(ph_info->ph_count, ph_info->threads);
}
