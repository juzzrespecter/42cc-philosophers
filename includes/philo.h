#ifndef PHILO_H
# define PHILO_H
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>

typedef struct	s_philo
{
	int				id;
	unsigned int	time[3];
	pthread_t		*philo_thread;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*fork_right;
	//pthread_mutex_t	forks[2]; // 0 -> left, 1 -> right
}				t_philo;

typedef struct	s_philo_info
{
	unsigned int		n_philo;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	unsigned int		entepima;
	pthread_mutex_t		**forks_arr;
	pthread_t			**pthread_arr;
}				t_philo_info;

unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);
void			init_threads(t_philo_info *data);


#endif
