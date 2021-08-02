/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_threads.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:32:16 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/02 21:42:06 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_of_philos)
	{
		pthread_mutex_destroy(data->forks[i]);
		free(data->philo_array[i]);
		free(data->forks[i]);
		i++;
	}
	free(data->philo_array);
	free(data->forks);
	free(data->threads);
	free(data->forks_state);
	pthread_mutex_destroy(data->lock);
	free(data->lock);
	free(data);
}

void	*metre(void *routine_args)
{
	t_data		*data;
	const char	*finish_msg;
	int			thread_state;

	finish_msg = "\033[96mall philosophers finished eating.\033[0m";
	data = (t_data *)routine_args;
	thread_state = 0;
	while (!thread_state)
	{
		pthread_mutex_lock(data->lock);
		thread_state = data->end_simulation_flag;
		if (!thread_state && data->finished_count == data->n_of_philos)
		{
			data->end_simulation_flag = 1;
			printf("%ld| %s\n", get_time() - data->time_start, finish_msg);
			thread_state = 1;
		}
		pthread_mutex_unlock(data->lock);
	}
	return (NULL);
}

t_philo	**init_philo_array(t_data *data)
{
	t_philo	**philos;
	int		index;

	philos = malloc(sizeof(t_philo) * data->n_of_philos);
	index = 0;
	while (index < data->n_of_philos)
	{
		philos[index] = malloc(sizeof(t_philo));
		philos[index]->time_start = data->time_start;
		philos[index]->lock = data->lock;
		philos[index]->forks = data->forks;
		philos[index]->forks_state = data->forks_state;
		philos[index]->time = data->time;
		philos[index]->end_simulation_flag = &data->end_simulation_flag;
		philos[index]->finished_count = &data->finished_count;
		philos[index]->time_to_starve = data->time_start;
		philos[index]->id = index;
		philos[index]->hands_id[(index % 2)] = (index + 1) % data->n_of_philos;
		philos[index]->hands_id[!(index % 2)] = index;
		philos[index]->finished_meals = 0;
		index++;
	}
	return (philos);
}

static void	init_threads_waits(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_of_philos)
	{
		pthread_join(data->threads[i], NULL);
		i++;
	}
}

void	init_threads(t_data *data)
{
	t_philo	**philos;
	int		i;

	if (!data)
		return ;
	data->time_start = get_time();
	philos = init_philo_array(data);
	if (!philos)
		return ;
	i = 0;
	data->philo_array = philos;
	while (i < data->n_of_philos)
	{
		pthread_create(&data->threads[i], 0, routine, (void *)philos[i]);
		usleep(10);
		i++;
	}
	sem_unlink("/start");
	if (data->time.times_must_eat != -1)
	{
		pthread_create(&data->metre, 0, metre, (void *)data);
		pthread_detach(data->metre);
	}
	init_threads_waits(data);
	clean_data(data);
}
