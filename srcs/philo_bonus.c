#include "philosophers_bonus.h"

/* waitpid(-1, NULL, 0) == wait();
 *
 * if exit status of waitpid != id of process, matalo todo
 */
/*
 * force end of all processes
 * (a philo dies; end of sim.)
 *
 */ 

static void	close_threads(pid_t *pid_arr, t_data *data)
{
	unsigned int	count;

	count = 0;
	while (count < data->n_philo)
	{
		kill(pid_arr[count], SIGKILL); // pasa por pid de filosofo muerto!!
		count++;
	}
}

static void	init_threads_parent_waits(pid_t *pid_arr, t_data *data)
{
	pid_t	wait_ret;
	int		p_stat;

	wait_ret = waitpid(-1, &p_stat, 0);
	while (wait_ret != -1)
	{
		if (WEXITSTATUS(p_stat) == 0)
		{
			close_threads(pid_arr, data);
			break ;
		}
		wait_ret = waitpid(-1, &p_stat, 0);
	}
	free(pid_arr);
	free(data);
}

static void	init_threads(t_data *data)
{
	unsigned int	id;
	pid_t			*pid_arr;

	id = 0;
	pid_arr = malloc(sizeof(pid_t) * data->n_philo);
	data->time_start = get_time();
	data->time_last_meal = data->time_start;
	while (id < data->n_philo)
	{
		pid_arr[id] = fork();
		if (!pid_arr[id])
			philo_routine(id + 1, data);
		id++;
	}
	sem_unlink(data->sem_name);
	sem_unlink("status");
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
	data->meals_eaten = -2 * (data->times_must_eat == -1);
	data->sem_name = "/fork_pile";
	data->fork_pile = sem_open(data->sem_name, O_CREAT, 0600, data->n_philo);
	data->lock = sem_open("status", O_CREAT, 0600, 1);
	return (data);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
