#include "ray.h"
#include "scene_types.h"
#include "vector.h"
#include <math.h>

struct s_plane	_plane_init(void);

// return top if 1, bot if 0
static t_element	_cyl_caps(const struct s_cylinder *cyl, const int top_bot)
{
	struct s_plane	ret;

	ret = _plane_init();
	ret.type = RT_ELEM_PLANE;
	ret.center = cyl->center;
	ret.v_normal = cyl->axis;
	ret.color = cyl->color;
	ret.scene = cyl->scene;
	if (!top_bot)
		ret.v_normal = vec3_scale(ret.v_normal, -1);
	ret.center = vec3_add(ret.center, vec3_scale(ret.v_normal, cyl->height));
	ret.d = vec3_dot(ret.center, ret.v_normal);
	return ((t_element){.plane = ret});
}

static t_hit	_validate_cyl_hit(const t_element *e, t_hit *hit)
{
	t_vec3	p;
	t_vec3	v;

	if (isnan(hit->t))
		return (*hit);
	p = ray_to_vec3(hit->ray, hit->t);
	v = vec3_add(e->plane.center, vec3_scale(p, -1));
	if (vec3_length(v) < hit->e->cyl.radius)
		return (*hit);
	hit->t = NAN;
	return (*hit);
}

t_hit	cyl_cap_hit(const struct s_cylinder *cyl, t_ray *ray)
{
	t_element	cap;
	t_hit		top_hit;
	t_hit		bot_hit;

	cap = _cyl_caps(cyl, 1);
	top_hit = cap.hit(&cap, *ray);
	top_hit.e = cyl->as_element;
	top_hit = _validate_cyl_hit(&cap, &top_hit);
	cap = _cyl_caps(cyl, 0);
	bot_hit = cap.hit(&cap, *ray);
	bot_hit.e = cyl->as_element;
	bot_hit = _validate_cyl_hit(&cap, &bot_hit);
	if (isnan(bot_hit.t))
		return (top_hit);
	if (isnan(top_hit.t))
		return (bot_hit);
	if (top_hit.t > bot_hit.t)
		return (top_hit);
	return (bot_hit);
}

