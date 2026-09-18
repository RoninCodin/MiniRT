#include "libft.h"
#include "scene_types.h"
#include "utils.h"
#include "vector.h"

#include <math.h>
#include <stdio.h>

#define CAMERA_ID "C"

static struct s_camera	_camera_init(void)
{
	return ((struct s_camera){
		.type = RT_ELEM_INVALID,
		.__normal = NULL,
		.__hit = NULL,
		.heading.a = {NAN, NAN, NAN},
		.origin.a = {NAN, NAN, NAN},
		.__color.a = {NAN, NAN, NAN},
		.as_element = NULL,
		.scene = NULL,
		.fov = NAN,
	});
}

static bool	_fov_valid(const char *s)
{
	float	fov;

	fov = rt_atof(s);
	if (isnan(fov) || fov >= 180.0f || fov < 0.0f)
		return (false);
	return (true);
}

t_element	parse_camera(const char **s)
{
	struct s_camera	res;
	t_vec3			v;
	int				i;
	const int		n_fields = 4;

	ft_assert(!!s, __FUNCTION__, "s is NULL", NULL);
	res = _camera_init();
	i = 0;
	while (s[i])
	{
		v = a_to_vec3(s[i]);
		if (i == 0 && !ft_strncmp(s[i++], CAMERA_ID, ft_strlen(CAMERA_ID) + 1))
			;
		else if (i == 1 && vec3_valid(v) && ++i)
			res.origin = v;
		else if (i == 2 && vec3_valid(v) && vec3_abs_lte(v, 1) && ++i)
			res.heading = v;
		else if (i == 3 && _fov_valid(s[i]))
			res.fov = rt_atof(s[i++]);
		else
			return ((t_element){.type = RT_ELEM_INVALID});
	}
	if (i == n_fields && !isnan(res.fov))
		res.type = RT_ELEM_CAMERA;
	return ((t_element){.camera = res});
}

