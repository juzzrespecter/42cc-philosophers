#ifndef PHILO_H
# define PHILO_H
# include <stdbool.h>
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

typedef struct	s_fork
{
	int				state;
	pthread_mutex_t	*fork;
}				t_fork;

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
	t_data	*common; // comun a todos los threads---> cambiar a nombre mas intuitivo
				   // ----------- informacion especifica de cada thread
//	pthread_t		*philo_thread; why???
	int				id;
	int				fork_flags[2]; // para que suelten los tenedores en caso de fin de simulacion
//	int				number_of_meals;
	int				new_meal_flag;
	int				thread_ended_flag;
	pthread_mutex_t			*supervisor_lock; // read-write time_since_last_meal (to test)
	pthread_mutex_t			*first_fork;
	pthread_mutex_t			*second_fork;
	t_fork			*left_fork;
	t_fork			*right_fork;
}				t_philo;


unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);

long			get_time(void);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);
void			*philo_routine(void *routine_args);
void			*supervisor_routine(void *routine_args);
int			is_there_a_corpse_on_the_table(t_philo *philo_data);
void			print_status(int status_id, long time, int philo_id);

#endif
