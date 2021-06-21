#include "philo.h"

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
