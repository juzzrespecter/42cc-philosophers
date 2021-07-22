#include "philosophers.h"

/*
 * philo_waits:
 * 		checks every 10 ms if simluation should end because
 * 		a philosopher dies.
 * 		returns when a philo. dies or routine finished 
 * 		its activity (eat, sleep)
 */

static bool	philo_waits(unsigned int time_to_wait, t_philo *philo_data)
{
	long	time_spent;
	long	time_start;
	
	time_spent = 0;
	time_start = get_time();
	while (time_spent < (time_to_wait - 10))
	{
		usleep(10 * 1000);
		time_spent = get_time() - time_start;
		if (is_there_a_corpse_on_the_table(philo_data))
			return (true);
	}
	if (time_spent < time_to_wait)
		usleep((time_to_wait - time_spent) * 1000);
	return (is_there_a_corpse_on_the_table(philo_data));
}	

static int	philo_takes_forks(...)
{
}	

static int	philo_eats(...)
{
}	

static int	philo_sleeps(...)
{
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
		// ---- take_forks ----
		pthread_mutex_lock(philo_data->first_fork);
		philo_data->fork_flags[0] = 1;
		if (check_corpse_on_table(philo_data) == 1)
			return (NULL);
		print_status(FORK_ID, get_time() - time_start, philo_data->id);
		pthread_mutex_lock(philo_data->second_fork);
		philo_data->fork_flags[1] = 1;
		if (check_corpse_on_table(philo_data) == 1)
			return (NULL);
		// ---- take_forks ----
		 
		// ---- philo_eats ----
		print_status(FORK_ID, get_time() - time_start, philo_data->id);
		pthread_mutex_lock(philo_data->monitor_lock);
		philo_data->meal_start_switch = 1; // mutex here??????????
		pthread_mutex_unlock(philo_data->monitor_lock);
		print_status(EAT_ID, get_time() - time_start, philo_data->id);
		if (philo_waits(philo_data, philo_data->common->time_to_eat) == 1)
			return (NULL);
		// ---- philo_eats ----
		 
		// ---- philo_sleeps ----
		pthread_mutex_unlock(philo_data->first_fork);
		pthread_mutex_unlock(philo_data->second_fork);
		print_status(SLEEP_ID, get_time() - time_start, philo_data->id);
		if (philo_waits(philo_data, philo_data->common->time_to_sleep) == 1)
			return (NULL);
		// ---- philo_sleeps ----

		if (philo_data->common->times_must_eat != -1)
			philo_data->number_of_meals++;
	}
	return (NULL);
}
