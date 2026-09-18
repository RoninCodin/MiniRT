#include "dynarr.h"
#include "libft.h"

#include <stdlib.h>

static size_t	_dynarr_extend(t_dynarr *v)
{
	void	*new;

	if (!v)
		return (0);
	new = ft_realloc(v->d, 2 * v->capacity * v->elem_size,
			v->size * v->elem_size);
	if (new == v->d)
		return (v->capacity);
	v->d = new;
	v->capacity *= 2;
	return (v->capacity);
}

size_t	dynarr_pushback(t_dynarr *v, void *d)
{
	if (!v || ((v->_me) && !*v->_me))
		return (0);
	if (!d)
		return (v->size);
	if (v->size == v->capacity && v->capacity >= _dynarr_extend(v))
		return (v->size);
	ft_memmove(v->d + v->size * v->elem_size, d, v->elem_size);
	return (++v->size);
}

size_t	dynarr_insert(t_dynarr *v, size_t pos, void *d)
{
	if (!v || ((v->_me) && !*v->_me))
		return (0);
	if (!d || pos > v->size
		|| (v->size == v->capacity && v->capacity >= _dynarr_extend(v)))
		return (v->size);
	ft_memmove(v->d + (pos + 1) * v->elem_size,
		v->d + pos * v->elem_size, (v->size - pos) * v->elem_size);
	if (d >= v->d + (pos * v->elem_size)
		&& d < v->d + (v->size * v->elem_size))
		d += v->elem_size;
	ft_memmove(v->d + pos * v->elem_size, d, v->elem_size);
	return (++v->size);
}

void	*dynarr_popback(t_dynarr *v)
{
	void	*ret;

	if (!v || ((v->_me) && !*v->_me))
		return (NULL);
	ret = dynarr_back(v);
	--v->size;
	return (ret);
}

void	*dynarr_erase(t_dynarr *v, size_t pos)
{
	if (!v || ((v->_me) && !*v->_me))
		return (NULL);
	if (pos >= v->size)
		return (NULL);
	if (pos == v->size - 1)
		--v->size;
	else
		ft_memmove(dynarr_at(v, pos),
			dynarr_at(v, pos + 1), (--v->size - pos) * v->elem_size);
	return (dynarr_at(v, pos));
}

