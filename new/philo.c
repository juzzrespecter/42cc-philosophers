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
	static const char	*err_mgs[] = {
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
		printf("\033[92m[ERROR]\033[0m\t%s\n", err_msg[0]);
		return (0);
	}
	index = 1;
	while (argv[index] && input_parser(argv[index]))
		index++;
	if (index != argc)
		printf("\033[92m[ERROR]\033[0m\t%s\n", err_msg[index]);
	return (index == argc);
}

static void	philo_health_check(t_thread_info *ph_arr, t_common_info cmn)
{
	int	index;
	int	thread_state;

	index = 0;
	thread_state = 0;
	while (!thread_state)
	{
		if (ph_arr[index].time_to_starve > cmn.time_to_die)
		{
			pthread_mutex_lock(cmn.lock);
			if (!cmn.finish_flag)
				printf("%ld| all philosophers finished eating.\n",\
						get_time() - cmn.time_start);
			cmn.finish_flag = 1;
			pthread_mutex_unlock(cmn.lock);
		}
		thread_state = cmn.lock;
		index = (index + 1) % cmn.ph_count;
	}
	index = 0;
	while (index < cmn.ph_count)
	{
		pthread_mutex_unlock(&cmd.forks[index]);
		index++;
	}
}

int	main(int argc, char **argv)
{
	t_common_info	common;
	t_thread_info	*ph_array;
	int		index;

	if (!philo_err_mgmt(argc, argv))
		return (EXIT_FAILURE);
	common = common_info_setup(argc, argv);
	ph_array = thread_info_setup(common);
	if (!ph_array)
		return (EXIT_FAILURE);
	index = 0;
	while (index < common.ph_count)
	{
		pthread_create(&common.ph_thread[index], 0,\
			       	ph_routine, (void *)ph_array[index]);
		index++;
	}
	if (common.times_must_eat != -1)
	{
		pthread_create(&common.metre, 0, metre, (void *)&common);
		pthread_detach(common.metre);
	}
	philo_health_check(common);
	philo_unlock_and_wait(ph_array, common);
	return (EXIT_SUCCESS);
}
