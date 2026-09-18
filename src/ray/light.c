#include "ft_linalg.h"
#include "ray.h"
#include "scene_types.h"
#include "utils.h"
#include "vector.h"
#include <math.h>

static t_vec3	_blend_light(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){.v = a.v * b.v});
}

static t_vec3	_ambient(const t_hit *hit)
{
	if (hit->e->scene->ambient.type != RT_ELEM_AMBIENT)
		return ((t_vec3){.a = {0, 0, 0}});
	return (_blend_light(hit->e->color,
			vec3_scale(hit->e->scene->ambient.color,
				hit->e->scene->ambient.brightness)));
}

__attribute__((unused))
static int	_light_obstructed(const t_hit *hit, const t_ray light_ray)
{
	t_hit	light_hit;

	light_hit = ray_hit(hit->e->scene, light_ray, hit->e);
	if (!isnan(light_hit.t) && light_hit.t >= -1)
		return (1);
	return (0);
}

__attribute__((unused))
static t_vec3	_diffuse(const t_hit *hit)
{
	t_vec3	ret;
	t_ray	ray;
	t_real	dot;
	t_real	dist;

	if (hit->e->scene->light.type != RT_ELEM_LIGHT)
		return ((t_vec3){.a = {0, 0, 0}});
	ret = (t_vec3){.a = {0}};
	ray.origin = hit->e->scene->light.origin;
	ray.heading = vec3_add(ray.origin, vec3_scale(hit->coords, -1));
	if (_light_obstructed(hit, ray))
		return (ret);
	dot = vec3_dot(hit->normal, vec3_normalize(ray.heading));
	if (dot < 0)
		return (ret);
	dist = vec3_length(ray.heading);
	(void)dist;
	ret = _blend_light(hit->e->color,
			vec3_scale(hit->e->scene->light.color,
				hit->e->scene->light.brightness * dot));
	return (ret);
}

t_color	hit_color(const t_hit *hit)
{
	t_vec3	ret;

	if (!hit->e)
		return (bg_checkerboard(hit->ray.pixel.x, hit->ray.pixel.y));
	ret = _ambient(hit);
	ret.v += _diffuse(hit).v;
	return (vec3_to_color(ret));
}

