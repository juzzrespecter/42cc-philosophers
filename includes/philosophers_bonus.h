/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/03 22:00:05 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/17 17:07:40 by danrodri         ###   ########.fr       */
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
# define FINISHED_ID 5

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
	sem_t			*process_lock;
	sem_t			*start_lock;
	sem_t			*waiter;

    void *thread_lock;
}	t_data;

unsigned int	ft_atou(char *a);
char *ft_strncpy(char *, char *, int);
int				ft_isdigit(int c);
void			print_status(int status_id, long time, int philo_id);

long			get_time(void);
void			philo_wait(long time_to_wait);
int				philo_err_mgmt(int argc, char **argv);
void			init_threads(t_data *data);
int				ft_atoi(char *a);

void			philosopher_process_starts(int id, t_data *data);
void			philosopher_thinks(t_data *data);
void			philosopher_eats(t_data *data);
void			philosopher_sleeps(t_data *data);

#endif
