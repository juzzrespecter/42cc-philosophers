#include "philosophers.h"

void	*supervisor_routine(void *routine_args)
{
	t_philo		*data;
	int			thread_state;
	long		locked_time;

	thread_state = 0;
	data = (t_philo *)routine_args;
	while (!thread_state)
	{
		pthread_mutex_lock(data->supervisor_lock);
		locked_time = data->time_since_new_meal;
		pthread_mutex_unlock(data->supervisor_lock);
		pthread_mutex_lock(data->common->thread_lock);
		if (get_time() - locked_time > data->common->time_to_die)
		{
			if (data->common->end_simulation_flag == 0)
				print_status(DEAD_ID, get_time() - data->common->time_start, data->id);
			data->common->end_simulation_flag = 1;
		}
		thread_state = data->common->end_simulation_flag;
		pthread_mutex_unlock(data->common->thread_lock);
	}
	return (NULL);
}

void	*philo_routine(void *routine_args)
{
	static	int	(*philo_action[])(t_philo *) = {
		philo_thinks,
		philo_eats,
		philo_sleeps
	};
	int			status_id;
	int			status_ret;
	t_philo		*data;

	data = (t_philo *)routine_args;
	status_id = 0;
	status_ret = 0;
	while (!status_ret)
	{
		status_ret = philo_action[status_id](data);
		status_id = (status_id + 1) % 3;
	}
	return (NULL);
}
