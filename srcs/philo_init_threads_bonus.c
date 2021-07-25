#include "philosophers_bonus.h"

void	init_threads(t_data *data)
{
	unsigned int	id;

	id = 0;
	while (id < data->n_philo)
	{
		philo_pid = fork();
		if (!philo_pid)
			philo_routine(id, data);
		id++;
	}
	while (/* wait pid */)
	{
		...;
	}
}
