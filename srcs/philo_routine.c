#include "philosophers.h"

/*
 * philo_waits:
 * 		checks every 10 ms if simluation should end because
 * 		a philosopher dies.
 * 		returns when a philo. dies or routine finished 
 * 		its activity (eat, sleep)
 *		Returns 1 if simulation ends (a philo died),
 *		else returns 0.
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

/*
 * philo_takes_forks:
 *	Thread interaction between philosophers,
 *	locks resources (forks) if available, locks thread
 *	if not available and waits for thread with locked
 *	shared resource to stop eating.
 *	Returns 0 if simulation ends (a philo died),
 *	else returns 1.
 */

static int	philo_takes_forks(long time_start, t_philo *philo_data) 
{
	pthread_mutex_lock(philo_data->first_fork);
	philo_data->fork_flags[0] = 1;
	if (is_there_a_corpse_on_the_table(philo_data))
		return (0);
	print_status(FORK_ID, get_time() - time_start, philo_data->id);
	pthread_mutex_lock(philo_data->second_fork);
	philo_data->fork_flags[1] = 1;
	if (is_there_a_corpse_on_the_table(philo_data))
		return (0);
	print_status(FORK_ID, get_time() - time_start, philo_data->id);
	return (1);
}

/*static int	philo_takes_forks(long time_start, t_philo *philo_data)
{
	while (1) // ??
	{
		if (philo
	}
}*/

/*
 * philo_eats:
 * 	Thread waits time specified in time_to_eat
 *	then unlocks forks mutex.
 *	Also turns on meal switch to signal supervisor philo	
 *	started a new meal, which resets time left to die to 0.
 *	As func. before, returns 0 if sim. ended, else 1.
 */

static int	philo_eats(long time_start, t_philo *philo_data)
{
	pthread_mutex_lock(philo_data->supervisor_lock);
	philo_data->new_meal_flag = 1;
	pthread_mutex_unlock(philo_data->supervisor_lock);
	print_status(EAT_ID, get_time() - time_start, philo_data->id);
	if (philo_waits(philo_data->common->time_to_eat, philo_data))
		return (0);
	pthread_mutex_unlock(philo_data->first_fork);
	philo_data->fork_flags[0] = 0;
	pthread_mutex_unlock(philo_data->second_fork);
	philo_data->fork_flags[1] = 0;
	return (1);
}	

/*
 * philo_sleeps:
 * 	Prints status msg, waits, returns sim. status.
 */

static int	philo_sleeps(long time_start, t_philo *philo_data)
{
		print_status(SLEEP_ID, get_time() - time_start, philo_data->id);
		return (!philo_waits(philo_data->common->time_to_sleep, philo_data));
}	

void	*philo_routine(void *routine_args)
{
	t_philo		*philo_data;
	long		time_start;
	int			meals_eaten;

	philo_data = (t_philo *)routine_args;
	time_start = get_time();
	meals_eaten = 0;
	while ((meals_eaten < philo_data->common->times_must_eat) ||\
			(philo_data->common->times_must_eat == -1))
	{
		print_status(THINK_ID, get_time() - time_start, philo_data->id);
		if (!philo_takes_forks(time_start, philo_data))
			return (NULL);
		if (!philo_eats(time_start, philo_data))
			return (NULL);
		if (!philo_sleeps(time_start, philo_data))
			return (NULL);
		if (philo_data->common->times_must_eat != -1)
			meals_eaten++;
	}
	pthread_mutex_lock(philo_data->supervisor_lock);
	philo_data->thread_ended_flag = 1;
	pthread_mutex_unlock(philo_data->supervisor_lock);
	print_status(FINISH_ID, get_time() - time_start, philo_data->id);
	return (NULL);
}
