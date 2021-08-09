/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_err_mgmt.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/09 17:48:00 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/09 17:53:19 by danrodri         ###   ########.fr       */
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
