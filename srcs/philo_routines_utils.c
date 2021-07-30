#include "philosophers.h"

static int	philo_takes_fork(int hand_id, t_philo *data)
{
	int	fork_state;

	fork_state = 1;
	while (fork_state)
	{
		pthread_mutex_lock(data->common->forks[hand_id]);
		fork_state = data->common->forks_state[hand_id];
		if (!fork_state)
			data->common->forks_state[hand_id] = 1;
		pthread_mutex_unlock(data->common->forks[hand_id]);
		pthread_mutex_lock(data->common->thread_lock);
		fork_state = (fork_state && !data->common->end_simulation_flag);
		pthread_mutex_unlock(data->common->thread_lock);
	}
	return (print_status_mutex(FORK_ID, data));
}

int	philo_thinks(t_philo *data)
{
	if (print_status_mutex(THINK_ID, data))
		return (1);
	if (philo_takes_fork(data->hands_id[0], data))
		return (1);
	return (philo_takes_fork(data->hands_id[1], data));
}

int	philo_eats(t_philo *data)
{
	if (print_status_mutex(EAT_ID, data))
		return (1);
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
	if (print_status_mutex(SLEEP_ID, data))
		return (1);
	usleep(data->common->time_to_sleep * 1000);
	return (0);
}

int	print_status_mutex(int status_id, t_philo *data)
{
	int	thread_state;

	pthread_mutex_lock(data->common->thread_lock);
	thread_state = data->common->end_simulation_flag;
	if (!thread_state)
		print_status(status_id, get_time() - data->common->time_start, data->id);
	pthread_mutex_unlock(data->common->thread_lock);
	return (thread_state);
}
