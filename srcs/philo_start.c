#include "philo.h"

/*
 *
 * loop
 *		take a fork
 *		eat with the fork
 *		drop the fork
 *		sleep
 *		think
 *		if (time_to_die == true)
 *			die
 *
 *
 */

void	rm_threads(t_philo_info *data)
{
	unsigned int	index = 0;
	// when a philo dies (thread returns) exit everything
	// pthread_join for waiting to die, pthread_detach for killing
	// pthread_mutex_destroy
	while (index < data->n_philo)
	{
		pthread_join(*(data->pthread_arr[index]), NULL);
		printf("\t\tRIP\n\n");
		index++;
	}
	printf("tot be\n");
}

static unsigned long	philo_time_get(void)
{
	struct timeval	time;

	gettimedat(&time, NULL);
}

void	*philo_routine(void *to_cast)
{
	t_philo *philo = (t_philo *)to_cast; // tmp
	struct timeval	time;

	while (1 /* here opt cond */)
	{
		gettimeofday(&time, NULL);
		printf("%d|\t\033[32m%d is thinking\033[0m\n", time.tv_usec / 1000, philo->id);
		gettimeofday(&time, NULL);
		pthread_mutex_lock(philo->fork_left);
		gettimeofday(&time, NULL);
		printf("%d|\t%d has taken a fork\n", time.tv_usec / 1000, philo->id);
		pthread_mutex_lock(philo->fork_right);
		gettimeofday(&time, NULL);
		printf("%d|\t%d has taken a fork\n", time.tv_usec / 1000, philo->id);
		gettimeofday(&time, NULL);
		printf("%d|\t%d is eating\n", time.tv_usec / 1000, philo->id);
		usleep(philo->time[1] * 1000);
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		gettimeofday(&time, NULL);
		printf("%d|\t%d is sleeping\n", time.tv_usec / 1000, philo->id);
		usleep(philo->time[2] * 1000);
	//	printf("%d|\t\033[91m%d has died\033[0m\n", time.tv_usec / 1000, philo->id);
		//return (NULL);
	}
	return (NULL);

}

void	init_threads(t_philo_info *data)
{
	unsigned int	index;
	t_philo	**philo_arr;

	index = 0;
	philo_arr = (t_philo **)malloc(sizeof(t_philo *) * data->n_philo);
	while (index < data->n_philo)
	{
		philo_arr[index] = (t_philo *)malloc(sizeof(t_philo));
		philo_arr[index]->id = index + 1;
		philo_arr[index]->time[0] = data->time_to_die;
		philo_arr[index]->time[1] = data->time_to_eat;
		philo_arr[index]->time[2] = data->time_to_sleep;
		philo_arr[index]->philo_thread = data->pthread_arr[index];
		philo_arr[index]->fork_left = data->forks_arr[index];
		if (index == (data->n_philo - 1))
			philo_arr[index]->fork_right = data->forks_arr[index];
		else
			philo_arr[index]->fork_right = data->forks_arr[index + 1];
		pthread_create(data->pthread_arr[index], NULL, &philo_routine, \
			(void *)philo_arr[index]);
		index++;
	}
	rm_threads(data);

}

