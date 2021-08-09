/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftphilo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 17:43:54 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/09 17:44:41 by danrodri         ###   ########.fr       */
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
