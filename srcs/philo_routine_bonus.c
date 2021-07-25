#include "philosopher_bonus.h"

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

void	philo_routine(int id, t_data *data)
{
	int		meals_eaten;
	long	time_start;

	meals_eaten = 0;
	time_start = get_time();
	while (meals_eaten < data->times_must_eat)
	{
		print_status(THINK_ID, get_time() - time_start, id);
		sem_wait(fork_pile);
		sem_wait(fork_pile);
		print_status(EAT_ID, get_time() - time_start, id);
		sem_post(fork_pile);
		sem_post(fork_pile);
		print_status(SLEEP_ID, get_time() - time_start, id);
		meals_eaten++;
	}
}
