#include "philo.h"

int				ft_isdigit(int c)
{
	return ((c >= '0' && c <= '9'));
}

unsigned long	ft_atoul(char *a)
{
	unsigned long	ul;
	int				index;

	ul = 0;
	index = 0;
	while (a[index] == ' ')
		index++;
	index += (a[index] == '+');
	while (a[index] && ft_isdigit(a[index]))
	{
		ul = ul * 10 + (a[index] - '0') ;
		index++;
	}
	return (ul);
}
