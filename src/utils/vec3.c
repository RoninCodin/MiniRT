#include "ft_linalg.h"
#include "libft.h"
#include "utils.h"
#include "vector.h"

#include <math.h>

t_vec3	a_to_vec3(const char *s)
{
	t_vec3	res;
	char	**split;
	int		i;

	res = (t_vec3){.a = {NAN, NAN, NAN}};
	ft_assert(!!s, __FUNCTION__, "s is NULL", NULL);
	split = ft_split(s, ',');
	if (!split || !*split)
	{
		ft_split_free(split);
		return (res);
	}
	i = 0;
	while (split[i] && i < 3)
	{
		res.a[i] = rt_atof(split[i]);
		if (isnan(res.a[i]))
			break ;
		i++;
	}
	ft_split_free(split);
	return (res);
}

int	vec3_valid(const t_vec3 v)
{
	int	i;

	i = 0;
	while (i < 3)
		if (isnan(v.a[i++]))
			return (0);
	return (1);
}

int	vec3_abs_lte(const t_vec3 v, const t_real max)
{
	return (fabs(v.a[0]) <= max
		&& fabs(v.a[1]) <= max
		&& fabs(v.a[2]) <= max);
}

int	vec3_valid_normal(const t_vec3 v)
{
	return (vec3_valid(v) && vec3_abs_lte(v, 1));
}

