/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhnine <mohhnine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:45:52 by mohhnine          #+#    #+#             */
/*   Updated: 2026/05/03 02:46:12 by mohhnine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	parse_int(char *str, int min, char *name)
{
	int	val;

	val = ft_atoi(str);
	if (val < min)
	{
		fprintf(stderr, "%s is not valid\n", name);
		return (-1);
	}
	return (val);
}

static int	parse_scheduler(char *str, t_args *info)
{
	if (strcmp("fifo", str) == 0)
		info->scheduler = 0;
	else if (strcmp("edf", str) == 0)
		info->scheduler = 1;
	else
	{
		fprintf(stderr, "you must use 'edf or fifo' as scheduler\n");
		return (1);
	}
	return (0);
}

int	parse_arg(char **av, t_args *info)
{
	info->num_coders = parse_int(av[1], 1, "num_coders");
	if (info->num_coders < 0)
		return (1);
	info->time_to_burnout = parse_int(av[2], 1, "time_to_burnout");
	if (info->time_to_burnout < 0)
		return (1);
	info->time_to_compile = parse_int(av[3], 1, "time_to_compile");
	if (info->time_to_compile < 0)
		return (1);
	info->time_to_debug = parse_int(av[4], 1, "time_to_debug");
	if (info->time_to_debug < 0)
		return (1);
	info->time_to_refactor = parse_int(av[5], 1, "time_to_refactor");
	if (info->time_to_refactor <= 0)
		return (1);
	info->num_compiles_required = parse_int(av[6], 1, "num_compiles_required");
	if (info->num_compiles_required < 0)
		return (1);
	info->dongle_cooldown = parse_int(av[7], 0, "dongle_cooldown");
	if (info->dongle_cooldown < 0)
		return (1);
	return (parse_scheduler(av[8], info));
}

// int	parse_arg(int ac, char **av, t_args *info)
// {
// 	if (ac != 9)
// 	{
// 		fprintf(stderr, "the number of arguments is not 9");
// 		return (1);
// 	}
// 	info->num_coders = ft_atoi(av[1]);
// 	if (info->num_coders < 1)
// 	{
// 		fprintf(stderr, "num_coders is not valid");
// 		return (1);
// 	}
// 	info->time_to_burnout = ft_atoi(av[2]);
// 	if (info->time_to_burnout < 1)
// 	{
// 		fprintf(stderr, "time_to_burnout is not valid");
// 		return (1);
// 	}
// 	info->time_to_compile = ft_atoi(av[3]);
// 	if (info->time_to_compile < 1)
// 	{
// 		fprintf(stderr, "time_to_compile is not valid");
// 		return (1);
// 	}
// 	info->time_to_debug = ft_atoi(av[4]);
// 	if (info->time_to_debug < 0)
// 	{
// 		fprintf(stderr, "time_to_debug is not valid");
// 		return (1);
// 	}
// 	info->time_to_refactor = ft_atoi(av[5]);
// 	if (info->time_to_refactor < 0)
// 	{
// 		fprintf(stderr, "time_to_refactor is not valid");
// 		return (1);
// 	}
// 	info->num_compiles_required = ft_atoi(av[6]);
// 	if (info->num_compiles_required < 1)
// 	{
// 		fprintf(stderr, "num_compiles_required is not valid");
// 		return (1);
// 	}
// 	info->dongle_cooldown = ft_atoi(av[7]);
// 	if (info->dongle_cooldown < 0)
// 	{
// 		fprintf(stderr, "dongle_cooldown is not valid");
// 		return (1);
// 	}
// 	if (strcmp("fifo", av[8]) == 0)
// 		info->scheduler = 0;
// 	else if (strcmp("edf", av[8]) == 0)
// 		info->scheduler = 1;
// 	else
// 	{
// 		fprintf(stderr, "you must use 'edf or fifo' as sechdeuler");
// 		return (1);
// 	}
// 	return (0);
// }
