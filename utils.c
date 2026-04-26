#include "header.h"

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