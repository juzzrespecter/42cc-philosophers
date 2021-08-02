/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 20:42:10 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/02 21:16:32 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>

# define THINK_ID 0
# define FORK_ID 1
# define EAT_ID 2
# define SLEEP_ID 3
# define DEAD_ID 4

typedef struct s_time
{
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				times_must_eat;

}	t_time;

typedef struct s_philo
{
	long			time_start;
	pthread_mutex_t	*lock;
	pthread_mutex_t	**forks;
	int				*forks_state;

	t_time			time;

	int				*end_simulation_flag;
	int				*finished_count;

	int				id;
	int				hands_id[2];
	int				finished_meals;
	long			time_to_starve;
}	t_philo;

typedef struct s_data
{
	long			time_start;
	pthread_t		*threads;
	pthread_mutex_t	*lock;
	pthread_mutex_t	**forks;
	int				*forks_state;
	pthread_t		metre;
	t_time			time;
	int				n_of_philos;
	t_philo			**philo_array;
	int				end_simulation_flag;
	int				finished_count;
}	t_data;

long			get_time(void);
void			print_status(int status_id, long time, int philo_id);
int				ft_isdigit(int c);
unsigned int	ft_atou(char *a);

int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);
void			*routine(void *routine_args);

int				philo_waits(long time_to_wait, t_philo *data);
int				philo_checks_if_died(t_philo *data);
void			msg_lock(int status_id, t_philo *data);

#endif
