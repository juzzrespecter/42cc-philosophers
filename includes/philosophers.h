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

typedef struct	s_data
{
	pthread_t			*pthread_arr;
	pthread_mutex_t		*fork_arr;
	pthread_mutex_t		*alive_lock;
	unsigned int		n_philo;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					alive_flag;
	int					times_must_eat;
}				t_data;

typedef struct	s_philo
{
	t_data				*common; 
	int					id;
	int					fork_flags[2]; // para que suelten los tenedores en caso de fin de simulacion
	int					new_meal_flag;
	int					thread_ended_flag;
	pthread_mutex_t		*supervisor_lock; // read-write time_since_last_meal (to test)
	pthread_mutex_t		*forks[2];
}				t_philo;


unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);

long			get_time(void);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);
void			*philo_routine(void *routine_args);
void			*supervisor_routine(void *routine_args);
void			print_status(int status_id, long time, int philo_id);

#endif
