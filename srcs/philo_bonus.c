/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:30:19 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/04 20:35:23 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static t_data	*init_data(int argc, char **argv)
{
	t_data			*data;

	data = malloc(sizeof(t_data));
	data->n_philo = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->times_must_eat = -1;
	if (argc == 6)
		data->times_must_eat = ft_atoi(argv[5]);
	data->finished_meals = 0;
	data->dead_flag = 0;
	data->forks = sem_open("/forks", O_CREAT, 0600, data->n_philo);
	data->meals = sem_open("/meals", O_CREAT, 0600, data->n_philo);
	data->process_lock = sem_open("/process_lock", O_CREAT, 0600, 1);
	data->start_lock = sem_open("/start_lock", O_CREAT, 0600, data->n_philo);
	data->waiter_lock = sem_open("/waiter_lock", O_CREAT, 0600, data->n_philo / 2);
	data->pid_arr = malloc(sizeof(pid_t) * data->n_philo);
	return (data);
}

int	main(int argc, char **argv)
{
	sem_unlink("/start_lock");
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
