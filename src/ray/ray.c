#include "MLX42.h"
#include "color.h"
#include "ft_linalg.h"
#include "ray.h"
#include "scene.h"
#include "scene_types.h"
#include "utils.h"
#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#define CHUNK_SIZE	100

extern const int	g_default_width;
extern const int	g_default_height;

size_t	get_time(void);
int		timeval(int curr_time, int time);

t_vec3	world_pos_gen(t_vec3 right, t_vec3 up, t_vec3 forward, t_ray *ray)
{
	t_vec3	world;
	t_vec3	rx;
	t_vec3	uy;
	t_vec3	fz;

	rx = vec3_scale(right, ray->heading.x);
	uy = vec3_scale(up, ray->heading.y);
	fz = vec3_scale(forward, ray->heading.z);
	world = vec3_add(vec3_add(rx, uy), fz);
	world = vec3_normalize(world);
	return (world);
}

static t_ray	_ray_gen(t_camera cam, int pixel_x, int pixel_y)
{
	t_ray	ray;
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	t_vec3	world_up;

	forward = vec3_normalize(cam.heading);
	if (forward.y > 0.8f || forward.y < -0.8f)
		world_up = (t_vec3){.x = 1, .y = 0, .z = 0};
	else
		world_up = (t_vec3){.x = 0, .y = 1, .z = 0};
	right = vec3_normalize(vec3_cross(world_up, forward));
	up = vec3_normalize(vec3_cross(forward, right));
	ray = cam.ray;
	ray.heading.x = ((2.0f * pixel_x / (t_real)g_default_width) - 1.0f);
	ray.heading.y = ((2.0f * pixel_y / (t_real)g_default_height) - 1.0f);
	ray.heading.x *= ((t_real)g_default_width / (t_real)g_default_height);
	ray.heading.xy = vec2_scale(ray.heading.xy, tan(cam.fov * M_PI / 360));
	ray.heading.z = -1.0f;
	ray.heading = vec3_normalize(ray.heading);
	ray.pixel = (t_vec2){.x = pixel_x, .y = pixel_y};
	ray.heading = world_pos_gen(right, up, forward, &ray);
	return (ray);
}

t_hit	ray_hit(struct s_scene *scene, t_ray ray, const union u_element *ignore)
{
	t_hit			hit;
	size_t			i;
	const t_element	*curr;
	t_hit			tophit;

	tophit = hit_init(NULL, ray);
	i = 0;
	while (i < scene->_->size)
	{
		curr = scene_get(*scene, i++);
		if (curr->type == RT_ELEM_INVALID || curr->hit == NULL
			|| (ignore && curr == ignore))
			continue ;
		hit = curr->hit(curr, ray);
		if (!isnan(hit.t) && !isgreater(tophit.t, hit.t))
			tophit = hit;
	}
	return (tophit);
}

t_color	ray_color(struct s_scene *scene, t_ray ray)
{
	t_hit	hit;

	hit = ray_hit(scene, ray, NULL);
	return (hit_color(&hit));
}

bool	ray_trace(mlx_image_t *image, t_scene *scene, int time)
{
	static t_vec2	pixel = (t_vec2){0};
	static size_t	curr_time = 0;

	curr_time = get_time();
	while (pixel.y < g_default_height)
	{
		pixel.x = 0;
		while (pixel.x < g_default_width)
		{
			mlx_put_pixel(image, pixel.x, pixel.y,
				ray_color(scene, _ray_gen(scene->camera, pixel.x, pixel.y)).c);
			pixel.x++;
		}
		pixel.y++;
		curr_time = get_time();
		if (timeval(curr_time, time) == -1)
			return (false);
	}
	return (true);
}

