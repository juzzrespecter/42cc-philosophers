/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 17:42:21 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/07 20:08:18 by danrodri         ###   ########.fr       */
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
		printf("\033[92m[ERROR]\033[0m\t%s\n", err_msg[index]);
	return (index == argc);
}

static void	*metre(void *metre_args)
{
	t_thread_info	*ph_info;
	int				thread_state;
	char			*status;

	ph_info = (t_thread_info *)metre_args;
	thread_state = 0;
	status = "\033[96mall philosophers finished their meals.\033[0m";
	while (!thread_state)
	{
		if (ph_info->finished_meals == ph_info->ph_count)
		{
			pthread_mutex_lock(&ph_info->lock);
			if (!ph_info->finish_flag)
				printf("%ld| %s\n", get_time() - ph_info->time_start, status);
			ph_info->finish_flag = 1;
			pthread_mutex_unlock(&ph_info->lock);
		}
		thread_state = ph_info->finish_flag;
	}
	return (NULL);
}

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
	while (index < ph_info->ph_count)
	{
		ph_info->time_to_starve[index] = ph_info->time_start;
		pthread_create(&ph_info->threads[index], 0, routine, (void *)ph_info);
		index++;
	}
	if (ph_info->times_must_eat != -1)
	{
		pthread_create(&th_metre, 0, metre, (void *)ph_info);
		pthread_detach(th_metre);
	}
	philo_health_check(ph_info);
	free_data(ph_info);
	return (EXIT_SUCCESS);
}
