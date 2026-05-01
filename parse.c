#include "header.h"

int	parse_arg(int ac, char **av, t_args *info)
{
	if (ac != 9)
	{
		fprintf(stderr, "the number of arguments is not 9");
		return (1);
	}
	info->num_coders = ft_atoi(av[1]);
	if (info->num_coders < 1)
	{
		fprintf(stderr, "num_coders is not valid");
		return (1);
	}
	info->time_to_burnout = ft_atoi(av[2]);
	if (info->time_to_burnout < 1)
	{
		fprintf(stderr, "time_to_burnout is not valid");
		return (1);
	}
	info->time_to_compile = ft_atoi(av[3]);
	if (info->time_to_compile < 1)
	{
		fprintf(stderr, "time_to_compile is not valid");
		return (1);
	}
	info->time_to_debug = ft_atoi(av[4]);
	if (info->time_to_debug < 0)
	{
		fprintf(stderr, "time_to_debug is not valid");
		return (1);
	}
	info->time_to_refactor = ft_atoi(av[5]);
	if (info->time_to_refactor < 0)
	{
		fprintf(stderr, "time_to_refactor is not valid");
		return (1);
	}
	info->num_compiles_required = ft_atoi(av[6]);
	if (info->num_compiles_required < 1)
	{
		fprintf(stderr, "num_compiles_required is not valid");
		return (1);
	}
	info->dongle_cooldown = ft_atoi(av[7]);
	if (info->dongle_cooldown < 0)
	{
		fprintf(stderr, "dongle_cooldown is not valid");
		return (1);
	}
	if (strcmp("fifo", av[8]) == 0)
		info->scheduler = 0;
	else if (strcmp("edf", av[8]) == 0)
		info->scheduler = 1;
	else
	{
		fprintf(stderr, "you must use 'edf or fifo' as sechdeuler");
		return (1);
	}
	return (0);
}
