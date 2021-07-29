#include "philosophers.h"

static pthread_mutex_t	**forks_setup(int n_philo)
{
	pthread_mutex_t	**forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t *) * n_philo);
	i = 0;
	while (i < n_philo)
	{
		forks[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(forks[i], 0);
		i++;
	}
	return (forks);
}

static t_common	*init_common(int argc, char **argv)
{
	t_common	*common;

	common = malloc(sizeof(t_common));
	common->n_philo = ft_atou(argv[1]);
	common->time_to_die = ft_atou(argv[2]);
	common->time_to_eat = ft_atou(argv[3]);
	common->time_to_sleep = ft_atou(argv[4]);
	common->times_must_eat = -1;
	if (argc == 6)
		common->times_must_eat = ft_atou(argv[5]);
	common->thread_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(common->thread_lock, 0);
	common->threads = malloc(sizeof(pthread_t) * common->n_philo);
	common->forks = forks_setup(common->n_philo);
	common->forks_state = malloc(sizeof(int) * common->n_philo);
	if (!common->thread_lock || !common->threads || !common->forks)
		return (0);
	memset(common->forks_state, 0, sizeof(int) * common->n_philo);
	common->end_simulation_flag = 0;
	common->finished_meals_counter = 0;
	return (common);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_common(argc, argv));
	return (EXIT_SUCCESS);
}
