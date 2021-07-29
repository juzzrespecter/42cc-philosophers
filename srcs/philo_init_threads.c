#include "philosophers.h"

static int	clean_common(t_common *common, t_philo **philos)
{
	int	i;

	i = 0;
	while (i < common->n_philo)
	{
		pthread_mutex_destroy(common->forks[i]);
		pthread_mutex_destroy(philos[i]->supervisor_lock);
		free(common->forks[i]);
		free(philos[i]->supervisor_lock);
		free(philos[i]);
		i++;
	}
	pthread_mutex_destroy(common->thread_lock);
	free(philos);
	free(common->forks);
	free(common->forks_state);
	free(common->threads);
	free(common->thread_lock);
	free(common);
	return (0);
}

static t_philo	**philo_setup(t_common *common)
{
	t_philo			**philosophers;
	t_philo			*philo;
	int	index;

	index = 0;
	philosophers = malloc(sizeof(t_philo *) * common->n_philo);
	while (index < common->n_philo)
	{
		philo = malloc(sizeof(t_philo));
		memset(philo, 0, sizeof(t_philo));
		philo->common = common;
		philo->id = index;
		philo->supervisor_lock = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(philo->supervisor_lock, 0);
		philo->time_since_new_meal = common->time_start;
		philo->hands_id[(index % 2)] = (index + 1) % common->n_philo;
		philo->hands_id[!(index % 2)] = index;
		philo->time_since_new_meal = common->time_start;
		philosophers[index] = philo;
		index++;
	}
	return (philosophers);
}

static void	*metre_routine(void *metre_args)
{
	t_common	*common;

	common = (t_common *)metre_args;
	while (1)
	{
		pthread_mutex_lock(common->thread_lock);
		if (common->end_simulation_flag == 1)
			break ;
		if (common->finished_meals_counter == (int)common->n_philo)
		{
			common->end_simulation_flag = 1;
			printf("%ld| \033[96mall philosophers finished their meals.\033[0m\n",\
				   	get_time() - common->time_start);
			break ;
		}
		pthread_mutex_unlock(common->thread_lock);
	}
	pthread_mutex_unlock(common->thread_lock);
	return (NULL);
}

void	init_threads_waits(t_common *common)
{
	int	i;

	i = 0;
	while (i < common->n_philo)
	{
		pthread_join(common->threads[i], 0);
		i++;
	}
}

void	init_threads(t_common *common)
{
	int	i;
	pthread_t		metre;
	t_philo			**ph_a;

	if (!common)
		return ;
	i = 0;
	common->time_start = get_time();
	ph_a = philo_setup(common);
	while (i < common->n_philo)
	{
		pthread_create(&common->threads[i], 0, philo_routine, (void *)ph_a[i]);
		i++;
	}
	if (common->times_must_eat != -1)
	{
		pthread_create(&metre, 0, metre_routine, (void *)common);
		pthread_detach(metre);
	}
	init_threads_waits(common);
	clean_common(common, ph_a);
}
