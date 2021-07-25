#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <sys/time.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <semaphore.h>

# define THINK_ID 0
# define FORK_ID 1
# define EAT_ID 2
# define SLEEP_ID 3
# define DEAD_ID 4
# define FINISH_ID 5

typedef struct s_data
{
	pthread_t		*pthread_arr;
	sem_t			*fork_pile;
	unsigned int	n_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				times_must_eat;
}	t_data;

unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);

long			get_time(void);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);

#endif
