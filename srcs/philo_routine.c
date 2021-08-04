/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:28:55 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/04 21:54:41 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	philo_takes_fork(int hands_id[2], t_philo *data)
{
	int	thread_state;
	int	fork_state;

	fork_state = 1;
	thread_state = 0;
	while (fork_state == 1)
	{
		pthread_mutex_lock(data->forks[hands_id[0]]);
		fork_state = data->forks_state[hands_id[0]];
		pthread_mutex_lock(data->forks[hands_id[1]]);
		fork_state = (data->forks_state[hands_id[1]] || fork_state);
		if (fork_state == 0)
		{
			data->forks_state[hands_id[0]] = 1;
			data->forks_state[hands_id[1]] = 1;
		}
		pthread_mutex_unlock(data->forks[hands_id[1]]);
		pthread_mutex_unlock(data->forks[hands_id[0]]);
		thread_state = philo_checks_if_died(data);
		fork_state = (fork_state && !thread_state);
	}
	msg_lock(FORK_ID, data);
	msg_lock(FORK_ID, data);
	return (thread_state);
}

static int	philo_thinks(t_philo *data)
{
	int	thread_state;

	msg_lock(THINK_ID, data);
	thread_state = philo_takes_fork(data->hands_id, data);
	return (thread_state);
}

static int	philo_eats(t_philo *data)
{
	int	thread_state;

	msg_lock(EAT_ID, data);
	data->time_to_starve = get_time();
	data->finished_meals += !(data->time.times_must_eat == -1);
	thread_state = philo_waits(data->time.time_to_eat, data);
	pthread_mutex_lock(data->forks[data->hands_id[0]]);
	pthread_mutex_lock(data->forks[data->hands_id[1]]);
	data->forks_state[data->hands_id[0]] = 0;
	data->forks_state[data->hands_id[1]] = 0;
	pthread_mutex_unlock(data->forks[data->hands_id[0]]);
	pthread_mutex_unlock(data->forks[data->hands_id[1]]);
	if (data->finished_meals == data->time.times_must_eat)
	{
		pthread_mutex_lock(data->lock);
		*data->finished_count += 1;
		pthread_mutex_unlock(data->lock);
	}
	return (thread_state);
}

static int	philo_sleeps(t_philo *data)
{
	msg_lock(SLEEP_ID, data);
	return (philo_waits(data->time.time_to_sleep, data));
}

void	*routine(void *routine_args)
{
	static	int (*philo_status[])(t_philo *) = {
		philo_thinks,
		philo_eats,
		philo_sleeps
	};
	int			status_id;
	int			status_ret;
	t_philo		*data;

	status_id = 0;
	status_ret = 0;
	data = (t_philo *)routine_args;
	while (!status_ret)
	{
		status_ret = philo_status[status_id](data);
		status_id = (status_id + 1) % 3;
	}
	return (NULL);
}
