#include "philosophers.h"

// common to both: check_corpse_on_table
// 		if true
// 			drop forks if case, exit

static void	print_status(int status_id, long time, int philo_id)
{
	static const char	*status_msg[] = {
		"is thinking.",
		"has taken a fork",
		"\033[32mis eating.\033[0m",
		"\033[92mis sleeping.\033[0m",
		"\033[91mhas died.\033[0m"
	};

	printf("%ld| %d %s\n", time, philo_id, status_msg[status_id]);
}

static int	check_corpse_on_table(t_philo *philo_data) // <<--- TO UTILS
{
	if (!philo_data->common->everyone_alive_flag)
	{
		if (philo_data->fork_flags[0])
			pthread_mutex_unlock(philo_data->first_fork);
		if (philo_data->fork_flags[1])
			pthread_mutex_unlock(philo_data->second_fork);
		return (1);
	}
	return (0);
}

/* monitor routine
* 		check every 10ms if (time_since_last_meal > time_to_die)
* 		keeps adding time to var
* 		if true
* 			print death, set up flag, exit
*/

void	*monitor_routine(void *routine_args)
{
	t_philo		*monitor_data;
	long		time_start;
	long		true_time_start;
	long		time_since_last_meal;

	monitor_data = (t_philo *)routine_args;
	time_start = get_time();
	true_time_start = time_start;
	while ((monitor_data->number_of_meals < monitor_data->common->times_must_eat) ||\
			(monitor_data->common->times_must_eat == -1)) // tmp until impl. times_must_eat
	{
		if (!monitor_data->common->everyone_alive_flag)
			return (NULL);
		pthread_mutex_lock(monitor_data->monitor_lock);
		if (monitor_data->meal_start_switch == 1)
		{
			time_start = get_time();
			monitor_data->meal_start_switch = 0;
		}
		pthread_mutex_unlock(monitor_data->monitor_lock); // check this
		time_since_last_meal = get_time() - time_start;
		if (time_since_last_meal > monitor_data->common->time_to_die)
		{
			print_status(DEAD_ID, get_time() - true_time_start, monitor_data->id);
			monitor_data->common->everyone_alive_flag = 0;
			return (NULL);
		}
		usleep(10 * 1000);
	}
	return (NULL);
}

// philo routine
// 		do philo shit: eat, sleep, think
// 		when 2 forks == true, set time_since_last_meal to 0
// 		print acts, always usleep with check_corpse

static int	philo_waits(t_philo *philo_data, unsigned int time_to_wait)
{
	long	time_spent;
	long	time_start;

	time_start = get_time();
	time_spent = get_time() - time_start;
	while (time_spent < (time_to_wait - 10))
	{
		//printf("\t%d| sp %ld, tw %u\n", philo_data->id, time_spent, time_to_wait);
		usleep(10 * 1000);
		time_spent = get_time() - time_start;
		if (check_corpse_on_table(philo_data))
			return (1);
	}
	if (time_spent < time_to_wait)
		usleep((time_to_wait - time_spent) * 1000);
	return (check_corpse_on_table(philo_data));
}

void	*philo_routine(void *routine_args)
{
	t_philo		*philo_data;
	long		time_start;

	philo_data = (t_philo *)routine_args;
	time_start = get_time();
	while ((philo_data->number_of_meals < philo_data->common->times_must_eat) ||\
			(philo_data->common->times_must_eat == -1)) // tmp until impl. times_must_eat
	{
		print_status(THINK_ID, get_time() - time_start, philo_data->id);
		pthread_mutex_lock(philo_data->first_fork);
		philo_data->fork_flags[0] = 1;
		if (check_corpse_on_table(philo_data) == 1)
			return (NULL);
		print_status(FORK_ID, get_time() - time_start, philo_data->id);
		pthread_mutex_lock(philo_data->second_fork);
		philo_data->fork_flags[1] = 1;
		if (check_corpse_on_table(philo_data) == 1)
			return (NULL);
		print_status(FORK_ID, get_time() - time_start, philo_data->id);
		pthread_mutex_lock(philo_data->monitor_lock);
		philo_data->meal_start_switch = 1; // mutex here??????????
		pthread_mutex_unlock(philo_data->monitor_lock);
		print_status(EAT_ID, get_time() - time_start, philo_data->id);
		if (philo_waits(philo_data, philo_data->common->time_to_eat) == 1)
			return (NULL);
		pthread_mutex_unlock(philo_data->first_fork);
		pthread_mutex_unlock(philo_data->second_fork);
		print_status(SLEEP_ID, get_time() - time_start, philo_data->id);
		if (philo_waits(philo_data, philo_data->common->time_to_sleep) == 1)
			return (NULL);
		if (philo_data->common->times_must_eat != -1)
			philo_data->number_of_meals++;
	}
	return (NULL);	
}
