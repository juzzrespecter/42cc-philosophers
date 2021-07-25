#include "philosophers.h"

static int		clean_data(t_data *data, t_philo **philo_arr)
{
	unsigned int	i;

	i = 0;
	while (i < data->n_philo)
	{
		pthread_mutex_destroy(&data->fork_arr[i]);
		free(philo_arr[i]);
		i++;
	}
	free(philo_arr);
	free(data->fork_arr);
	free(data->pthread_arr);
	free(data);
	return (0);
}

static t_philo	*philo_setup(unsigned int i, t_data *data)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	memset(philo, 0, sizeof(t_philo));
	philo->common = data;
	philo->id = i + 1;
	philo->supervisor_lock = malloc(sizeof(pthread_mutex_t));
	philo->forks[(i % 2)] = &data->fork_arr[(i + 1) * (i <= data->n_philo)]; 
	philo->forks[!(i % 2)] = &data->fork_arr[i];
	return (philo);
}

void	init_threads(t_data *data)
{
	t_philo			**philo_arr;
	unsigned int	i;

	if (!data)
		return ;
	philo_arr = malloc(sizeof(t_philo *) * data->n_philo);
	i = 0;
	while (i < data->n_philo) 
	{
		philo_arr[i] = philo_setup(i, data);	
		pthread_create(&data->pthread_arr[i],\
			   	0, philo_routine, (void *)philo_arr[i]);
		pthread_create(&data->pthread_arr[i + data->n_philo], \
				0, supervisor_routine, (void *)philo_arr[i]);	
		pthread_detach(data->pthread_arr[i + data->n_philo]);
		i++;
	}
	i = 0;
	while (i < data->n_philo)
	{
		pthread_join(data->pthread_arr[i], 0);
		i++;
	}
	clean_data(data, philo_arr);
}
