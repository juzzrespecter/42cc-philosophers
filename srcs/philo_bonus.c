#include "philosophers_bonus.h"

static void	*metre_routine(void *metre_args)
{
	t_data	*data;

	data = (t_data *)metre_args;
	usleep(1);
	sem_wait(data->still_eating);
	// kill shit
	sem_wait(data->lock);
	printf("Placeholder: todos los filosofos han comido sus raciones\n");
	sem_post(data->lock);
	return (NULL);
}

static void	init_threads_parent_waits(pid_t *pid_arr, t_data *data)
{
	pthread_t	metre;
	int		p_stat;
	int		i;

	i = 0;
	if (data->times_must_eat != -1)
	{
		pthread_create(&metre, 0, metre_routine, (void *)data);
		pthread_detach(metre);
	}
	waitpid(-1, &p_stat, 0);
	while (i < data->n_philo)
	{
		kill(pid_arr[i], SIGSTOP);
		i++;
	}
	free(pid_arr);
	free(data);
}

static void	init_threads(t_data *data)
{
	int		id;
	pid_t	*pid_arr;

	if (!data)
		return ; 
	id = 0;
	pid_arr = malloc(sizeof(pid_t) * data->n_philo);
	data->time_start = get_time();
	data->time_last_meal = data->time_start;
	while (id < data->n_philo)
	{
		pid_arr[id] = fork();
		if (!pid_arr[id])
			philo_routine(id, data);
		id++;
	}
	sem_unlink("/still_eating");
	sem_unlink("/fork_pile");
	sem_unlink("/status");
	init_threads_parent_waits(pid_arr, data);
}

static t_data	*init_data(int argc, char **argv)
{
	t_data			*data;

	data = malloc(sizeof(t_data));
	data->n_philo = ft_atou(argv[1]);
	data->time_to_die = ft_atou(argv[2]);
	data->time_to_eat = ft_atou(argv[3]);
	data->time_to_sleep = ft_atou(argv[4]);
	data->times_must_eat = -1;
	if (argc == 6)
		data->times_must_eat = ft_atou(argv[5]);
	data->finished_meals = 0;
	data->fork_pile = sem_open("/fork_pile", O_CREAT, 0600, data->n_philo);
	data->still_eating = sem_open("/still_eating", O_CREAT, 0600, data->n_philo);
	data->lock = sem_open("/status", O_CREAT, 0600, 1);
	return (data);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
