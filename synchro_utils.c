/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   synchro_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aleexcolleet <marvin@42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 19:19:17 by aleexcolleet      #+#    #+#             */
/*   Updated: 2024/09/08 19:19:19 by aleexcolleet     ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//waiting for all threads to be ready
//technically SPINLOCK to Synchronize philos start
void	waiting_all_threads(t_data *data)
{
	while (!get_bool(&data->table_mutex, &data->all_threads_ready, data))
		;
}

bool	all_threads_running(t_mtx *mutex, long *threads,
		long philo_nbr, t_data *data)
{
	bool	ret;

	ret = false;
	safe_mutex_handle(mutex, LOCK, data);
	if (*threads == philo_nbr)
		ret = true;
	safe_mutex_handle(mutex, UNLOCK, data);
	return (ret);
}

//this increases threads running to 
//synchro with the monitor
void	increase_long(t_mtx *mutex, long *value, t_data *data)
{
	safe_mutex_handle(mutex, LOCK, data);
	(*value)++;
	safe_mutex_handle(mutex, UNLOCK, data);
}
//I'm using this function to fit with norminette terms. Just so that 
//I use less lines per fucntion.

void	dinner_beggin_utils(t_data *data, int i)
{
	if (i == 1)
	{
		safe_thread_handle(&data->monitor, monitor_dinner, data, CREATE);
		data->start_simulation = get_time(MILLISECOND);
		set_bool(&data->table_mutex, &data->all_threads_ready, true, data);
		return ;
	}
	else if (i == 2)
	{
		set_bool(&data->table_mutex, &data->end_simulation, true, data);
		safe_thread_handle(&data->monitor, NULL, NULL, JOIN);
		return ;
	}
	return ;
}

int	dinner_beggin_utils_extended(t_data *data, int i)
{
	while (++i < data->num_philo)
	{
		if (0 > safe_thread_handle(&data->philos[i].thread_id,
				NULL, NULL, JOIN))
		{
			data->error = 2;
			error_exit("thread managing error\n");
			return (2);
		}
	}
	return (1);
}
