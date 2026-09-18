#include "libft.h"
#include "scene_types.h"
#include "utils.h"
#include "vector.h"

#include <math.h>

#define AMBIENT_ID	"A"
#define LIGHT_ID	"L"

static struct s_light	_light_init(void)
{
	return ((struct s_light){
		.type = RT_ELEM_INVALID,
		.scene = NULL,
		.as_element = NULL,
		.color.a = {NAN, NAN, NAN},
		.__normal = NULL,
		.__hit = NULL,
		.origin.a = {NAN, NAN, NAN},
		.brightness = NAN});
}

static bool	_brightness_valid(const char *string)
{
	float	range;

	range = rt_atof(string);
	if (range < 0 || range > 1.0f)
	{
		return (false);
	}
	return (true);
}

t_element	parse_ambient(const char **s)
{
	struct s_light	res;
	t_vec3			v;
	int				i;
	const int		n_fields = 3;

	ft_assert(!!s, __FUNCTION__, "s is NULL", NULL);
	res = _light_init();
	i = 0;
	while (s[i])
	{
		v = a_to_vec3(s[i]);
		if (i == 0 && !ft_strncmp(s[i], AMBIENT_ID, 2) && ++i)
			;
		else if (i == 1 && _brightness_valid(s[i]))
			res.brightness = rt_atof(s[i++]);
		else if (i == 2 && vec3_valid(v) && vec3_valid_color(s[i]) && ++i)
			res.color = vec3_scale(v, 1.0f / 255);
		else
			return ((t_element){.type = RT_ELEM_INVALID});
	}
	if (i == n_fields && !isnan(res.brightness))
		res.type = RT_ELEM_AMBIENT;
	return ((t_element){.light = res});
}

t_element	parse_light(const char **s)
{
	struct s_light	res;
	t_vec3			v;
	int				i;
	const int		n_fields = 4;

	ft_assert(!!s, __FUNCTION__, "s is NULL", NULL);
	res = _light_init();
	i = 0;
	while (s[i])
	{
		v = a_to_vec3(s[i]);
		if (i == 0 && !ft_strncmp(s[i], LIGHT_ID, 2) && ++i)
			;
		else if (i == 1 && vec3_valid(v) && ++i)
			res.origin = v;
		else if (i == 2)
			res.brightness = rt_atof(s[i++]);
		else if (i == 3 && vec3_valid(v) && vec3_valid_color(s[i]) && ++i)
			res.color = vec3_scale(v, 1.0f / 255);
		else
			return ((t_element){.type = RT_ELEM_INVALID});
	}
	if (i == n_fields && !isnan(res.brightness))
		res.type = RT_ELEM_LIGHT;
	return ((t_element){.light = res});
}

