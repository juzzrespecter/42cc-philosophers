#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>

# define THINK_ID 0
# define FORK_ID 1
# define EAT_ID 2
# define SLEEP_ID 3
# define DEAD_ID 4
# define FINISH_ID 5

typedef struct s_fork
{
	pthread_mutex_t	*fork_lock;
	int				taken;
}	t_fork;

typedef struct s_common
{
	pthread_t			*pthread_arr;
	t_fork				**fork_arr;
	pthread_mutex_t		*alive_lock;
	unsigned int		n_philo;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					alive_flag;
	int					times_must_eat;
}	t_common;

typedef struct s_philo
{
	t_common			*common;
	int					id;
	int					new_meal_flag;
	int					meals_eaten;
	pthread_mutex_t		*supervisor_lock;
	int					hands_id[2];
}	t_philo;

unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);

long			get_time(void);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_common *common);
void			*philo_routine(void *routine_args);
void			*supervisor_routine(void *routine_args);
void			print_status(int status_id, long time, int philo_id);
int				philo_checks_if_someone_died(t_philo *data);

int				philo_thinks(long time_start, t_philo *data);
int				philo_eats(long time_start, t_philo *data);
int				philo_checks_meal(long time_start, t_philo *data);
int				philo_sleeps(long time_start, t_philo *data);

#endif
