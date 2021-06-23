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

void	rm_threads(t_philo_info *data, t_philo **philo_arr)
{
	unsigned int	index = 0;
	// when a philo dies (thread returns) exit everything
	// pthread_join for waiting to die, pthread_detach for killing
	// pthread_mutex_destroy
	while (index < data->n_philo)
	{
		pthread_join(*(data->pthread_arr[index]), NULL);
		index++;
	}
	index = 0;
	while (index < data->n_philo)
	{
		pthread_mutex_destroy(data->forks_arr[index]);
		free(philo_arr[index]);
		free(data->pthread_arr[index]);
		free(data->forks_arr[index]);
		index++;
	}
	free(philo_arr);
	free(data->pthread_arr);
	free(data->forks_arr);
	printf("tot be\n");
}

static long	get_time(void)
{
	struct timeval	time_s;
	long time;

	gettimeofday(&time_s, NULL);
	time = time_s.tv_sec * 1000 + (long)time_s.tv_usec / 1000;
	return (time);
}

// los filosofos pares son diestros, los impares son zurdos

/* ****************
 * philosopher_eats
 *
 * philosopher_sleep
 *
 * philosopher_thinks
 *
 * philosopher_dies
 * *****************/

// check_pulse -> usleep (10 ms * 1000)
// check_corpse_on_the_table -> mutex para check_var en data que se inicia si un filosofo muere	

/*int		check_corpse_on_the_table(...)
  {
// llamada despues de pillar tenedores
// comprueba si el filosofo comiendo tiene filosofos a su lado muertos
// si es verdad, desbloquea tenedor y sale
}*/

int		check_pulse(int	time_to_die, int time_since_last_meal)
{
	return (time_to_die - time_since_last_meal);
}

int		philosopher_waits(int time_to_wait, t_philo *philo)
{
	int	start_time;
	int	time_spent;

	start_time = get_time();
	time_spent = get_time() - start_time;
	while (time_spent < (time_to_wait - 10))
	{
		usleep(10 * 1000);
		time_spent = get_time() - start_time;
		philo->time_since_last_meal += time_spent;
		if (check_pulse(philo->time_to_die, philo->time_since_last_meal) < 0)
			return (0);
	}
	usleep((time_to_wait - time_spent) * 1000);
	time_spent = get_time() - start_time;
	philo->time_since_last_meal += start_time;
	return (check_pulse(philo->time_to_die, philo->time_since_last_meal));
}

int		philosopher_takes_first_fork(t_philo *philo)
{
	int		start_time;

	start_time = get_time();
	if (philo->id % 2)
		pthread_mutex_lock(philo->fork_right);
	else
		pthread_mutex_lock(philo->fork_left);
	philo->time_since_last_meal += get_time() - start_time;
	return (check_pulse(philo->time_to_die, philo->time_since_last_meal));
}

int		philosopher_takes_second_fork(t_philo *philo)
{
	int		start_time;

	start_time = get_time();
	if (philo->id % 2)
		pthread_mutex_lock(philo->fork_left);
	else
		pthread_mutex_lock(philo->fork_right);
	philo->time_since_last_meal += get_time() - start_time;
	return (check_pulse(philo->time_to_die, philo->time_since_last_meal));
}

void	*philosopher_dies(t_philo *philo, long time_of_death)
{
	printf("%ld|\t\033[91m%d has died\033[0m\n", time_of_death, philo->id);
	if (fork_flag[0])
		pthread_mutex_unlock(philo->fork_first);
	if (fork_flag[1])
		pthread_mutex_unlock(philo->fork_second);
	return (NULL);
}

void	*philo_routine(void *routine_arg)
{
	t_philo *philo = (t_philo *)routine_arg;
	long	start_time;

	start_time = get_time();
	while (1 /* here opt cond */)
	{
		printf("%ld|\t\033[32m%d is thinking\033[0m\n", get_time() - start_time, philo->id);
		if (philosopher_takes_first_fork(philo) < 0)
			return (philosopher_dies(philo, get_time() - start_time));
		printf("%ld|\t%d has taken a fork\n", get_time() - start_time, philo->id);
		if (philosopher_takes_second_fork(philo) < 0)
			return (philosopher_dies(philo, get_time() - start_time));
		printf("%ld|\t%d has taken a fork\n", get_time() - start_time, philo->id);
		philo->time_since_last_meal = 0; // reiniciado contador de comida
		printf("%ld|\t%d is eating\n", get_time() - start_time, philo->id);
		if (philosopher_waits(philo->time_to_eat, philo) < 0)
			return (philosopher_dies(philo, get_time() - start_time));
		pthread_mutex_unlock(philo->fork_left);
		pthread_mutex_unlock(philo->fork_right);
		printf("%ld|\t%d is sleeping\n", get_time() - start_time, philo->id);
		if (philosopher_waits(philo->time_to_sleep, philo) < 0)
			return (philosopher_dies(philo, get_time() - start_time));
	}
	return ((void *)philo->philo_thread);
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
		philo_arr[index]->time_to_die = data->time_to_die;
		philo_arr[index]->time_to_eat = data->time_to_eat;
		philo_arr[index]->time_to_sleep = data->time_to_sleep;
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
	rm_threads(data, philo_arr);

}

