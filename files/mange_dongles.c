/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mange_dongles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhnine <mohhnine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:45:41 by mohhnine          #+#    #+#             */
/*   Updated: 2026/05/03 02:31:01 by mohhnine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static void	set_deadline(t_coder *coder, t_dongle *dongle)
{
	t_edf	hold;

	hold.id = coder->id;
	if (coder->sim->args.scheduler == 0)
		hold.deadline = get_time_ms();
	else
		hold.deadline = coder->last_compile + coder->sim->args.time_to_burnout;
	insert_heap(dongle, hold);
}

static int	whait_dongle(t_coder *coder, t_dongle *dongle)
{
	while (dongle->is_taken == 1 || (get_time_ms()
			- dongle->release < coder->sim->args.dongle_cooldown)
		|| dongle->quee[0].id != coder->id)
	{
		pthread_mutex_lock(&coder->sim->pause);
		if (coder->sim->simulation_running == 0)
		{
			dongle->size--;
			pthread_mutex_unlock(&coder->sim->pause);
			pthread_mutex_unlock(&dongle->pause_dongle);
			return (1);
		}
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&dongle->pause_dongle);
		usleep(1000);
		pthread_mutex_lock(&dongle->pause_dongle);
	}
	return (0);
}

static int	claim_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&coder->sim->pause_print);
	pthread_mutex_lock(&coder->sim->pause);
	if (coder->sim->simulation_running == 0 || coder->done == 1)
	{
		dongle->size--;
		pthread_mutex_unlock(&coder->sim->pause);
		pthread_mutex_unlock(&coder->sim->pause_print);
		pthread_mutex_unlock(&dongle->pause_dongle);
		return (1);
	}
	pthread_mutex_unlock(&coder->sim->pause);
	pop_heap(dongle);
	dongle->is_taken = 1;
	fprintf(stdout, "%s%ld %d has taken a dongle%s\n", coder->color,
		get_time_ms() - coder->sim->start_time, coder->id, RESET);
	pthread_mutex_unlock(&coder->sim->pause_print);
	return (0);
}

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->pause_dongle);
	set_deadline(coder, dongle);
	if (whait_dongle(coder, dongle))
		return ;
	if (claim_dongle(coder, dongle))
		return ;
	pthread_mutex_unlock(&dongle->pause_dongle);
}
