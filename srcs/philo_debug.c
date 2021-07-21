#include "philosophers.h"

void print_data(t_data *data)
{
	printf("philo: %u\ndie: %u\neat: %u\nsleep: %u\nmust: %d\n",\
			data->n_philo,\
			data->time_to_die,\
			data->time_to_eat,\
			data->time_to_sleep,\
			data->times_must_eat);
}
