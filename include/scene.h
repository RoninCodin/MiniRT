#ifndef SCENE_H
# define SCENE_H

# include "ray.h"
# include "scene_types.h"

t_scene			scene_new(void);
void			scene_free(t_scene *s);
size_t			scene_add(t_scene *s, t_element *e);
const t_element	*scene_get(t_scene s, size_t idx);

typedef t_element	(*t_elem_parser)(const char **s);
bool			scene_from_file(const char *file, t_scene *dest);

#endif

