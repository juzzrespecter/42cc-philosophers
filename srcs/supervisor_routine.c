#include "philosophers.h"

/* supervisor routine
* 		check every 10ms if (time_since_last_meal > time_to_die)
* 		keeps adding time to var
* 		if true
* 			print death, set up flag, exit
*/

void	*supervisor_routine(void *routine_args)
{
	t_philo		*supervisor_data;
	long		time_start_meal;
	long		time_start;
	long		time_since_last_meal;

	supervisor_data = (t_philo *)routine_args;
	time_start = get_time();
	time_start_meal = time_start;
	while ((supervisor_data->number_of_meals < supervisor_data->common->times_must_eat) ||\
			(supervisor_data->common->times_must_eat == -1)) // tmp until impl. times_must_eat
	{
		if (!supervisor_data->common->everyone_alive_flag)
			return (NULL);
		pthread_mutex_lock(supervisor_data->supervisor_lock);
		if (supervisor_data->meal_start_switch == 1)
		{
			time_start_meal = get_time();
			supervisor_data->meal_start_switch = 0;
		}
		pthread_mutex_unlock(supervisor_data->supervisor_lock); // check this
		time_since_last_meal = get_time() - time_start_meal;
		if (time_since_last_meal > supervisor_data->common->time_to_die)
		{
			print_status(DEAD_ID, get_time() - time_start, supervisor_data->id);
			supervisor_data->common->everyone_alive_flag = 0;
			return (NULL);
		}
		usleep(10 * 1000);
	}
	return (NULL);
}
