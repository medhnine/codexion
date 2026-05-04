/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tasks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhnine <mohhnine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:46:02 by mohhnine          #+#    #+#             */
/*   Updated: 2026/05/02 20:43:25 by mohhnine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_msleep(t_coder *coder, long ms)
{
	long	end;

	end = get_time_ms() + ms;
	while (get_time_ms() < end)
	{
		pthread_mutex_lock(&coder->sim->pause);
		if (coder->sim->simulation_running == 0)
		{
			pthread_mutex_unlock(&coder->sim->pause);
			return ;
		}
		pthread_mutex_unlock(&coder->sim->pause);
		usleep(1000);
	}
}

void	compile(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0 || coder->done == 1)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	fprintf(stdout, "%s%ld %d is compiling%s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	coder->last_compile = get_time_ms();
	coder->number_of_compilations++;
	pthread_mutex_unlock(&coder->sim->pause);
	ft_msleep(coder, coder->sim->args.time_to_compile);
	release_dongle(coder->right_dongle);
	release_dongle(coder->left_dongle);
}

void	debuging(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	fprintf(stdout, "%s%ld %d is debugging%s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	ft_msleep(coder, coder->sim->args.time_to_debug);
}

void	refactoring(t_coder *coder)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0)
	{
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		return ;
	}
	pthread_mutex_unlock(&coder->sim->pause);
	fprintf(stdout, "%s%ld %d is refactoring%s\n", coder->color, get_time_ms()
		- coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	ft_msleep(coder, coder->sim->args.time_to_refactor);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->number_of_compilations == coder->sim->args.num_compiles_required)
		coder->done = 1;
	pthread_mutex_unlock(&coder->sim->pause);
}
