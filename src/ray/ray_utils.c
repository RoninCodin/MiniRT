#include "ft_linalg.h"
#include "ray.h"

t_vec3	ray_to_vec3(const t_ray r, const t_real t)
{
	return (vec3_add(r.origin, vec3_scale(r.heading, t)));
}

t_ray	ray_reflect(t_ray r, t_ray n)
{
	t_ray	_r;

	_r.origin = n.origin;
	_r.heading = vec3_scale(n.heading, -2 * vec3_dot(r.heading, n.heading));
	_r.heading = vec3_add(r.heading, _r.heading);
	return (_r);
}

int	timeval(int curr_time, int time)
{
	if (curr_time - time > 50)
	{
		return (-1);
	}
	else
	{
		return (curr_time);
	}
}

