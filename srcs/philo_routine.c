#include "philosophers.h"

static int	is_there_a_corpse_on_the_table(t_philo *data)
{
	if (!data->common->alive_flag)
	{
		if (data->fork_flags[0])
			pthread_mutex_unlock(data->forks[0]);
		if (data->fork_flags[1])
			pthread_mutex_unlock(data->forks[1]);
		return (1);
	}
	return (0);
}

/*
 * philo_waits:
 * 		checks every 10 ms if simluation should end because
 * 		a philosopher dies.
 * 		returns when a philo. dies or routine finished 
 * 		its activity (eat, sleep)
 *		Returns 1 if simulation ends (a philo died),
 *		else returns 0.
 */

static int	philo_waits(unsigned int time_to_wait, t_philo *data)
{
	long	time_spent;
	long	time_start;

	time_spent = 0;
	time_start = get_time();
	while (time_spent < (time_to_wait - 10))
	{
		usleep(10 * 1000);
		time_spent = get_time() - time_start;
		if (is_there_a_corpse_on_the_table(data))
			return (0);
	}
	if (time_spent < time_to_wait)
		usleep((time_to_wait - time_spent) * 1000);
	return (!is_there_a_corpse_on_the_table(data));
}	

/*
 * philo_thinks:
 *	Thread interaction between philosophers,
 *	locks resources (forks) if available, locks thread
 *	if not available and waits for thread with locked
 *	shared resource to stop eating.
 *	Returns 0 if simulation ends (a philo died),
 *	else returns 1.
 */

static int	philo_thinks(long time_start, t_philo *data) 
{
	print_status(THINK_ID, get_time() - time_start, data->id);
	pthread_mutex_lock(data->forks[0]);
	data->fork_flags[0] = 1;
	if (is_there_a_corpse_on_the_table(data))
		return (0);
	print_status(FORK_ID, get_time() - time_start, data->id);
	pthread_mutex_lock(data->forks[1]);
	data->fork_flags[1] = 1;
	if (is_there_a_corpse_on_the_table(data))
		return (0);
	print_status(FORK_ID, get_time() - time_start, data->id);
	return (1);
}

/*
 * philo_eats:
 * 	Thread waits time specified in time_to_eat
 *	then unlocks forks mutex.
 *	Also turns on meal switch to signal supervisor philo	
 *	started a new meal, which resets time left to die to 0.
 *	As func. before, returns 0 if sim. ended, else 1.
 */

static int	philo_eats(long time_start, t_philo *data)
{
	pthread_mutex_lock(data->supervisor_lock);
	data->new_meal_flag = 1;
	pthread_mutex_unlock(data->supervisor_lock);
	print_status(EAT_ID, get_time() - time_start, data->id);
	if (!philo_waits(data->common->time_to_eat, data))
		return (0);
	pthread_mutex_unlock(data->forks[0]);
	data->fork_flags[0] = 0;
	pthread_mutex_unlock(data->forks[1]);
	data->fork_flags[1] = 0;
	return (1);
}	

void	*philo_routine(void *routine_args)
{
	t_philo	*data;
	int		meals_eaten;
	long	time_start;

	data = (t_philo *)routine_args;
	meals_eaten = 0;
	time_start = get_time();
	while ((meals_eaten < data->common->times_must_eat) ||\
			(data->common->times_must_eat == -1))
	{
		if (!philo_thinks(time_start, data))
			return (NULL);
		if (!philo_eats(time_start, data))
			return (NULL);
		print_status(SLEEP_ID, get_time() - time_start, data->id);
		if (!philo_waits(data->common->time_to_sleep, data))
			return (NULL);
		meals_eaten += (data->common->times_must_eat != -1);
	}
	pthread_mutex_lock(data->supervisor_lock);
	data->thread_ended_flag = 1;
	pthread_mutex_unlock(data->supervisor_lock);
	print_status(FINISH_ID, get_time() - time_start, data->id);
	return (NULL);
}
