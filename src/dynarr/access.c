#include "dynarr.h"
#include "libft.h"

void	*dynarr_at(t_dynarr *v, size_t pos)
{
	ft_assert(!!v, __FILE__, __FUNCTION__, "v is NULL", NULL);
	ft_assert(!((v->_me) && !*v->_me),
		__FILE__, __FUNCTION__, "dynarr marked as freed", NULL);
	ft_assert(v->size > pos,
		__FILE__, __FUNCTION__, "pos out of range", NULL);
	return (v->d + pos * v->elem_size);
}

void	*dynarr_front(t_dynarr *v)
{
	ft_assert(!!v, __FILE__, __FUNCTION__, "v is NULL", NULL);
	ft_assert(!((v->_me) && !*v->_me),
		__FILE__, __FUNCTION__, "dynarr marked as freed", NULL);
	ft_assert(v->size == 0,
		__FILE__, __FUNCTION__, "dynarr empty", NULL);
	return (v->d);
}

void	*dynarr_back(t_dynarr *v)
{
	ft_assert(!!v, __FILE__, __FUNCTION__, "v is NULL", NULL);
	ft_assert(!((v->_me) && !*v->_me),
		__FILE__, __FUNCTION__, "dynarr marked as freed", NULL);
	ft_assert(v->size == 0,
		__FILE__, __FUNCTION__, "dynarr empty", NULL);
	return (dynarr_at(v, v->size - 1));
}

