#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <semaphore.h>

typedef struct	s_data
{
	pthread_t	*pthread_arr;
	sem_t		*fork_pile;
	unsigned int	n_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				alive_flag /* ??????? */
	int				times_must_eat;
}				t_data;

#endif
