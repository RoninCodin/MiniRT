#include "libft.h"
#include "scene_types.h"
#include "vector.h"
#include "utils.h"
#include <math.h>

#define SPHERE_ID	"sp"

struct s_sphere	sphere_init(void);

t_element	parse_sphere(const char **s)
{
	struct s_sphere	res;
	const int		n_fields = 4;
	t_vec3			v;
	int				i;

	ft_assert(!!s, __FUNCTION__, "s is NULL", NULL);
	res = sphere_init();
	i = 0;
	while (s[i])
	{
		v = a_to_vec3(s[i]);
		if (i == 0 && !ft_strncmp(s[i], SPHERE_ID, 3) && ++i)
			;
		else if (i == 1 && vec3_valid(v) && ++i)
			res.center = v;
		else if (i == 2 && ft_atof(s[i]) != 0)
			res.radius = rt_atof(s[i++]) / 2;
		else if (i == 3 && vec3_valid(v) && vec3_valid_color(s[i]) && ++i)
			res.color = vec3_scale(v, 1.0f / 255);
		else
			return ((t_element){.type = RT_ELEM_INVALID});
	}
	if (i == n_fields && !isnan(res.radius))
		res.type = RT_ELEM_SPHERE;
	return ((t_element){.sphere = res});
}

