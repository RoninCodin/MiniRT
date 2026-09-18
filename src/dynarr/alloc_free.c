#include "dynarr.h"
#include "libft.h"

#include <stdlib.h>

// TODO: FIX CLAIM THINGY
t_dynarr	*dynarr_new(size_t capacity, size_t elem_size)
{
	t_dynarr	*res;

	ft_assert(elem_size > 0, __FILE__, __FUNCTION__,
		"element size can't be zero", NULL);
	res = malloc(sizeof(t_dynarr));
	if (!res)
		return (NULL);
	res->elem_size = elem_size;
	res->size = 0;
	res->_me = NULL;
	if (capacity > 0)
		res->capacity = capacity;
	else
		res->capacity = DYNARR_DEFAULT_SIZE;
	res->d = malloc(res->capacity * res->elem_size);
	if (!res->d)
	{
		free(res);
		return (NULL);
	}
	return (res);
}

void	dynarr_claim(t_dynarr *v, t_dynarr **owner)
{
	ft_assert(!!v, __FILE__, __FUNCTION__, "v is NULL", NULL);
	ft_assert(!v->_me,
		__FILE__, __FUNCTION__, "dynarr already claimed", NULL);
	ft_assert(!!owner, __FILE__, __FUNCTION__, "owner is NULL", NULL);
	if (!v || (v->_me && !*v->_me) || !owner)
		return ;
	v->_me = owner;
}

t_dynarr	*dynarr_copy(t_dynarr *src)
{
	t_dynarr	*res;

	ft_assert(!!src, __FILE_NAME__, __FUNCTION__, "src is NULL", NULL);
	ft_assert(!(src->_me && !*src->_me),
		__FILE_NAME__, __FUNCTION__, "dynarr marked as freed", NULL);
	res = dynarr_new(src->size, src->elem_size);
	if (!res)
		return (NULL);
	res->_me = NULL;
	ft_memcpy(res->d, src->d, src->size * src->elem_size);
	res->capacity = src->capacity;
	res->elem_size = src->elem_size;
	res->size = src->size;
	return (res);
}

void	*dynarr_free(t_dynarr **p)
{
	t_dynarr	*v;

	ft_assert(!!p, __FILE_NAME__, __FUNCTION__, "p is NULL", NULL);
	if (!p)
		return (NULL);
	v = *p;
	if (v->_me && v->_me != p)
		return (NULL);
	free(v->d);
	free(v);
	v = NULL;
	return (NULL);
}

