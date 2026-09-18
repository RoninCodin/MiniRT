#include "ray.h"
#include "scene_types.h"
#include "vector.h"
#include <math.h>

static t_vec2	_get_cyl_roots(const struct s_cylinder *cyl, const t_ray *r)
{
	t_vec3	oc;
	t_vec3	dxv;
	t_vec3	ocxv;
	t_vec3	_;
	t_real	discriminant;

	oc = vec3_add(cyl->center, vec3_scale(r->origin, -1));
	dxv = vec3_cross(r->heading, cyl->axis);
	ocxv = vec3_cross(oc, cyl->axis);
	_.x = vec3_dot(dxv, dxv);
	_.y = vec3_dot(dxv, ocxv);
	_.z = vec3_dot(ocxv, ocxv) - cyl->radius * cyl->radius;
	discriminant = _.y * _.y - _.x * _.z;
	if (discriminant < 0)
		return ((t_vec2){.x = NAN, .y = NAN});
	return ((t_vec2){.x = (_.y - sqrtf(discriminant)) / _.x,
		.y = (_.y + sqrtf(discriminant)) / _.x});
}

static int	_validate_cyl_height(const struct s_cylinder *cyl,
	const t_ray *r, t_real t)
{
	t_vec3	p;
	t_vec3	cp;
	t_real	m;

	if (isnan(t))
		return (0);
	p = ray_to_vec3(*r, t);
	cp = vec3_add(p, vec3_scale(cyl->center, -1));
	m = vec3_dot(cp, cyl->axis);
	return (m >= -cyl->height && m <= cyl->height);
}

t_hit	cyl_side_hit(const struct s_cylinder *cyl, t_ray *ray)
{
	t_vec2	roots;
	t_hit	hit;

	hit = hit_init(cyl->as_element, *ray);
	roots = _get_cyl_roots(cyl, ray);
	hit.t = roots.a[1];
	if (!isnan(hit.t) && _validate_cyl_height(cyl, ray, hit.t)
		&& validate_hit(cyl->as_element, &hit))
		return (hit);
	hit.t = roots.a[0];
	if (!isnan(hit.t) && _validate_cyl_height(cyl, ray, hit.t))
		validate_hit(cyl->as_element, &hit);
	else
		hit.t = NAN;
	return (hit);
}

