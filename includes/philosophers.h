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

typedef struct s_common
{
	long				time_start;
	pthread_t			*threads;
	pthread_mutex_t		*thread_lock;
	pthread_mutex_t		**forks;
	int					*forks_state;
	int					n_philo;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					times_must_eat;
	int					finished_meals_counter;
	int					end_simulation_flag;
}	t_common;

typedef struct s_philo
{
	t_common			*common;
	int					id;
	long				time_since_new_meal;
	int					finished_meals;
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
//int				thread_checks_if_simulation_ended(t_common *common);
int				print_status_mutex(int status_id, t_philo *data);

int				philo_thinks(t_philo *data);
int				philo_eats(t_philo *data);
int				philo_checks_meal(t_philo *data);
int				philo_sleeps(t_philo *data);

#endif
