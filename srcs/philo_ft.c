#include "philosophers.h"

long	get_time(void)
{
	struct timeval	time_s;
	long			time;

	gettimeofday(&time_s, NULL);
	time = time_s.tv_sec * 1000 + (long)time_s.tv_usec / 1000;
	return (time);
}

void	print_status(int status_id, long time, int philo_id)
{
	static const char	*status_msg[] = {
		"\033[33mis thinking.\033[0m",
		"has taken a fork.",
		"\033[32mis eating.\033[0m",
		"\033[92mis sleeping.\033[0m",
		"\033[91mhas died.\033[0m"
	};

	printf("%ld| %d %s\n", time, philo_id + 1, status_msg[status_id]);
}

int	ft_isdigit(int c)
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
		u = u * 10 + (a[index] - '0');
		index++;
	}
	return (u);
}
