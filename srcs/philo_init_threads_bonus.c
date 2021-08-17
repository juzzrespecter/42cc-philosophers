/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_threads_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 19:45:29 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:32:09 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	close_and_free(t_data *data)
{
	data->dead_flag = 1;
	sem_post(data->meals);
	sem_close(data->meals);
	sem_close(data->forks);
	sem_close(data->process_lock);
	sem_close(data->start_lock);
	sem_close(data->waiter);
	free(data->pid_arr);
	free(data);
}

static void	*metre(void *metre_args)
{
	t_data	*data;
	int		finished_meals;

	data = (t_data *)metre_args;
	finished_meals = 0;
	while (finished_meals < data->n_philo)
	{
		sem_wait(data->meals);
		finished_meals++;
		if (data->dead_flag == 1)
			return (NULL);
	}
	sem_wait(data->process_lock);
	print_status(FINISHED_ID, get_time() - data->time_start, -1);
	kill(data->pid_arr[0], SIGKILL);
	return (NULL);
}

static void	kill_threads(pid_t process_id, t_data *data)
{
	int	count;

	count = 0;
	while (count < data->n_philo)
	{
		if (count != WEXITSTATUS(process_id))
			kill(data->pid_arr[count], SIGKILL);
		count++;
	}
}

static void	init_threads_parent_waits(t_data *data)
{
	pthread_t	metre_t;
	pid_t		p_stat;
	int			count;

	sem_unlink("/forks");
	sem_unlink("/meals");
	sem_unlink("/process_lock");
	sem_unlink("/start_lock");
	sem_unlink("/waiter");
	count = 0;
	while (count < data->n_philo)
	{
		sem_wait(data->meals);
		sem_post(data->start_lock);
		count++;
	}
	if (data->times_must_eat != -1)
	{
		pthread_create(&metre_t, 0, metre, (void *)data);
		pthread_detach(metre_t);
	}
	waitpid(-1, &p_stat, 0);
	kill_threads(p_stat, data);
	close_and_free(data);
}

void	init_threads(t_data *data)
{
	int	id;
	int	count;

	if (!data)
		return ;
	id = 0;
	count = 0;
	while (count < data->n_philo)
	{
		sem_wait(data->start_lock);
		count++;
	}
	data->time_start = get_time();
	data->time_last_meal = data->time_start;
	while (id < data->n_philo)
	{
		data->pid_arr[id] = fork();
		if (!data->pid_arr[id])
			philosopher_process_starts(id, data);
		id++;
	}
	init_threads_parent_waits(data);
}
