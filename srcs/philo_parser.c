#include "philo.h"

static void	print_and_exit(int id)
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
	exit(EXIT_FAILURE);
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

void	philo_err_mgmt(int argc, char **argv)
{
	int	index;

	index = 1;
	if (argc < 5 || argc > 6)
		print_and_exit(0);
	while (index < 5 || (index < 6 && argc == 6))
	{
		if (!parse_input(argv[index]))
			print_and_exit(index);
		index++;
	}
}
