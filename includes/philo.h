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
	pthread_t		*philo_thread;
	int				id;
	int				fork_flags[2]; // para que los muertos suelten los tenedores
	unsigned int	time_since_last_meal;
	pthread_mutex_t			*first_fork;
	pthread_mutex_t			*second_fork;
	//pthread_mutex_t			*monitor_lock;
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

typedef struct	s_philo_data
{
	t_data *data;		// comun a todos los threads
	t_philo *philo;		// dependente de cada thread
}				t_philo_data;

void			philo_err_mgmt(int argc, char **argv);
unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);
void			init_threads(t_data *data);


#endif
