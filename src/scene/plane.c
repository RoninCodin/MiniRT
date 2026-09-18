#include "libft.h"
#include "ray.h"
#include "scene_types.h"
#include "utils.h"
#include "vector.h"

#include <math.h>
#include <stdio.h>

#define PLANE_ID "pl"

__attribute__((used))
static t_vec3	_plane_normal(const union u_element *elem, const t_vec3 *v)
{
	(void)v;
	return (elem->plane.v_normal);
}

static t_hit	_plane_hit(const t_element *elem, t_ray ray)
{
	const struct s_plane	*plane;
	t_hit					hit;

	ft_assert(elem->type == RT_ELEM_PLANE,
		__FUNCTION__, "invalid element type", NULL);
	plane = &elem->plane;
	hit = hit_init(elem, ray);
	hit.t = (plane->d - vec3_dot(ray.origin, plane->v_normal))
		/ vec3_dot(ray.heading, plane->v_normal);
	validate_hit(elem, &hit);
	return (hit);
}

struct s_plane	_plane_init(void)
{
	return ((struct s_plane){
		.type = RT_ELEM_INVALID,
		.normal = _plane_normal,
		.hit = _plane_hit,
		.v_normal.a = {NAN, NAN, NAN},
		.center.a = {NAN, NAN, NAN},
		.color.a = {NAN, NAN, NAN},
		.d = NAN,
		.as_element = NULL,
		.scene = NULL,
	});
}

t_element	parse_plane(const char **s)
{
	struct s_plane	res;
	t_vec3			v;
	int				i;

	res = _plane_init();
	i = 0;
	while (s[i])
	{
		v = a_to_vec3(s[i]);
		if (i == 0 && !ft_strncmp(s[i], PLANE_ID, 3) && ++i)
			continue ;
		if (i == 1 && vec3_valid(v) && ++i)
			res.center = v;
		else if (i == 2 && vec3_valid_normal(v) && ++i)
			res.v_normal = v;
		else if (i == 3 && vec3_valid(v) && vec3_valid_color(s[i]) && ++i)
			res.color = vec3_scale(v, 1.0f / 255);
		else
			return ((t_element){.type = RT_ELEM_INVALID});
	}
	if (i == 4)
		res.type = RT_ELEM_PLANE;
	if (i == 4)
		res.d = vec3_dot(res.center, res.v_normal);
	return ((t_element){.plane = res});
}

