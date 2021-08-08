/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 19:40:00 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/08 18:55:02 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *a)
{
	int	index;
	int	i;

	index = 0;
	i = 0;
	while (a[index] == ' ')
		i++;
	if (a[index] == '+')
		i++;
	while (a[index] >= '0' && a[index] <= '9')
	{
		i = (i * 10) + (a[index] - '0');
		index++;
	}
	return (i);
}

long	get_time(void)
{
	struct timeval	time_s;
	long			time;

	gettimeofday(&time_s, NULL);
	time = time_s.tv_sec * 1000 + (long)time_s.tv_usec / 1000;
	return (time);
}

void	philo_waits(long time_to_wait)
{
	long time_start;

	time_start = get_time();
	while (get_time() - time_start < time_to_wait)
		usleep(500);
}

void	print_status(int status_id, long timestamp, int philo_id)
{
	static const char	*status_msg[] = {
		"\033[33mis thinking.\033[0m",
		"has taken a fork.",
		"\033[32mis eating.\033[0m",
		"\033[92mis sleeping.\033[0m",
		"\033[91mhas died.\033[0m",
		"\033[96mall philosophers finished their meals.\033[0m"
	};

	if (philo_id == -1)
		printf("%ld| %s\n", timestamp, status_msg[status_id]);
	else
		printf("%ld| %d %s\n", timestamp, philo_id + 1, status_msg[status_id]);
}

void	*free_data(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	while (ph_info->forks && index < ph_info->ph_count)
	{
		pthread_mutex_destroy(&ph_info->forks[index]);
		index++;
	}
	free(ph_info->time_to_starve);
	free(ph_info->meals);
	//free(ph_info->forks);
	free(ph_info->threads);
	pthread_mutex_destroy(&ph_info->lock);
	free(ph_info);
	return (NULL);
}
