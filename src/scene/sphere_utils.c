#include "libft.h"
#include "scene_types.h"
#include "vector.h"
#include <math.h>
#include <stdio.h>

static t_vec3	_sphere_normal(const union u_element *e, const t_vec3 *v_hit)
{
	return (vec3_normalize((t_vec3){.v = v_hit->v - e->sphere.center.v}));
}

// Only using the '_' vector to save on variable declarations
static t_vec2	_get_roots(const struct s_sphere *s, const t_ray *r)
{
	t_vec3	_;
	t_vec3	oc;
	t_real	discriminant;

	oc = vec3_add(s->center, vec3_scale(r->origin, -1));
	_.x = vec3_dot(r->heading, r->heading);
	_.y = vec3_dot(r->heading, oc);
	_.z = vec3_dot(oc, oc) - s->radius * s->radius;
	discriminant = _.y * _.y - _.x * _.z;
	if (discriminant < 0)
		return ((t_vec2){.x = NAN, .y = NAN});
	return ((t_vec2){.x = (_.y - sqrtf(discriminant)) / _.x,
		.y = (_.y + sqrtf(discriminant)) / _.x});
}

static t_hit	_sphere_hit(const t_element *elem, const t_ray ray)
{
	const struct s_sphere	sphere = elem->sphere;
	t_vec2					roots;
	t_hit					hit;

	hit = hit_init(elem, ray);
	ft_assert(elem->type == RT_ELEM_SPHERE,
		__FUNCTION__, "invalid element type", NULL);
	roots = _get_roots(&sphere, &ray);
	hit.t = roots.a[1];
	if (validate_hit(elem, &hit))
		return (hit);
	hit.t = roots.a[0];
	validate_hit(elem, &hit);
	return (hit);
}

struct s_sphere	sphere_init(void)
{
	return ((struct s_sphere){
		.type = RT_ELEM_INVALID,
		.normal = _sphere_normal,
		.hit = _sphere_hit,
		.center.a = {NAN, NAN, NAN},
		.color.a = {NAN, NAN, NAN},
		.radius = NAN,
		.as_element = NULL,
		.scene = NULL,
	});
}

