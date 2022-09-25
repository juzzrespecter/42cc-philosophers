/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:42:21 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:20:48 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	philo_health_check(t_thread_info *ph_info)
{
	int		index;
	long	starve_time;

	index = 0;
	usleep(ph_info->time_to_die * 1000); /* bad */
	while (!m_read_finish_flag(ph_info))
	{
		pthread_mutex_lock(&ph_info->starve_lock);
		starve_time = get_time() - ph_info->time_to_starve[index];
		pthread_mutex_unlock(&ph_info->starve_lock);
		if (starve_time > ph_info->time_to_die)
		{
			pthread_mutex_lock(&ph_info->finish_lock);
			if (!ph_info->finish_flag)
			{
				pthread_mutex_lock(&ph_info->meal_lock);
				print_status(DEAD_ID, get_time() - ph_info->time_start, index);
				ph_info->finish_flag = 1;
				pthread_mutex_unlock(&ph_info->meal_lock);
			}
			pthread_mutex_unlock(&ph_info->finish_lock);
		}
		index = (index + 1) % ph_info->ph_count;
	}
}

/*
 * in order:
 * initialize crowd ctrl thread for turn management,
 * initialize a thread for each philosopher,
 * lastly inits metre thread for number of meals monitoring (if needed)
 */

static void	philo_init_threads(pthread_t *monitor_threads, t_thread_info *ph_info)
{
	int	index;

	index = 0;
	pthread_mutex_lock(&ph_info->crowd_ctrl_start);
	pthread_create(&monitor_threads[CROWD_CTRL], 0, crowd_ctrl_th, (void *)ph_info);
	while (index < ph_info->ph_count)
	{
		ph_info->time_to_starve[index] = ph_info->time_start;
		pthread_create(&ph_info->threads[index], 0, philo_th, (void *)ph_info);
		index++;
	}
	pthread_mutex_unlock(&ph_info->crowd_ctrl_start);
	if (ph_info->times_must_eat != -1)
	    pthread_create(&monitor_threads[METRE], 0, metre_th, (void *)ph_info);
}

static void philo_wait_ending_threads(pthread_t *monitor_threads, t_thread_info *ph_info)
{
    int idx;

    idx = 0;
    while (idx < ph_info->ph_count)
    {
	pthread_join(ph_info->threads[idx], NULL);
	printf("[END] joining thread (%d)...\n", idx+1);
	idx++;
    }
    if (ph_info->times_must_eat != -1)\
    {
	pthread_join(monitor_threads[METRE], NULL);
	printf("[END] joining metre...\n");
    }
    pthread_join(monitor_threads[CROWD_CTRL], NULL);
    printf("[END] joining crowd ctrl...\n");
}

int	main(int argc, char **argv)
{
	t_thread_info	*ph_info;
	pthread_t monitor_threads[2];

	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	ph_info = thread_info_setup(argc, argv);
	if (!ph_info)
		return (EXIT_FAILURE);
	philo_init_threads(monitor_threads, ph_info);
	philo_health_check(ph_info);
	philo_wait_ending_threads(monitor_threads, ph_info);
	free_data(ph_info);
	return (EXIT_SUCCESS);
}
