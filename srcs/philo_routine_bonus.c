/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:32:32 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/02 22:17:42 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

void	kill_threads(pid_t *pid_arr, int n_philo, int current_pid)
{
	int	i;

	i = 0;
	while (i < n_philo)
	{
		if (i != current_pid)
			kill(pid_arr[i], SIGKILL);
		i++;
	}
}

static void	*supervisor_routine(void *routine_args)
{
	t_data	*data;
	long	time_start_meal;

	data = (t_data *)routine_args;
	while (1)
	{
		sem_wait(data->supervisor_lock);
		time_start_meal = get_time() - data->time_last_meal;
		sem_post(data->supervisor_lock);
		if (time_start_meal > data->time_to_die)
		{
			sem_wait(data->lock);
			print_status(DEAD_ID, get_time() - data->time_start, data->id);
			exit(data->id);
		}
	}
}

static void	print_lock(int status_id, t_data *data)
{
	sem_wait(data->lock);
	print_status(status_id, get_time() - data->time_start, data->id);
	sem_post(data->lock);
}

static void	philo_routine_sv_init(int id, t_data *data)
{
	data->supervisor_lock = sem_open("/lock", O_CREAT, 0600, 1);
	sem_unlink("/lock");
	data->id = id;
	pthread_create(&data->sv_thread, NULL, supervisor_routine, (void *)data);
	pthread_detach(data->sv_thread);
}	

void	philo_routine(int id, t_data *data)
{
	sem_wait(data->meals);
	sem_wait(data->start);
	philo_routine_sv_init(id, data);
	data->time_start = get_time();
	while (1)
	{
		print_lock(THINK_ID, data);
		sem_wait(data->waiter);
		sem_wait(data->forks);
		print_lock(FORK_ID, data);
		sem_wait(data->forks);
		print_lock(FORK_ID, data);
		sem_post(data->waiter);
		print_lock(EAT_ID, data);
		sem_wait(data->supervisor_lock);
		data->time_last_meal = get_time();
		data->finished_meals += (data->times_must_eat != -1);
		sem_post(data->supervisor_lock);
		usleep(data->time_to_eat * 1000);
		sem_post(data->forks);
		sem_post(data->forks);
		if (data->finished_meals == data->times_must_eat)
			sem_post(data->meals);
		print_lock(SLEEP_ID, data);
		usleep(data->time_to_sleep * 1000);
	}
	sem_close(data->forks);
	exit(data->id);
}
