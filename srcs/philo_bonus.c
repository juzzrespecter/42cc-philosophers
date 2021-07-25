#include "philosophers_bonus.h"

static t_data	*init_data(int argc, char *argv)
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
	data->fork_pile = sem_open("Forks", O_CREAT, 0644, data->n_philo);
	return (data);
}

int	main(int argc, char **argv)
{
	t_bonus	data;

	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
