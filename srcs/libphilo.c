/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libphilo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/17 16:44:48 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:11:23 by danrodri         ###   ########.fr       */
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
		index++;
	if (a[index] == '+')
		index++;
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

void	philo_wait(long	time_to_wait)
{
	long	time_start;

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
