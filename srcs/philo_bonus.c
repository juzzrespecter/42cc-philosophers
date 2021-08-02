/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:30:19 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/02 22:15:48 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	*metre_routine(void *metre_args)
{
	t_data	*data;
	int		finished_meals;

	data = (t_data *)metre_args;
	finished_meals = 0;
	while (finished_meals < data->times_must_eat - 1)
	{
		while (sem_wait(data->meals))
		{
		}
		finished_meals++;
	}
	sem_wait(data->lock);
	printf("%ld| \033[96mall philosophers finished eating.\033[0m\n", \
			get_time() - data->time_start);
	kill_threads(data->pid_arr, data->n_philo, -1);
	sem_close(data->lock);
	return (NULL);
}

static void	init_threads_parent_waits(t_data *data)
{
	pthread_t	metre;
	pid_t		wait_ret;
	int			p_stat;
	int			i;

	i = 0;
	wait_ret = 0;
	if (data->times_must_eat != -1)
	{
		pthread_create(&metre, 0, metre_routine, (void *)data);
		pthread_detach(metre);
	}
	waitpid(-1, &p_stat, 0);
	kill_threads(data->pid_arr, data->n_philo, WEXITSTATUS(p_stat));
	free(data->pid_arr);
	free(data);
}

static void	init_threads(t_data *data)
{
	int		id;
	int		count = 0;

	if (!data)
		return ;
	id = 0;
	data->time_start = get_time();
	data->time_last_meal = data->time_start;
	while (count < data->n_philo)
	{
		sem_wait(data->start);
		count++;
	}
	while (id < data->n_philo)
	{
		data->pid_arr[id] = fork();
		if (!data->pid_arr[id])
			philo_routine(id, data);
		id++;
	}
	//usleep(100);
	count = 0;
	while (count < data->n_philo)
	{
		sem_post(data->start);
		count++;
	}
	sem_unlink("/meals");
	sem_unlink("/forks");
	sem_unlink("/status");
	sem_unlink("/waiter");
	sem_unlink("/start");
	init_threads_parent_waits(data);
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
	data->forks = sem_open("/forks", O_CREAT, 0600, data->n_philo);
	data->meals = sem_open("/meals", O_CREAT, 0600, data->n_philo);
	data->start = sem_open("/start", O_CREAT, 0600, data->n_philo);
	data->waiter = sem_open("/waiter", O_CREAT, 0600, data->n_philo / 2);
	data->lock = sem_open("/status", O_CREAT, 0600, 1);
	data->pid_arr = malloc(sizeof(pid_t) * data->n_philo);
	return (data);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
//	getchar();
	return (EXIT_SUCCESS);
}
