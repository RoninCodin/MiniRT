#include "libft.h"
#include "scene_types.h"
#include "utils.h"
#include "vector.h"

#include <math.h>

#define CYLINDER_ID "cy"

__attribute__((used))
static t_vec3	_cylinder_normal(const union u_element *elem, const t_vec3 *v)
{
	const struct s_cylinder	*cyl;
	t_vec3					cp;
	t_real					m;
	t_vec3					axis_proj;
	t_vec3					normal;

	cyl = &elem->cyl;
	cp = vec3_add(*v, vec3_scale(cyl->center, -1));
	m = vec3_dot(cp, cyl->axis);
	axis_proj = vec3_scale(cyl->axis, m);
	normal = vec3_add(cp, vec3_scale(axis_proj, -1));
	return (vec3_normalize(normal));
}

t_hit	cyl_cap_hit(const struct s_cylinder *cyl, t_ray *ray);
t_hit	cyl_side_hit(const struct s_cylinder *cyl, t_ray *ray);

static t_hit	_cylinder_hit(const t_element *elem, t_ray ray)
{
	const struct s_cylinder	*cyl;
	t_hit					side;
	t_hit					cap;

	ft_assert(elem->type == RT_ELEM_CYLINDER,
		__FUNCTION__, "invalid element type", NULL);
	cyl = &elem->cyl;
	side = cyl_side_hit(cyl, &ray);
	cap = cyl_cap_hit(cyl, &ray);
	if (isnan(side.t) && isnan(cap.t))
		return (hit_init(elem, ray));
	if (isnan(side.t))
		return (cap);
	if (isnan(cap.t))
		return (side);
	if (side.t > cap.t)
		return (side);
	return (cap);
}

static struct s_cylinder	_cylinder_init(void)
{
	return ((struct s_cylinder){
		.type = RT_ELEM_INVALID,
		.normal = _cylinder_normal,
		.hit = _cylinder_hit,
		.center.a = {NAN, NAN, NAN},
		.axis.a = {NAN, NAN, NAN},
		.color.a = {NAN, NAN, NAN},
		.height = NAN,
		.radius = NAN,
		.as_element = NULL,
		.scene = NULL,
	});
}

static int	_vec_parse(int *i, struct s_cylinder *cyl, const char *s)
{
	t_vec3	v;

	v = a_to_vec3(s);
	if (*i == 1 && vec3_valid(v) && ++*i)
		cyl->center = v;
	else if (*i == 2 && vec3_valid(v) && ++*i)
		cyl->axis = vec3_normalize(v);
	else if (*i == 5 && vec3_valid(v) && vec3_valid_color(s) && ++*i)
		cyl->color = vec3_scale(v, 1.0f / 255);
	else
		return (0);
	return (1);
}

t_element	parse_cylinder(const char **s)
{
	struct s_cylinder	res;
	int					i;
	const int			n_fields = 6;

	ft_assert(!!s, __FUNCTION__, "s is NULL", NULL);
	res = _cylinder_init();
	i = 0;
	while (s[i])
	{
		if (i == 0 && ft_strncmp(s[i++], CYLINDER_ID,
				ft_strlen(CYLINDER_ID) + 1))
			return ((t_element){.type = RT_ELEM_INVALID});
		else if (i == 3)
			res.radius = rt_atof(s[i++]) / 2;
		else if (i == 4)
			res.height = rt_atof(s[i++]) / 2;
		else if ((i == 1 || i == 2 || i == 5) && _vec_parse(&i, &res, s[i]))
			;
		else
			return ((t_element){.type = RT_ELEM_INVALID});
	}
	if (i == n_fields && !isnan(res.radius) && !isnan(res.height))
		res.type = RT_ELEM_CYLINDER;
	return ((t_element){.cyl = res});
}

