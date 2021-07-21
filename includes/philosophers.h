#ifndef PHILO_H
# define PHILO_H
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

typedef struct	s_data
{
	pthread_t			*pthread_arr;
	pthread_mutex_t		*fork_arr;
	unsigned int		n_philo;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					everyone_alive_flag;
	int					times_must_eat;
}				t_data;

typedef struct	s_philo
{
	t_data	*data; // comun a todos los threads---> cambiar a nombre mas intuitivo
				   // ----------- informacion especifica de cada thread
//	pthread_t		*philo_thread; why???
	int				id;
	int				fork_flags[2]; // para que los muertos suelten los tenedores
	int				number_of_meals;
	unsigned int	time_since_last_meal;
	pthread_mutex_t			*first_fork;
	pthread_mutex_t			*second_fork;
	//pthread_mutex_t			*monitor_lock; // read-write time_since_last_meal (to test)
}				t_philo;


unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);

long			get_time(void);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);
void			*monitor_routine(void *routine_args);
void			*philo_routine(void *routine_args);

// to delete

void print_data(t_data *data);


#endif