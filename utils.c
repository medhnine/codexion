#include "header.h"

char	*get_color(int id)
{
    const char *colors[6];

    colors[0] = "\033[1;31m";
    colors[1] = "\033[1;32m";
    colors[2] = "\033[1;33m";
    colors[3] = "\033[1;34m";
    colors[4] = "\033[1;35m";
    colors[5] = "\033[1;36m";
    return ((char *)colors[(id - 1) % 6]);
}

int	ft_atoi(char *str)
{
	int	result, count;

	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	while (*str == '-' || *str == '+')
	{
		if (*str == '-')
            return -1;
		str++;
	}
	result = 0;
    count = 0;
	while (*str != '\0' && *str >= '0' && *str <= '9')
	{
        if (count >= 10)
            return -1;
		result = result * 10 + (*str - '0');
		str++;
        count++;
	}
    if (count == 0 || *str != '\0')
        return -1;
    
    return (result);
}