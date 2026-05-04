/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhnine <mohhnine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:45:56 by mohhnine          #+#    #+#             */
/*   Updated: 2026/05/01 18:45:57 by mohhnine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	threads_manger(t_args argument, t_coder *list_coders)
{
	int	i;

	i = 0;
	while (i < argument.num_coders)
	{
		pthread_create(&list_coders[i].thread, NULL, &simulation,
			(void *)&list_coders[i]);
		i++;
	}
	i = 0;
	while (i < argument.num_coders)
	{
		pthread_join(list_coders[i].thread, NULL);
		i++;
	}
}

void	intalaize_thredas(t_args argument, t_coder *list_coders,
		t_simulation *get)
{
	int	i;

	i = 0;
	while (i < argument.num_coders)
	{
		list_coders[i].id = i + 1;
		list_coders[i].done = 0;
		list_coders[i].color = get_color(i + 1);
		list_coders[i].number_of_compilations = 0;
		list_coders[i].last_compile = get->start_time;
		pthread_mutex_init(&get->dongles[i].pause_dongle, NULL);
		get->dongles[i].release = 0;
		get->dongles[i].size = 0;
		get->dongles[i].is_taken = 0;
		list_coders[i].left_dongle = &get->dongles[i];
		list_coders[i].right_dongle = &get->dongles[(i + 1)
			% argument.num_coders];
		list_coders[i].sim = get;
		i++;
	}
}
