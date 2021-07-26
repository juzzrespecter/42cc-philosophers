#include "philosophers_bonus.h"

void	init_threads(t_data *data)
{
	unsigned int	id;
	unsigned int	count;
	pid_t			*pid_arr;

	id = 0;
	pid_arr = malloc(sizeof(pid_t) * data->n_philo);
	/* repite procesos creados: cambiar */
	while (id < data->n_philo)
	{
		pid_arr[id] = fork();
		if (!pid_arr[id])
			philo_process_routine(id + 1, data);
		id++;
	}
	count = 0;
	while (count < data->n_philo && waitpid(pid_arr[count], NULL, 0) > 0)
			count++;
	free(pid_arr);
}
