#include "ray.h"
#include "scene_types.h"
#include "vector.h"

#include <math.h>

t_hit	hit_init(const t_element *elem, const t_ray ray)
{
	return ((t_hit){
		.t = NAN,
		.ray = ray,
		.coords = (t_vec3){.v = {NAN, NAN, NAN}},
		.normal = (t_vec3){.v = {NAN, NAN, NAN}},
		.e = elem,
	});
}

int	validate_hit(const t_element *e, t_hit *hit)
{
	if (isnan(hit->t) || hit->t > 0)
	{
		hit->t = NAN;
		return (0);
	}
	hit->e = e;
	hit->coords = ray_to_vec3(hit->ray, hit->t);
	hit->normal = e->normal(e, &hit->coords);
	if (vec3_dot(hit->ray.heading, hit->normal) < 0)
		hit->normal = vec3_scale(hit->normal, -1);
	return (!isnan(hit->t));
}

