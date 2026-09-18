#ifndef RAY_H
# define RAY_H

# include "MLX42.h"
# include "color.h"
# include "ft_linalg.h"
# include "vector.h"
# include <stddef.h>

typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	heading;
	t_vec2	pixel;
}	t_ray;

typedef struct s_hit
{
	t_real					t;
	t_vec3					coords;
	t_vec3					normal;
	t_color					color;
	t_ray					ray;
	const union u_element	*e;
}	t_hit;

typedef t_hit	(*t_hit_fn)(const union u_element *e, const t_ray);

struct			s_scene;

t_vec3			ray_to_vec3(const t_ray r, const t_real t);
t_ray			ray_reflect(t_ray r, t_ray n);

t_color			ray_color(struct s_scene *s, t_ray r);
t_hit			ray_hit(struct s_scene *s, t_ray r, const union u_element *ig);
bool			ray_trace(mlx_image_t *img, struct s_scene *s, int t);

t_hit			hit_init(const union u_element *e, const t_ray ray);
int				validate_hit(const union u_element *e, t_hit *hit);
t_color			hit_color(const t_hit *hit);

#endif

