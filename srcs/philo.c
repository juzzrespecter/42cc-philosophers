#include "philosophers.h"

static t_fork	**fork_arr_setup(int n_philo)
{
	t_fork	**fork_arr;
	int		i;

	fork_arr = malloc(sizeof(t_fork *) * n_philo);
	i = 0;
	while (i < n_philo)
	{
		fork_arr[i] = malloc(sizeof(t_fork));
		fork_arr[i]->fork_lock = malloc(sizeof(pthread_mutex_lock));
		pthread_mutex_init(fork_arr[i]->fork_lock, 0);
		fork_arr[i]->taken = 0;
		i++;
	}
	return (fork_arr);
}

static t_common	*init_common(int argc, char **argv)
{
	t_common			*common;
	unsigned int	index;

	index = 0;
	common = malloc(sizeof(t_common));
	common->n_philo = ft_atou(argv[1]);
	common->time_to_die = ft_atou(argv[2]);
	common->time_to_eat = ft_atou(argv[3]);
	common->time_to_sleep = ft_atou(argv[4]);
	common->times_must_eat = -1;
	if (argc == 6)
		common->times_must_eat = ft_atou(argv[5]);
	common->alive_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(common->alive_lock, 0);
	common->pthread_arr = malloc(sizeof(pthread_t) * common->n_philo * 2);
	common->fork_arr = fork_arr_setup(common->n_philo);
	if (!common->alive_lock || !common->pthread_arr || !common->fork_arr)
		return (0);
	common->alive_flag = 1;
	return (common);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_common(argc, argv));
	return (EXIT_SUCCESS);
}
