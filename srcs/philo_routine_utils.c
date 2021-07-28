#include "philosophers.h"

static int	philo_takes_fork(int hand_id, t_philo *data)
{
	int	fork_state;

	while (1)
	{
		if (philo_checks_if_someone_died(data))
			return (1);
		pthread_mutex_lock(data->common->fork_arr[hand_id]->fork_lock);
		fork_state = data->common->fork_arr[hand_id]->taken;
		if (!fork_state)
			data->common->fork_arr[hand_id]->taken = 1;
		pthread_mutex_unlock(data->common->fork_arr[hand_id]->fork_lock);
		if (!fork_state)
			break ;
		usleep(1 * 1000);
	}
	print_status(FORK_ID, get_time() - data->common->time_start, data->id);
	return (0);
}

int	philo_thinks( t_philo *data)
{
	if (philo_checks_if_someone_died(data))
		return (1);
	print_status(THINK_ID, get_time() - data->common->time_start, data->id);
	if (philo_takes_fork(data->hands_id[0], data))
		return (1);
	return (philo_takes_fork(data->hands_id[1], data));
}

int	philo_eats(t_philo *data)
{
	if (philo_checks_if_someone_died(data))
		return (1);
	print_status(EAT_ID, get_time() - data->common->time_start, data->id);
	pthread_mutex_lock(data->supervisor_lock);
	data->time_new_meal = get_time();
	data->meals_eaten += (data->common->times_must_eat != -1);
	pthread_mutex_unlock(data->supervisor_lock);
	usleep(data->common->time_to_eat * 1000);
	pthread_mutex_lock(data->common->fork_arr[data->hands_id[0]]->fork_lock);
	data->common->fork_arr[data->hands_id[0]]->taken = 0;
	pthread_mutex_unlock(data->common->fork_arr[data->hands_id[0]]->fork_lock);
	pthread_mutex_lock(data->common->fork_arr[data->hands_id[1]]->fork_lock);
	data->common->fork_arr[data->hands_id[1]]->taken = 0;
	pthread_mutex_unlock(data->common->fork_arr[data->hands_id[1]]->fork_lock);
	return (0);
}

int	philo_checks_meal(t_philo *data)
{
	if (data->meals_eaten == data->common->times_must_eat)
	{
		if (philo_checks_if_someone_died(data))
			return (1);
		print_status(FINISH_ID, get_time() - data->common->time_start, data->id);
		return (1);
	}
	return (0);
}

int	philo_sleeps(t_philo *data)
{
	if (philo_checks_if_someone_died(data))
		return (1);
	print_status(SLEEP_ID, get_time() - data->common->time_start, data->id);
	usleep(data->common->time_to_sleep * 1000);
	return (0);
}
