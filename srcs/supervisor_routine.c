#include "philosophers.h"

static long	supervisor_checks_meal(long time_start_meal, t_philo *data)
{
	pthread_mutex_lock(data->supervisor_lock);
	if (data->new_meal_flag == 1)
	{
		time_start_meal = get_time();
		data->new_meal_flag = 0;
	}
	pthread_mutex_unlock(data->supervisor_lock);
	return (time_start_meal);
}

/* supervisor routine
 * 		check every 10ms if (time_since_last_meal > time_to_die)
 * 		keeps adding time to var
 * 		if true
 * 			print death, set up flag, exit
 */

/*static int	supervisor_checks_thread_state(t_philo *data, int dead)
{
	int	thread_state;

	pthread_mutex_lock(data->supervisor_lock);
	thread_state = !(!data->thread_ended_flag && data->common->alive_flag);
	if (dead == 1)
		data->common->alive_flag = 0;
	pthread_mutex_unlock(data->supervisor_lock);
	pthread_mutex_unlock(data->common->alive_lock);
	return (thread_state);
}*/

/*
 * superv:
 * 	check if finished
 * 	check if new meal -> reset time and setup flag
 * 	check if dead -> print and setup flag
 *
 */

static int	supervisor_checks_state_of_thread(t_philo *data)
{
	int	thread_state;

	pthread_mutex_lock(data->supervisor_lock);
	thread_state = data->thread_ended_flag;
	pthread_mutex_unlock(data->supervisor_lock);
	pthread_mutex_lock(data->common->alive_lock);
	thread_state += !(data->common->alive_flag);
	pthread_mutex_unlock(data->common->alive_lock);
	return (thread_state);
}

static void	supervisor_murders_philosopher(long time_start, t_philo *data)
{
	pthread_mutex_lock(data->common->alive_lock);
	if (data->common->alive_flag == 1)
		print_status(DEAD_ID, get_time() - time_start, data->id);
	data->common->alive_flag = 0;
	pthread_mutex_unlock(data->common->alive_lock);
}

void	*supervisor_routine(void *routine_args)
{
	t_philo	*data;
	long	time_start_meal;
	long	time_start;

	data = (t_philo *)routine_args;
	time_start = get_time();
	time_start_meal = time_start;
	while (1)
	{
		if (supervisor_checks_state_of_thread(data))
			return (NULL);
		time_start_meal = supervisor_checks_meal(time_start_meal, data);
		if (get_time() - time_start_meal > data->common->time_to_die)
			supervisor_murders_philosopher(time_start, data); // change name
	}
}


/*void	*supervisor_routine(void *routine_args)
{
	t_philo		*data;
	long		time_start_meal;
	long		time_start;
	long		time_since_last_meal;

	data = (t_philo *)routine_args;
	time_start = get_time();
	time_start_meal = time_start;
	while (1)
	{
		if (supervisor_checks_thread_state(data, 0))
			return (NULL);
		time_start_meal = supervisor_checks_meal(time_start_meal, data);
		time_since_last_meal = get_time() - time_start_meal;
		if (time_since_last_meal > data->common->time_to_die)
		{
			pthread_mutex_lock(data->common->alive_lock);
			if (supervisor_checks_thread_state(data, 1))
				return (NULL);
			print_status(DEAD_ID, get_time() - time_start, data->id);
			pthread_mutex_lock(data->common->alive_lock);
			data->common->alive_flag = 0;
			pthread_mutex_unlock(data->common->alive_lock);
			return (NULL);
		}
		//usleep(10 * 1000);
	}
	return (NULL);
}*/
