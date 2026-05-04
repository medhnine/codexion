/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhnine <mohhnine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:45:47 by mohhnine          #+#    #+#             */
/*   Updated: 2026/05/01 18:45:48 by mohhnine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	check_burnout(t_simulation *manger, int i)
{
	int		burned_out;
	int		coder_id;
	long	start_time;

	burned_out = 0;
	pthread_mutex_lock(&manger->pause_print);
	pthread_mutex_lock(&manger->pause);
	if (manger->coders[i].done == 0 && get_time_ms()
		- manger->coders[i].last_compile > manger->args.time_to_burnout)
	{
		manger->simulation_running = 0;
		burned_out = 1;
	}
	coder_id = manger->coders[i].id;
	start_time = manger->coders[i].sim->start_time;
	pthread_mutex_unlock(&manger->pause);
	if (burned_out)
		fprintf(stdout, "%s%ld %d burned out%s\n", BURNOUT, get_time_ms()
			- start_time, coder_id, RESET);
	pthread_mutex_unlock(&manger->pause_print);
	if (burned_out)
		return (1);
	return (0);
}

static int	check_coders(t_simulation *manger)
{
	int	i;
	int	round;

	i = 0;
	round = 0;
	while (i < manger->args.num_coders)
	{
		if (check_burnout(manger, i))
			return (-1);
		pthread_mutex_lock(&manger->pause);
		if (manger->coders[i].done == 1)
			round++;
		pthread_mutex_unlock(&manger->pause);
		i++;
	}
	return (round);
}

void	*monitor(void *arg)
{
	int				round;
	t_simulation	*manger;

	manger = (t_simulation *)arg;
	pthread_mutex_lock(&manger->pause);
	while (manger->simulation_running == 1)
	{
		pthread_mutex_unlock(&manger->pause);
		usleep(1000);
		round = check_coders(manger);
		if (round == -1)
			return (NULL);
		if (round == manger->args.num_coders)
		{
			pthread_mutex_lock(&manger->pause);
			manger->simulation_running = 0;
			pthread_mutex_unlock(&manger->pause);
			return (NULL);
		}
		pthread_mutex_lock(&manger->pause);
	}
	pthread_mutex_unlock(&manger->pause);
	return (NULL);
}
