#include "philosophers.h"

static int	philo_takes_fork(int hand_id, t_philo *data)
{
	int	fork_state;

	fork_state = 1;
	while (fork_state)
	{
		if (thread_checks_if_simulation_ended(data->common))
			return (1);
		pthread_mutex_lock(data->common->forks[hand_id]);
		fork_state = data->common->forks_state[hand_id];
		if (!fork_state)
			data->common->forks_state[hand_id] = 1;
		pthread_mutex_unlock(data->common->forks[hand_id]);
		if (!fork_state)
			break ;
	}
	print_status(FORK_ID, get_time() - data->common->time_start, data->id);
	return (0);
}

int	philo_thinks(t_philo *data)
{
	if (thread_checks_if_simulation_ended(data->common))
		return (1);
	print_status(THINK_ID, get_time() - data->common->time_start, data->id);
	if (philo_takes_fork(data->hands_id[0], data))
		return (1);
	return (philo_takes_fork(data->hands_id[1], data));
}

int	philo_eats(t_philo *data)
{
	if (thread_checks_if_simulation_ended(data->common))
		return (1);
	print_status(EAT_ID, get_time() - data->common->time_start, data->id);
	pthread_mutex_lock(data->supervisor_lock);
	data->time_since_new_meal = get_time();
	data->finished_meals += (data->common->times_must_eat != -1);
	pthread_mutex_unlock(data->supervisor_lock);
	usleep(data->common->time_to_eat * 1000);
	pthread_mutex_lock(data->common->forks[data->hands_id[0]]);
	data->common->forks_state[data->hands_id[0]] = 0;
	pthread_mutex_unlock(data->common->forks[data->hands_id[0]]);
	pthread_mutex_lock(data->common->forks[data->hands_id[1]]);
	data->common->forks_state[data->hands_id[1]] = 0;
	pthread_mutex_unlock(data->common->forks[data->hands_id[1]]);
	if (data->finished_meals == data->common->times_must_eat)
	{
		pthread_mutex_lock(data->common->thread_lock);
		data->common->finished_meals_counter++;
		pthread_mutex_unlock(data->common->thread_lock);
	}
	return (0);
}

int	philo_sleeps(t_philo *data)
{
	if (thread_checks_if_simulation_ended(data->common))
		return (1);
	print_status(SLEEP_ID, get_time() - data->common->time_start, data->id);
	usleep(data->common->time_to_sleep * 1000);
	return (0);
}

int	thread_checks_if_simulation_ended(t_common *common)
{
	int	thread_state;

	pthread_mutex_lock(common->thread_lock);
	thread_state = common->end_simulation_flag;
	pthread_mutex_unlock(common->thread_lock);
	return (thread_state);
}
