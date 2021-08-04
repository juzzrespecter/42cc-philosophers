/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:00:05 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/04 20:35:07 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H
# include <sys/time.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <semaphore.h>
# include <signal.h>

# define THINK_ID 0
# define FORK_ID 1
# define EAT_ID 2
# define SLEEP_ID 3
# define DEAD_ID 4

typedef struct s_data
{
	pthread_t		*pthread_arr;
	pthread_t		sv_thread;
	pid_t			*pid_arr;
	long			time_start;
	long			time_last_meal;
	int				n_philo;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	int				times_must_eat;
	int				finished_meals;
	int				dead_flag;
	int				id;

	sem_t			*forks;
	sem_t			*meals;
	sem_t			*waiter;
	sem_t			*process_lock;
	sem_t			*start_lock;
	sem_t			*finish_lock;
	sem_t			*supervisor_lock;
}	t_data;

unsigned int	ft_atou(char *a);
int				ft_isdigit(int c);
void			print_status(int status_id, long time, int philo_id);

long			get_time(void);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);

void			philosopher_process_starts(int id, t_data *data);
void			philosopher_thinks(t_data *data);
void			philosopher_eats(t_data *data);
void			philosopher_sleeps(t_data *data);

#endif
