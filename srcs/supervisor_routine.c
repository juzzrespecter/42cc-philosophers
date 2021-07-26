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
		if (thread_checks_if_simulation_ended(data))
			return (NULL);
		time_start_meal = supervisor_checks_meal(time_start_meal, data);
		if (get_time() - time_start_meal > data->common->time_to_die)
			supervisor_murders_philosopher(time_start, data); // change name
	}
}
