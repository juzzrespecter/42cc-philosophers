#include "philosophers.h"

void	destroy_arr(void **arr, unsigned int arr_size)
{
	unsigned int	index;

	index = 0;
	while (index < arr_size)
	{
		free(arr[index]);
		index++;
	}
	free(arr);
}

t_pthread	**init_pthread_arr(unsigned int n_philo)
{
	unsigned int	index;
	t_pthread		**pthread_arr;

	index = 0;
	pthread_arr = (pthread **)malloc(sizeof(pthread_t *) * n_philo);
	while (index < n_philo)
	{
		pthread_arr[index] = (pthread *)malloc(sizeof(pthread_t));
		index++;
	}
	return (pthread_arr);
}

t_pthread_mutex	**init_fork_arr(unsigned int n_philo)
{
	unsigned int	index;
	t_phread_mutex	**fork_arr;

	index = 0;
	fork_arr = (t_pthread_mutex **)malloc(sizeof(pthread_mutex_t) * n_philo);
	while (index < n_philo)
	{
		pthread_mutex_init(fork_arr[index], NULL);
		index++;
	}
	return (fork_arr);
}

void	exit_threads()
{
}
// detach monitors????
// detach everything????

void	init_threads(t_data *data)
{
	// init phtread arr
	// init fork arr
	// init philosophers arr
	//












}
/*	unsigned int	index;
	t_pthread		**pthread_arr;
	t_pthread_mutex	**fork_arr;
	t_philo			**philo_arr;

	index = 0;
	philo_arr = (t_philo **)malloc(sizeof(t_philo *) * data->n_philo);
	while (index < data->n_philo)
	{
		philo_arr[index] = (t_philo *)malloc(sizeof(t_philo));
		philo_arr[index]->id = index + 1;
		philo_arr[index]->philo_thread = data->pthread_arr[index];
		philo_arr[index]->fork_left = data->forks_arr[index];
		if (index == (data->n_philo - 1))
			philo_arr[index]->fork_right = data->forks_arr[0];
		else
			philo_arr[index]->fork_right = data->forks_arr[index + 1];
		pthread_create(data->pthread_arr[index], NULL, &philo_routine, \
				(void *)philo_arr[index]);
		index++;
	}
	rm_threads(data, philo_arr);*/
