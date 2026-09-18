#include "ft_linalg.h"
#include "libft.h"

#include <math.h>
#include <stdbool.h>

t_real	rt_atof(const char *str)
{
	_Bool	period;
	size_t	i;

	i = 0;
	period = false;
	if (str[i] == '-')
		++i;
	while (str[i])
	{
		if (str[i] == '.')
		{
			if (period)
				return (NAN);
			period = true;
		}
		else if (!ft_isdigit(str[i]))
			return (NAN);
		++i;
	}
	return (ft_atof(str));
}

