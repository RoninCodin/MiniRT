#include "dynarr.h"
#include "libft.h"
#include "scene_types.h"

#include <stddef.h>

t_scene	scene_new(void)
{
	t_scene	res;

	res._ = dynarr_new(8, sizeof(t_element));
	return (res);
}

void	scene_free(t_scene *s)
{
	if (s->_)
		dynarr_free(&s->_);
	s->_ = NULL;
	s->elements = NULL;
}

size_t	scene_add_unique(t_scene *s, t_element *e)
{
	static int	table[RT_N_ELEMS] = {0};

	if (e->type == RT_ELEM_INVALID || e->type >= RT_N_ELEMS
		|| table[e->type] == 1)
		return (0);
	if (e->type == RT_ELEM_AMBIENT && !table[RT_ELEM_AMBIENT])
	{
		s->ambient = e->light;
		table[RT_ELEM_AMBIENT] = 1;
	}
	else if (e->type == RT_ELEM_LIGHT && !table[RT_ELEM_LIGHT])
	{
		s->light = e->light;
		table[RT_ELEM_LIGHT] = 1;
	}
	else if (e->type == RT_ELEM_CAMERA && !table[RT_ELEM_CAMERA])
	{
		s->camera = e->camera;
		table[RT_ELEM_CAMERA] = 1;
	}
	return (1);
}

size_t	scene_add(t_scene *s, t_element	*e)
{
	static const int	_u[RT_N_ELEMS] = {
	[RT_ELEM_CAMERA] = 1, [RT_ELEM_AMBIENT] = 1, [RT_ELEM_LIGHT] = 1
	};

	if (e->type < RT_N_ELEMS && _u[e->type])
		return (scene_add_unique(s, e));
	return (dynarr_pushback(s->_, e));
}

const t_element	*scene_get(t_scene s, size_t idx)
{
	t_element	*res;

	ft_assert(s._->size > idx, __FUNCTION__, "idx out of range", NULL);
	res = dynarr_at(s._, idx);
	res->as_element = res;
	return (res);
}

