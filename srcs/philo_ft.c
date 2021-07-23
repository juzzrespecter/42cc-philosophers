#include "philosophers.h"

long	get_time(void)
{
	struct timeval	time_s;
	long time;

	gettimeofday(&time_s, NULL);
	time = time_s.tv_sec * 1000 + (long)time_s.tv_usec / 1000;
	return (time);
}

void	print_status(int status_id, long time, int philo_id)
{
	static const char	*status_msg[] = {
		"is thinking.",
		"has taken a fork.",
		"\033[32mis eating.\033[0m",
		"\033[92mis sleeping.\033[0m",
		"\033[91mhas died.\033[0m",
		"stood up and left."
	};

	printf("%ld| %d %s\n", time, philo_id, status_msg[status_id]);
}

int	is_there_a_corpse_on_the_table(t_philo *philo_data)
{
	if (!philo_data->common->alive_flag)
	{
		if (philo_data->fork_flags[0])
			pthread_mutex_unlock(philo_data->first_fork);
		if (philo_data->fork_flags[1])
			pthread_mutex_unlock(philo_data->second_fork);
		return (1);
	}
	return (0);
}


int				ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

unsigned int	ft_atou(char *a)
{
	unsigned int	u;
	int				index;

	u = 0;
	index = 0;
	while (a[index] == ' ')
		index++;
	index += (a[index] == '+');
	while (a[index] && ft_isdigit(a[index]))
	{
		u = u * 10 + (a[index] - '0') ;
		index++;
	}
	return (u);
}
