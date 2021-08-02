/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:30:01 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/02 21:08:58 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

pthread_mutex_t	**init_forks_array(int n_of_philos)
{
	pthread_mutex_t	**forks;
	int				count;

	forks = malloc(sizeof(pthread_mutex_t *) * n_of_philos);
	if (!forks)
		return (NULL);
	count = 0;
	while (count < n_of_philos)
	{
		forks[count] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(forks[count], 0);
		count++;
	}
	return (forks);
}

t_time	init_data_time(int argc, char **argv)
{
	t_time	time;

	time.time_to_die = ft_atou(argv[2]);
	time.time_to_eat = ft_atou(argv[3]);
	time.time_to_sleep = ft_atou(argv[4]);
	time.times_must_eat = -1;
	if (argc == 6)
		time.times_must_eat = ft_atou(argv[5]);
	return (time);
}

t_data	*init_data(int argc, char **argv)
{
	t_data		*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->n_of_philos = ft_atou(argv[1]);
	data->time = init_data_time(argc, argv);
	data->threads = malloc(sizeof(pthread_t) * data->n_of_philos);
	data->lock = malloc(sizeof(pthread_mutex_t));
	data->forks_state = malloc(sizeof(int) * data->n_of_philos);
	data->forks = init_forks_array(data->n_of_philos);
	if (!data->threads || !data->lock || !data->forks || !data->forks_state)
		return (NULL);
	memset(data->forks_state, 0, sizeof(int) * data->n_of_philos);
	pthread_mutex_init(data->lock, 0);
	data->end_simulation_flag = 0;
	data->finished_count = 0;
	return (data);
}

int	main(int argc, char **argv)
{
	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	init_threads(init_data(argc, argv));
	return (EXIT_SUCCESS);
}
