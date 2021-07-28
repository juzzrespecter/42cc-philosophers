#include "philosophers_bonus.h"

void	*supervisor_routine(void *routine_args)
{
	t_data *data;
	long	time_start;
	long	time_since_meal;

	data = (t_data *)routine_args;
	time_start = get_time();
	time_since_meal = 0;
	while (1)
	{
		sem_wait(data->supervisor_lock);
		if (data->meal_flag == 1)
		{
			time_start = get_time();
			data->meal_flag = 0;
		}
		sem_post(data->supervisor_lock);
		time_since_meal = get_time() - time_start;
		if (time_since_meal > data->time_to_die)
		{
			// 
			// wait here
			print_status(DEAD_ID, get_time() - time_start, data->id);
			// post here
			exit(PHILO_DEAD);
		}
	}
}

void	philo_routine(int id, t_data *data)
{
	pthread_t	s_thread;
	int			meals_eaten;
	long		time_start;

	sem_open(data->sem_name, 0);
	meals_eaten = -2 * (data->times_must_eat == -1);
	data->supervisor_lock = sem_open("lock", O_CREAT, 0644, 1);
	sem_unlink("lock");
	data->id = id;
	pthread_create(&s_thread, NULL, supervisor_routine, (void *)data);
	pthread_detach(s_thread);
	time_start = get_time();
	while (meals_eaten < data->times_must_eat)
	{
		print_status(THINK_ID, get_time() - time_start, id);
		sem_wait(data->fork_pile);
		print_status(FORK_ID, get_time() - time_start, id);
		sem_wait(data->fork_pile);
		print_status(FORK_ID, get_time() - time_start, id);
		print_status(EAT_ID, get_time() - time_start, id);
		sem_wait(data->supervisor_lock);
		data->meal_flag = 1;
		sem_post(data->supervisor_lock);
		usleep(data->time_to_eat * 1000);
		sem_post(data->fork_pile);
		sem_post(data->fork_pile);
		print_status(SLEEP_ID, get_time() - time_start, id);
		usleep(data->time_to_sleep * 1000);
		meals_eaten += (data->times_must_eat != -1);
	}
	print_status(FINISH_ID, get_time() - time_start, id);
	sem_close(data->fork_pile);
	exit(id);
}
