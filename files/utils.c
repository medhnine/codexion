/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohhnine <mohhnine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 18:46:14 by mohhnine          #+#    #+#             */
/*   Updated: 2026/05/03 23:07:02 by mohhnine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char	*get_color(int id)
{
	const char	*colors[6];

	colors[0] = "\033[1;32m";
	colors[1] = "\033[1;33m";
	colors[2] = "\033[1;34m";
	colors[3] = "\033[1;35m";
	colors[4] = "\033[1;36m";
	colors[5] = "\033[1;37m";
	return ((char *)colors[(id - 1) % 6]);
}

int	ft_atoi(char *str)
{
	long	result;
	int		count;

	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
			return (-1);
		str++;
	}
	result = 0;
	count = 0;
	while (*str != '\0' && *str >= '0' && *str <= '9')
	{
		if (count >= 10)
			return (-1);
		result = result * 10 + (*str - '0');
		str++;
		count++;
	}
	if (count == 0 || *str != '\0' || result > INT_MAX)
		return (-1);
	return (result);
}

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
