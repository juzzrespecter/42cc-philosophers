#include "philosophers.h"

// common to both: check_corpse_on_the_table
// 		if true
// 			drop forks if case, exit

static int	check_corpse_on_the_table(t_data *data)
{
	if (data->everyone_alive_flag)
	{
		if (data->fork_flags[0])
			pthread_mutex_unlock(data->fist_fork);
		if (data->fork_flags[1])
			pthread_mutex_unlock(data->second_fork);
		return (1);
	}
	return (0);
}

// monitor routine
// 		check every 10ms if (time_since_last_meal > time_to_die)
// 		keeps adding time to var
// 		if true
// 			print death, set up flag, exit

void	*monitor_routine(void *routine_args)
{
	t_philo	*monitor_data;
	unsigned int	time_start;
	unsigned int	time_current;

	monitor_data = (t_philo *)routine_args;
	time_start = get_time();
	while (1) // tmp until impl. times_must_eat
	{
		if (monitor_data->everyone_alive_flag)
			return (NULL);
		time_current = get_time() - time_start;
		monitor_data->philo_data->time_since_last_meal += time_current;
		if (monitor_data->data->time_since_last_meal > monitor_data->time_to_die)
		{
			printf("%ld|\t\033[91m%d has died\033[0m.\n", get_time() - time_start, \
					monitor_data->id);
			monitor_data->everyone_alive_flag = 1;
			return (NULL);
		}
		usleep(10 * 1000);
	}
}

// philosopher routine
// 		do philosopher shit: eat, sleep, think
// 		when 2 forks == true, set time_since_last_meal to 0
// 		print acts, always usleep with check_corpse

static int	philosopher_waits(t_philo *data, unsigned int time_to_wait)
{
	unsigned int	time_spent;
	unsigned int	time_start;

	time_start = get_time();
	time_spent = get_time() - time_start;
	while (time_spent < (time_to_wait - 10))
	{
		usleep(10 * 1000);
		time_spent = get_time() - time_start;
		if (check_corpse_on_the_table(data))
			return (1);
	}

	return (data->everyone_alive_flag);
}

void	*philosopher_routine(void *routine_args)
{
	t_philo	*philosopher_data;
	unsigned int	time_start;

	philosopher_data = (t_philo *)routine_args;
	while (1) // tmp until etc.
	{
		printf("%ld|\t%d is thinking.\n", get_time() - time_start, \
					monitor_data->id);
		pthread_mutex_lock(philosopher_data->first_fork);
		philosopher_data->fork_flags[0] = 1;
		if (check_corpse_on_table(philosopher_data) == 1)
			return (NULL);
		printf("%ld|\t%d has taken a fork.\n", get_time() - time_start, \
					monitor_data->id);
		pthread_mutex_lock(philosopher_data->second_fork);
		philosopher_data->fork_flags[1] = 1;
		if (check_corpse_on_table(philosopher_data) == 1)
			return (NULL);
		printf("%ld|\t%d has taken a fork.\n", get_time() - time_start, \
					monitor_data->id);
		philosopher_data->time_since_last_meal = 0; // mutex here??????????
		printf("%ld|\t%d is eating.\n", get_time() - time_start, \
					monitor_data->id);
		if (philosopher_waits(philosopher_data, philosopher_data->time_to_eat) == 1)
			return (NULL);
		pthread_mutex_unlock(philosopher_data->first_fork);
		pthread_mutex_unlock(philosopher_data->second_fork);
		printf("%ld|\t%d is sleeping.\n", get_time() - time_start, \
					monitor_data->id);
		if (philosopher_waits(philosopher_data, philosopher_data->time_to_sleep) == 1);
			return (NULL);
	}
	

}
