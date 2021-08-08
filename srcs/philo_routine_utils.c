/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danrodri <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/08 18:57:04 by danrodri          #+#    #+#             */
/*   Updated: 2021/08/08 18:59:30 by danrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	get_id(void)
{
	static int	id_counter = -1;

	id_counter++;
	return (id_counter);
}

void	msg_lock(int status_id, int philo_id, t_thread_info *ph_info)
{
	pthread_mutex_lock(&ph_info->lock);
	print_status(status_id, get_time() - ph_info->time_start, philo_id);
	pthread_mutex_unlock(&ph_info->lock);
}
