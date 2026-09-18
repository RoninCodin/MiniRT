#ifndef SCENE_TYPES_H
# define SCENE_TYPES_H

# include "dynarr.h"
# include "ray.h"
# include "vector.h"

# include <stddef.h>

struct			s_scene;
union			u_element;

typedef t_vec3	(*t_normal_fn)(const union u_element *e, const t_vec3 *v_hit);

typedef enum e_elem_type
{
	RT_ELEM_INVALID,
	RT_ELEM_CYLINDER,
	RT_ELEM_PLANE,
	RT_ELEM_SPHERE,
	RT_ELEM_TEST,
	RT_ELEM_AMBIENT,
	RT_ELEM_CAMERA,
	RT_ELEM_LIGHT,
	RT_N_ELEMS,
}	t_elem_type;

struct s_cylinder
{
	t_elem_type		type;
	struct s_scene	*scene;
	union u_element	*as_element;
	t_vec3			color;
	t_normal_fn		normal;
	t_hit_fn		hit;
	t_vec3			center;
	t_vec3			axis;
	float			radius;
	float			height;
};

struct s_plane
{
	t_elem_type		type;
	struct s_scene	*scene;
	union u_element	*as_element;
	t_vec3			color;
	t_normal_fn		normal;
	t_hit_fn		hit;
	t_vec3			center;
	t_vec3			v_normal;
	float			d;
};

struct s_sphere
{
	t_elem_type		type;
	struct s_scene	*scene;
	union u_element	*as_element;
	t_vec3			color;
	t_normal_fn		normal;
	t_hit_fn		hit;
	t_vec3			center;
	float			radius;
};

typedef struct s_camera
{
	t_elem_type		type;
	struct s_scene	*scene;
	union u_element	*as_element;
	t_vec3			__color;
	t_normal_fn		__normal;
	t_hit_fn		__hit;
	union
	{
		t_ray		ray;
		struct
		{
			t_vec3	origin;
			t_vec3	heading;
		};
	};
	t_real			fov;
}	t_camera;

typedef struct s_light
{
	t_elem_type		type;
	struct s_scene	*scene;
	union u_element	*as_element;
	t_vec3			color;
	t_normal_fn		__normal;
	t_hit_fn		__hit;
	t_vec3			origin;
	t_real			brightness;
}	t_light;

typedef union u_element
{
	struct
	{
		t_elem_type		type;
		struct s_scene	*scene;
		union u_element	*as_element;
		t_vec3			color;
		t_normal_fn		normal;
		t_hit_fn		hit;
	};
	struct s_cylinder	cyl;
	struct s_plane		plane;
	struct s_sphere		sphere;
	struct s_camera		camera;
	struct s_light		light;
}	t_element;

typedef struct s_scene
{
	union
	{
		struct
		{
			t_element	**elements;
			size_t		size;
		};
		t_dynarr		*_;
	};
	t_light				ambient;
	t_light				light;
	t_camera			camera;
}	t_scene;

#endif

