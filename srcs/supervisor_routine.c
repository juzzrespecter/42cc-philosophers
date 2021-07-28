#include "philosophers.h"

static void	supervisor_murders_philosopher(t_philo *data)
{
	pthread_mutex_lock(data->common->alive_lock);
	if (data->common->alive_flag == 1)
		print_status(DEAD_ID, get_time() - data->common->time_start, data->id);
	data->common->alive_flag = 0;
	pthread_mutex_unlock(data->common->alive_lock);
}

void	*supervisor_routine(void *routine_args)
{
	t_philo	*data;
	long	time_start_meal;
	int		thread_state;

	data = (t_philo *)routine_args;
	while (1)
	{
		thread_state = philo_checks_if_someone_died(data);
		pthread_mutex_lock(data->supervisor_lock);
		time_start_meal = data->time_new_meal;
		thread_state += (data->meals_eaten == data->common->times_must_eat);
		pthread_mutex_unlock(data->supervisor_lock);
		if (thread_state)
			return (NULL);
		if (get_time() - time_start_meal > data->common->time_to_die)
			supervisor_murders_philosopher(data);
		usleep(1 * 1000);
	}
}
