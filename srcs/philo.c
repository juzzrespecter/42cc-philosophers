#include "philo.h"

static t_philo_info	init_data(int argc, char **argv)
{
	unsigned int	index;
	t_philo_info	data;

	memset(&data, 0, sizeof(t_philo_info));
	data.n_philo = ft_atou(argv[1]);
	data.time_to_die = ft_atou(argv[2]);
	data.time_to_eat = ft_atou(argv[3]);
	data.time_to_sleep = ft_atou(argv[4]);
	if (argc == 6)
		data.entepima = ft_atou(argv[5]);
	else
		data.entepima = -1;
/*	data.forks_arr = (pthread_mutex_t **)malloc(data.n_philo * sizeof(pthread_mutex_t *));
	data.pthread_arr = (pthread_t **)malloc(data.n_philo * sizeof(pthread_t *));
	index = 0;
	while (index < data.n_philo)
	{
		data.forks_arr[index] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(data.forks_arr[index], NULL);
		data.pthread_arr[index] = (pthread_t *)malloc(sizeof(pthread_t));
		index++;
	}*/
	return (data);
}

int	main(int argc, char **argv)
{
	t_philo_info	data;

	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	data = init_data(argc, argv);
	init_threads(&data);
	return (EXIT_SUCCESS);
}
