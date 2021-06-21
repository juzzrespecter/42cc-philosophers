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

static void	philo_err_mgmt(int argc, char **argv)
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

static t_philo_info	init_data(int argc, char **argv)
{
	unsigned int	index;
	t_philo_info	data;

	memset(&data, 0, sizeof(t_philo_info));
	data.n_philo = ft_atou(argv[1]);
	data.time_to_die = ft_atou(argv[2]);
	data.time_to_eat = ft_atou(argv[3]);
	data.time_to_sleep = ft_atou(argv[4]);
	if (argc == 6)
		data.entepima = ft_atou(argv[5]);
	else
		data.entepima = -1;
	data.forks_arr = (pthread_mutex_t **)malloc(data.n_philo * sizeof(pthread_mutex_t *));
	data.pthread_arr = (pthread_t **)malloc(data.n_philo * sizeof(pthread_t *));
	index = 0;
	while (index < data.n_philo)
	{
		data.forks_arr[index] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(data.forks_arr[index], NULL);
		data.pthread_arr[index] = (pthread_t *)malloc(sizeof(pthread_t));
		index++;
	}
	return (data);
}

int	main(int argc, char **argv)
{
	t_philo_info	data;

	philo_err_mgmt(argc, argv);
	data = init_data(argc, argv);
	//set_up_table(data);
	init_threads(&data);
	return (EXIT_SUCCESS);
}
