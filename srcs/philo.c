#include "philosophers.h"

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
	if (argc == 6)
		data->times_must_eat = ft_atou(argv[5]);
	else
		data->times_must_eat = -1;
	data->pthread_arr = malloc(sizeof(pthread_t) * data->n_philo * 2);
	data->fork_arr = malloc(sizeof(pthread_mutex_t) * data->n_philo);
	if (!data->pthread_arr || !data->fork_arr)
		return(NULL/* free func */);
	while (index < data->n_philo)
	{
		pthread_mutex_init(&data->fork_arr[index], 0);
		index++;
	}
	data->alive_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(data->alive_lock, 0);
	data->alive_flag = 1;
	return (data);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
