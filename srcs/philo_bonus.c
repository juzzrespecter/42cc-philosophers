#include "philosophers_bonus.h"


/* crear semaforo con nombre, abrir ese semaforo de forma independiente en cada nuevo proceso */

static t_data	*init_data(int argc, char **argv)
{
	t_data			*data;
	unsigned int	index;

	index = 0;
	data = malloc(sizeof(t_data));
	data->n_philo = ft_atou(argv[1]);
	data->time_to_die = ft_atou(argv[2]);
	data->time_to_eat = ft_atou(argv[3]);
	data->time_to_sleep = ft_atou(argv[4]);
	data->times_must_eat = -1;
	if (argc == 6)
		data->times_must_eat = ft_atou(argv[5]);
	/*sem shit */
	data->fork_pile = sem_open("/forks", O_CREAT, 0644, data->n_philo); /* creates named semaphore */
	return (data);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
