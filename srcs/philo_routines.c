#include "philosophers.h"

void	*supervisor_routine(void *routine_args)
{
	t_philo		*data;
	t_common	*common;
	long		locked_time;

	data = (t_philo *)routine_args;
	common = data->common;
	while (1)
	{
		if (thread_checks_if_simulation_ended(data->common))
			break ;
		pthread_mutex_lock(data->supervisor_lock);
		locked_time = data->time_since_new_meal;
		pthread_mutex_unlock(data->supervisor_lock);
		if (get_time() - locked_time > common->time_to_die)
		{
			pthread_mutex_lock(common->thread_lock);
			if (data->common->end_simulation_flag == 0)
				print_status(DEAD_ID, get_time() - common->time_start, data->id);
			data->common->end_simulation_flag = 1;
			pthread_mutex_unlock(common->thread_lock);
		}
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
	pthread_t	sv_thread;
	int			status_id;
	int			status_ret;
	t_philo		*data;

	pthread_create(&sv_thread, 0, supervisor_routine, routine_args);
	pthread_detach(sv_thread);
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
