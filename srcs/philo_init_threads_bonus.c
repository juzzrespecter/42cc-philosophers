/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_init_threads_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 19:45:29 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/03 21:59:19 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

static void	close_and_free(t_data *data)
{
	sem_close(data->meals);
	sem_close(data->forks);
	sem_close(data->process_lock);
	sem_close(data->waiter);
	sem_close(data->start_lock);
	sem_close(data->finish_lock);
	free(data->pid_arr);
	free(data);
}

static void	*metre(void *metre_args)
{
	t_data	*data;
	char	*finish_msg;
	int		finished_meals;

	data = (t_data *)metre_args;
	finish_msg = "\033[96mall philosophers finished eating.\033[0m";
	finished_meals = 0;
	while (finished_meals < data->n_philo)
	{
		sem_wait(data->meals);
		finished_meals++;
		if (finished_meals != data->n_philo)
			sem_post(data->process_lock);
	}
	printf("%ld| %s\n", get_time() - data->time_start, finish_msg);
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
	sem_unlink("/waiter");
	sem_unlink("/process_lock");
	sem_unlink("/start_lock");
	sem_unlink("/finish_lock");
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
	sem_wait(data->finish_lock);
	data->time_start = get_time();
	while (id < data->n_philo)
	{
		data->pid_arr[id] = fork();
		if (!data->pid_arr[id])
			philosopher_process_starts(id, data);
		id++;
	}
	init_threads_parent_waits(data);
}
