#include "philosophers.h"

static int	clean_common(t_common *common, t_philo **philo_arr)
{
	unsigned int	i;

	i = 0;
	while (i < common->n_philo)
	{
		pthread_mutex_destroy(common->fork_arr[i]->fork_lock);
		free(common->fork_arr[i]->fork_lock);
		free(common->fork_arr[i]);
		free(philo_arr[i]->supervisor_lock);
		free(philo_arr[i]);
		i++;
	}
	free(philo_arr);
	free(common->fork_arr);
	free(common->pthread_arr);
	free(common->alive_lock);
	free(common);
	return (0);
}

static t_philo	*philo_setup(unsigned int i, t_common *common)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	memset(philo, 0, sizeof(t_philo));
	philo->common = common;
	philo->id = i + 1;
	philo->supervisor_lock = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(philo->supervisor_lock, 0);
	philo->time_new_meal = common->time_start;
	philo->hands_id[i % 2] = (i + 1) % common->n_philo;
	philo->hands_id[!(i % 2)] = i;
	return (philo);
}

void	init_threads(t_common *common)
{
	t_philo			**philo_arr;
	unsigned int	i;

	if (!common)
		return ;
	philo_arr = malloc(sizeof(t_philo *) * common->n_philo);
	i = 0;
	common->time_start = get_time();
	while (i < common->n_philo)
	{
		philo_arr[i] = philo_setup(i, common);
		pthread_create(&common->pthread_arr[i], \
				0, philo_routine, (void *)philo_arr[i]);
		pthread_create(&common->pthread_arr[i + common->n_philo], \
				0, supervisor_routine, (void *)philo_arr[i]);
		pthread_detach(common->pthread_arr[i + common->n_philo]);
		i++;
	}
	i = 0;
	while (i < common->n_philo)
	{
		pthread_join(common->pthread_arr[i], 0);
		i++;
	}
	clean_common(common, philo_arr);
}
