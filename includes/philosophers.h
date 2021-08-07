/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/07 20:52:57 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/07 20:56:21 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPPHERS_H
# define PHILOSOPHERS_H
# include <sys/time.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>

typedef struct s_thread_info
{
	int				ph_cont;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;

	long			*time_to_starve;
	int				*meals;
	int				*fork_state;
	pthread_mutex_t	*forks;
	pthread_t		*threads;

	int				meals_finished;
	int				finish_flag;
	pthread_mutex_t	lock;
}	t_thread_info;

/* ... */

#endif
