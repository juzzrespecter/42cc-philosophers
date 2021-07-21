#include "philosophers.h"

static int	print_err(int id)
{
	static const char *error_msg[] = {
		"invalid number of options provided to the program.",
		"invalid input for number_of_philosophers.",
		"invalid input for time_to_die.",
		"invalid input for time_to_eat.",
		"invalid input for time_to_sleep.",
		"invalid input for n_of_times_each_philosopher_must_eat."
	};

	printf("ERROR:\t%s\n", error_msg[id]);
	return (0);
}

static int	parse_input(char *input)
{
	int	index;

	index = 0;
	while (input[index] == ' ')
		index++;
	if (input[index] == '+')
		index++;
	while (input[index])
	{
		if (!ft_isdigit(input[index]))
			return (0);
		index++;
	}
	return (1);
}

int	philo_err_mgmt(int argc, char **argv)
{
	int	index;

	index = 1;
	if (argc < 5 || argc > 6)
		return (print_err(0));
	while (index < 5 || (index < 6 && argc == 6))
	{
		if (!parse_input(argv[index]))
			return (print_err(index));
		index++;
	}
	return (1);
}