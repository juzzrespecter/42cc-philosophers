/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_err_mgmt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 17:48:00 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:15:57 by danrodri         ###   ########.fr       */
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
	return (!input[index] && ft_atoi(input) > 0);
}

int	philo_err_mgmt(int argc, char **argv)
{
	static const char	*err_msg[] = {
		"invalid number of arguments provided to program.",
		"invalid input for number_of_philosophers.",
		"invalid input for time_to_die.",
		"invalid input for time_to_eat.",
		"invalid input for time_to_sleep.",
		"invalid input for number_of_times_philosopher_must_eat"
	};
	int					index;

	if (argc < 5 || argc > 6)
	{
		printf(RED"[ERROR]"FN"\t%s\n", err_msg[0]);
		return (0);
	}
	index = 1;
	while (argv[index] && input_parser(argv[index]))
		index++;
	if (index != argc)
		printf(RED"[ERROR]"FN"\t%s\n", err_msg[index]);
	return (index == argc);
}

void	msg_lock(int status_id, int philo_id, t_thread_info *ph_info)
{
    int th_stat;

    th_stat = m_read_finish_flag(ph_info); 
	pthread_mutex_lock(&ph_info->meal_lock);
	if (!th_stat)
	    print_status(status_id, get_time() - ph_info->time_start, philo_id);
	pthread_mutex_unlock(&ph_info->meal_lock);
}

int m_read_finish_flag(t_thread_info *ph_info)
{
	int	status;

	pthread_mutex_lock(&ph_info->finish_lock);
	status = ph_info->finish_flag;
	pthread_mutex_unlock(&ph_info->finish_lock);
	return status;
}

int m_read_finished_meals(t_thread_info *ph_info)
{
    int fin_meal_stat;

    pthread_mutex_lock(&ph_info->meal_lock);
    fin_meal_stat = ph_info->finished_meals;
    pthread_mutex_unlock(&ph_info->meal_lock);
    return fin_meal_stat;
}
