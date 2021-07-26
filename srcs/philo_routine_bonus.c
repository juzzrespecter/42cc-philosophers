#include "philosophers_bonus.h"

void	philo_routine_start()
{
	/* placeholder por si un proceso usa mas de un thread */
}

/* 
 * to_do
 *	pasarle ids distintos a cada proceso hijo
 *
 *
 */

void	philo_process_routine(int id, t_data *data)
{
	int		meals_eaten;
	long	time_start;

	meals_eaten = 0;
	time_start = get_time();
	while (meals_eaten < data->times_must_eat)
	{
		print_status(THINK_ID, get_time() - time_start, id);
		sem_wait(data->fork_pile);
		print_status(FORK_ID, get_time() - time_start, id);
		sem_wait(data->fork_pile);
		print_status(FORK_ID, get_time() - time_start, id);
		print_status(EAT_ID, get_time() - time_start, id);
		usleep(data->time_to_eat * 1000);
		sem_post(data->fork_pile);
		print_status(FORK_ID, get_time() - time_start, id);
		sem_post(data->fork_pile);
		print_status(FORK_ID, get_time() - time_start, id);
		print_status(SLEEP_ID, get_time() - time_start, id);
		usleep(data->time_to_sleep * 1000);
		meals_eaten++;
	}
}
