#include "philosophers.h"

int	philo_checks_if_someone_died(t_philo *data)
{
	int	thread_state;

	pthread_mutex_lock(data->common->alive_lock);
	thread_state = !(data->common->alive_flag);
	pthread_mutex_unlock(data->common->alive_lock);
	return (thread_state);
}

void	*philo_routine(void *routine_args)
{
	static	int	(*philo_action[])(long, t_philo *) = {
		philo_thinks,
		philo_eats,
		philo_checks_meal,
		philo_sleeps
	};
	int			status_id;
	int			status_ret;
	long		time_start;
	t_philo		*data;

	data = (t_philo *)routine_args;
	status_id = 0;
	time_start = get_time();
	while (1)
	{
		if (status_id > 3)
			status_id = 0;
		status_ret = philo_action[status_id](time_start, data);
		if (status_ret == 1)
			break ;
		status_id++;
	}
	return (NULL);
}
