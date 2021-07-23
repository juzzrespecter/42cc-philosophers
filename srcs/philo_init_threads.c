#include "philosophers.h"

void	wait_and_exit_threads(t_philo **philo_arr, t_data *data)
{
	unsigned int	index;
	(void)	philo_arr;

	index = 0;
	while (index < (data->n_philo * 2))
	{
		pthread_join(data->pthread_arr[index], 0);
		index++;
	}
	/* dentro de exit func ??? */
	index = 0;
	while (index < data->n_philo)
	{
		pthread_mutex_destroy(&data->fork_arr[index]);
//		free(philo_arr[index]);
		index++;
	}
//	free(philo_arr);
	printf("OK!\n");
}

static t_philo	*philo_setup(unsigned int index, t_data *data)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	memset(philo, 0, sizeof(t_philo));
	philo->common = data;
	philo->id = index + 1;
	philo->supervisor_lock = malloc(sizeof(pthread_mutex_t));
	if (!(index % 2))
	{
//		printf("for %d | first fork: %d\n", index + 1, index);
		philo->first_fork = &data->fork_arr[index];
		philo->second_fork = &data->fork_arr[(index + 1) * (index + 1 < data->n_philo)];
	}
	else
	{
//		printf("for %d | first fork: %d\n", index + 1, (index + 1) * (index + 1 < data->n_philo));
		philo->first_fork = &data->fork_arr[(index + 1) * (index + 1 < data->n_philo)];
		philo->second_fork = &data->fork_arr[index];
	}
	return (philo);
}

void	init_threads(t_data *data)
{
	t_philo			**philo_arr;
	unsigned int	index;

	if (!data)
		return ;
	print_data(data); // rm
	philo_arr = malloc(sizeof(t_philo *) * data->n_philo);
	/* malloc ctrl */
	index = 0;
	while (index < data->n_philo) 
	{
		philo_arr[index] = philo_setup(index, data);	
		/* malloc ctrl */
		pthread_create(&data->pthread_arr[index], NULL, philo_routine, (void *)philo_arr[index]);
		pthread_create(&data->pthread_arr[index + data->n_philo], NULL, supervisor_routine, (void *)philo_arr[index]);	
		index++;
	}
	wait_and_exit_threads(philo_arr, data);
	// wait for threads to finish
}
