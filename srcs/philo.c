/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:42:21 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/08 21:41:47 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	*metre(void *metre_args)
{
	t_thread_info	*ph_info;

	ph_info = (t_thread_info *)metre_args;
	while (!ph_info->finish_flag)
	{
		if (ph_info->finished_meals == ph_info->ph_count)
		{
			pthread_mutex_lock(&ph_info->lock);
			if (!ph_info->finish_flag)
				print_status(FINISH_ID, get_time() - ph_info->time_start, -1);
			ph_info->finish_flag = 1;
			pthread_mutex_unlock(&ph_info->lock);
		}
	}
	return (NULL);
}

static void	philo_health_check(t_thread_info *ph_info)
{
	int	index;

	index = 0;
	usleep(ph_info->time_to_die * 1000);
	while (!ph_info->finish_flag)
	{
		if (get_time() - ph_info->time_to_starve[index] > ph_info->time_to_die)
		{
			pthread_mutex_lock(&ph_info->lock);
			if (!ph_info->finish_flag)
				print_status(DEAD_ID, get_time() - ph_info->time_start, index);
			ph_info->finish_flag = 1;
		}
		index = (index + 1) % ph_info->ph_count;
	}
}

/* test */

void	*waiter_th(void *arg)
{
	t_thread_info *ph = (t_thread_info *)arg;
	int		start_index = 0;
	int		lock_index = 0;
	int		move_index = 0;

	pthread_mutex_lock(&ph->waiter_start);
	/*while (!ph->finish_flag)
	{
		wait_for_turn_to_finish(index, ph);
		unlock_some(index, ph);
		index = !index;
	}*/
/*	while !flag
		unlock from index to floor[n / 2] ->index = index % n; index += 2
		wait for unlocked to finish
		index + 1 % n
	lock_index = 0;*/
	while (!ph_info->finish_flag)
	{
		/* 
		 * indice que lleva el inicio del array, aumenta 1 cada ciclo principal
		 * indice que se mueve a traves del array, aumenta 2 cada ciclo secundario
		 * indice que lleva la cuenta del numero de elementos chequeados, aumenta 1 cada ciclo secundario
		 */

		while (move_index < ph_info->ph_count / 2)
		{
			pthread_unlock(ph_info->waiter[(start_index + lock_index) % ph_info->ph_count]);
			lock_index += 2;
			move_index++;
		}
		lock_index = 0;
		move_index = 0;
		while (move_index < ph_info->ph_count / 2)
		{
			move_index += ph_info->waiter_state[(start_index + lock_index) % ph_info->ph_count];
			lock_index += 2;
		}
		lock_index = 0;
		move_index = 0;
		start_index = (start_index + 1) % ph_info->ph_count;
	}
	return (NULL);
}

void	philo_init_waiter(t_thread_info *ph_info)
{
	pthread_t	waiter;
	int		index;

	index = 0;
	pthread_mutex_lock(&ph_info->waiter_start);
	while (index < ph_info->ph_count)
		pthread_mutex_lock(ph_info->waiter[index++]);
	pthread_create(&waiter, 0, waiter_t, (void *)ph_info);
	pthread_detach(waiter);
}

/* ---- */

int	main(int argc, char **argv)
{
	t_thread_info	*ph_info;
	pthread_t		th_metre;
	int				index;

	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	ph_info = thread_info_setup(argc, argv);
	if (!ph_info)
		return (EXIT_FAILURE);
	philo_init_waiter(ph_info);
	/* philo init threads */
	/* philo init metre si procede */
	index = 0;
	while (index < ph_info->ph_count)
	{
		ph_info->time_to_starve[index] = ph_info->time_start;
		pthread_create(&ph_info->threads[index], 0, routine, (void *)ph_info);
		pthread_detach(ph_info->threads[index]);
		index++;
	}
	pthread_mutex_unlock(&ph_info->waiter_start); /* test */
	if (ph_info->times_must_eat != -1)
	{
		pthread_create(&th_metre, 0, metre, (void *)ph_info);
		pthread_detach(th_metre);
	}
	philo_health_check(ph_info);
	free_data(ph_info);
	return (EXIT_SUCCESS);
}
