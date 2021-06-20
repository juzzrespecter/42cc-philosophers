#ifndef PHILO_H
# define PHILO_H
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>

typedef struct	s_philo_info
{
	unsigned long	n_of_philosophers;
	unsigned long	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	entepima;
}				t_philo_info;

unsigned long	ft_atoul(char *a);
int				ft_isdigit(int c);
void			set_up_table(t_philo_info data);

int	pthreads_test(t_philo_info data);  // tmp

#endif
