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

static int	input_parser(char *input)
{
	int	index;

	index = 0;
	while (input[index] == ' ')
		index++;
	if (input[index] == '+')
		index++;
	while (input[index] >= '0' && input[index] <= '9')
		index++;
	return (!input[index]);
}

static int	philo_err_mgmt(int argc, char **argv)
{
	static const char	*err_msg[] = {
		"invalid number of arguments provided to program.",
		"invalid input for number_of_philosophers.",
		"invalid input for time_to_die.",
		"invalid input for time_to_eat.",
		"invalid input for time_to_sleep.",
		"invalid input for number_of_times_philosopher_must_eat"
	};
	int			index;

	if (argc < 5 || argc > 6)
	{
		printf("\033[91m[ERROR]\033[0m\t%s\n", err_msg[0]);
		return (0);
	}
	index = 1;
	while (argv[index] && input_parser(argv[index]))
		index++;
	if (index != argc)
		printf("\033[91m[ERROR]\033[0m\t%s\n", err_msg[index]);
	return (index == argc);
}

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

void	wait_for_turn_to_finish(int turn_id, t_thread_info *ph)
{
	int	count = 0;

	count += !turn_id;
	while (count < ph->ph_count - (ph->ph_count % 2))
	{
		//printf("pos %d: state %d\n", count, ph->waiter_state[count]);
		count += (ph->waiter_state[count] * 2);
	}
	if ((ph->ph_count % 2) && !turn_id)
		while (!ph->waiter_state[ph->ph_count - 1]) {};
}

void	lock_all(t_thread_info *ph)
{
	int i = 0;

	while (i < ph->ph_count)
	{
		//printf("locking %d...\n", i);
		pthread_mutex_lock(&ph->waiter[i++]);
	}
}

void	unlock_some(int turn_id, t_thread_info *ph)
{
	int	id = turn_id;

	while (id < ph->ph_count - (ph->ph_count % 2))
	{
		//printf("unlocking %d...\n", turn_id);
		pthread_mutex_unlock(&ph->waiter[id]);
		id += 2;
	}
	if ((ph->ph_count % 2) && !turn_id)
		pthread_mutex_unlock(&ph->waiter[ph->ph_count - 1]);
}

void	*waiter_t(void *arg)
{
	t_thread_info *ph = (t_thread_info *)arg;
	int		index = 0;

	pthread_mutex_lock(&ph->waiter_start);
	/*while (!ph->finish_flag)
	{
		wait_for_turn_to_finish(index, ph);
		unlock_some(index, ph);
		index = !index;
	}*/
	while !flag
		unlock from index to floor[n / 2] ->index = index % n; index += 2
		wait for unlocked to finish
		index + 1 % n
	return (NULL);
}

void	test(t_thread_info *ph_info)
{
	pthread_t	waiter;

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
	index = 0;
	pthread_mutex_lock(&ph_info->waiter_start); /* test */
	lock_all(ph_info);
	test(ph_info); 							/* test */
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
