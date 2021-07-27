#include "philosophers.h"

static int	philo_takes_fork(int hand, t_philo *data)
{
	int	fork_status;

	while (1)
	{
		pthread_mutex_lock(data->common->fork_arr[data->hands_id[hand]]->fork_lock);
		fork_status = data->common->fork_arr[data->hands_id[hand]]->taken;
		if (!fork_status)
		{
			data->common->fork_arr[data->hands_id[hand]]->taken = 1;
			pthread_mutex_unlock(data->common->fork_arr[data->hands_id[hand]]->fork_lock);
			return (1);
		}
		pthread_mutex_unlock(data->common->fork_arr[data->hands_id[hand]]->fork_lock);
		if (thread_checks_if_simulation_ended(data))
			break ;
	}
	return (0);
}

static int	philo_thinks(long time_start, t_philo *data)
{
	print_status(THINK_ID, get_time() - time_start, data->id);
	if (!philo_takes_fork(0, data))
		return (1);
	print_status(FORK_ID, get_time() - time_start, data->id);
	if (!philo_takes_fork(1, data))
		return (1);
	print_status(FORK_ID, get_time() - time_start, data->id);
	return (0);
}

static int	philo_eats(long time_start, t_philo *data)
{
	pthread_mutex_lock(data->supervisor_lock);
	data->new_meal_flag = 1;
	data->meals_eaten += (data->common->times_must_eat != -1);
	pthread_mutex_unlock(data->supervisor_lock);
	print_status(EAT_ID, get_time() - time_start, data->id);
	usleep(data->common->time_to_eat * 1000);
	pthread_mutex_lock(data->common->fork_arr[data->hands_id[0]]->fork_lock);
	data->common->fork_arr[data->hands_id[0]]->taken = 0;
	pthread_mutex_unlock(data->common->fork_arr[data->hands_id[0]]->fork_lock);
	pthread_mutex_lock(data->common->fork_arr[data->hands_id[1]]->fork_lock);
	data->common->fork_arr[data->hands_id[1]]->taken = 0;
	pthread_mutex_unlock(data->common->fork_arr[data->hands_id[1]]->fork_lock);
	return (thread_checks_if_simulation_ended(data));
}

int	thread_checks_if_simulation_ended(t_philo *data)
{
	int	thread_state;

	pthread_mutex_lock(data->supervisor_lock);
	thread_state = (data->meals_eaten == data->common->times_must_eat);
	pthread_mutex_unlock(data->supervisor_lock);
	pthread_mutex_lock(data->common->alive_lock);
	thread_state += !(data->common->alive_flag);
	pthread_mutex_unlock(data->common->alive_lock);
	return (thread_state);
}

void	*philo_routine(void *routine_args)
{
	t_philo	*data;
	long	time_start;

	data = (t_philo *)routine_args;
	time_start = get_time();
	while (1)
	{
		if (philo_thinks(time_start, data))
			break ;
		if (philo_eats(time_start, data))
			break ;
		print_status(SLEEP_ID, get_time() - time_start, data->id);
		usleep(data->common->time_to_sleep * 1000);
		if (thread_checks_if_simulation_ended(data))
			break ;
	}
	if (data->meals_eaten == data->common->times_must_eat)
		print_status(FINISH_ID, get_time() - time_start, data->id);
	return (NULL);
}
