/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 20:52:57 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/08 20:33:13 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>

# define THINK_ID 0
# define FORK_ID 1
# define EAT_ID 2
# define SLEEP_ID 3
# define DEAD_ID 4
# define FINISH_ID 5

typedef struct s_thread_info
{
	int				ph_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;

	long			*time_to_starve;
	int				*meals;
	pthread_mutex_t	*forks;
	pthread_t		*threads;

	int				finished_meals;
	int				finish_flag;
	pthread_mutex_t	lock;
	long			time_start;

	/* test */
	pthread_mutex_t	*waiter;
	pthread_mutex_t waiter_start;
	int				*waiter_state;
}	t_thread_info;


t_thread_info	*thread_info_setup(int argc, char **argv);

void		*routine(void *args);
int			get_id(void);
int			ft_atoi(char *a);
long		get_time(void);
void		philo_waits(long time_to_wait);
void		msg_lock(int status_id, int philo_id, t_thread_info *ph_info);
void		print_status(int status_id, long timestamp, int philo_id);
void		*free_data(t_thread_info *ph_info);

#endif
