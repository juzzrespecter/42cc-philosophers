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
	t_data	*data; // comun a todos los threads
				   // ----------- informacion especifica de cada thread
//	pthread_t		*philo_thread; why???
	int				id;
	int				fork_flags[2]; // para que los muertos suelten los tenedores
	unsigned int	time_since_last_meal;
	unsigned int	number_of_meals;
	pthread_mutex_t			*first_fork;
	pthread_mutex_t			*second_fork;
	//pthread_mutex_t			*monitor_lock; // read-write time_since_last_meal (to test)
}				t_philo;

typedef struct	s_data
{
	unsigned int		n_philo;
	unsigned int		time_to_die;
	unsigned int		time_to_eat;
	unsigned int		time_to_sleep;
	int					everyone_alive_flag;
	int					entepima;
}				t_data;

unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);

int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);
void			*monitor_routine(void *routine_args);
void			*philosopher_routine(void *routine_args);


#endif
