#include "philosophers_bonus.h"

void	*supervisor_routine(void *routine_args)
{
	t_data *data;
	long	time_start_meal;

	data = (t_data *)routine_args;
	while (1)
	{
		sem_wait(data->supervisor_lock);
		time_start_meal = get_time() - data->time_last_meal;
		sem_post(data->supervisor_lock);
		if (time_start_meal > data->time_to_die)
		{
			sem_wait(data->lock);
			print_status(DEAD_ID, get_time() - data->time_start, data->id);
			exit(PHILO_DEAD);
		}
	}
}

void	print_lock(int status_id, long timestamp, int pid, sem_t *lock)
{
	sem_wait(lock);
	print_status(status_id, timestamp, pid);
	sem_post(lock);
}

void	philo_thinks(t_data *data)
{
	print_lock(THINK_ID, get_time() - data->time_start, data->id, data->lock);
	sem_wait(data->fork_pile);
	print_lock(FORK_ID, get_time() - data->time_start, data->id, data->lock);
	sem_wait(data->fork_pile);
	print_lock(FORK_ID, get_time() - data->time_start, data->id, data->lock);
}

void	philo_eats(t_data *data)
{
	print_lock(EAT_ID, get_time() - data->time_start, data->id, data->lock);
	sem_wait(data->supervisor_lock);
	data->time_last_meal = get_time();
	data->finished_meals += (data->times_must_eat != -1);
	sem_post(data->supervisor_lock);
	usleep(data->time_to_eat * 1000);
	sem_post(data->fork_pile);
	sem_post(data->fork_pile);
	if (data->finished_meals == data->times_must_eat)
	{
		sem_wait(data->lock);
		*data->finished_meals_counter += 1;
		sem_post(data->lock);
	}
	printf("eooo %d\n", *data->finished_meals_counter);
	sem_wait(data->lock);
	if (*data->finished_meals_counter == data->times_must_eat)
	{
		printf("chao\n");
		exit(0);
	}
	sem_post(data->lock);
}

int	philo_routine(int id, t_data *data)
{
	pthread_t	sv_thread;

	data->supervisor_lock = sem_open("/lock", O_CREAT, 0600, 1);
	sem_unlink("/lock");
	data->id = id;
	pthread_create(&sv_thread, NULL, supervisor_routine, (void *)data);
	pthread_detach(sv_thread);
	while (1)
	{
		philo_thinks(data);
		philo_eats(data);
		print_lock(SLEEP_ID, get_time() - data->time_start, data->id, data->lock);
		usleep(data->time_to_sleep * 1000);
	}
	sem_close(data->fork_pile);
	return (EXIT_SUCCESS);
}
