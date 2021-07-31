#include "philosophers_bonus.h"

void		kill_threads(pid_t *pid_arr, int n_philo, int current_pid)
{
	int	i;

	i = 0;
	while (i < n_philo)
	{
		if (i != current_pid)
			kill(pid_arr[i], SIGKILL);
		i++;
	}
}

static void	*supervisor_routine(void *routine_args)
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
			sem_wait(data->process_lock);
			print_status(DEAD_ID, get_time() - data->time_start, data->id);
		//	kill_threads(data->pid_arr, data->n_philo, data->id);
			exit(data->id);
		}
	}
}

static void	print_lock(int status_id, t_data *data)
{
	sem_wait(data->process_lock);
	print_status(status_id, get_time() - data->time_start, data->id);
	sem_post(data->process_lock);
}

static void	philo_routine_sv_init(int id, t_data *data)
{
	data->supervisor_lock = sem_open("/lock", O_CREAT, 0600, 1);
	sem_unlink("/lock");
	data->id = id;
	pthread_create(&data->sv_thread, NULL, supervisor_routine, (void *)data);
	pthread_detach(data->sv_thread);
}	

void	philo_routine(int id, t_data *data)
{
	sem_wait(data->still_eating);
	philo_routine_sv_init(id, data);
	while (1)
	{
		print_lock(THINK_ID, data);
		sem_wait(data->fork_pile);
		print_lock(FORK_ID, data);
		sem_wait(data->fork_pile);
		print_lock(FORK_ID, data);
		print_lock(EAT_ID, data);
		sem_wait(data->supervisor_lock);
		data->time_last_meal = get_time();
		data->finished_meals += (data->times_must_eat != -1);
		sem_post(data->supervisor_lock);
		usleep(data->time_to_eat * 1000);
		sem_post(data->fork_pile);
		sem_post(data->fork_pile);
		if (data->finished_meals == data->times_must_eat)
			sem_post(data->still_eating);
		print_lock(SLEEP_ID, data);
		usleep(data->time_to_sleep * 1000);
	}
	sem_close(data->fork_pile);
	exit(data->id);
}
