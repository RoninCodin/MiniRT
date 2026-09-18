#ifndef DYNARR_H
# define DYNARR_H

# include <stddef.h>

# define DYNARR_DEFAULT_SIZE 32

typedef struct s_dynarr
{
	void			*d;
	size_t			size;
	size_t			capacity;
	size_t			elem_size;
	struct s_dynarr	**_me;
}	t_dynarr;

t_dynarr	*dynarr_new(size_t size, size_t elem_size);
void		dynarr_claim(t_dynarr *v, t_dynarr **owner);
t_dynarr	*dynarr_copy(t_dynarr *src);
void		*dynarr_free(t_dynarr **v);
// modify.c
size_t		dynarr_pushback(t_dynarr *v, void *d);
size_t		dynarr_insert(t_dynarr *v, size_t pos, void *d);
void		*dynarr_popback(t_dynarr *v);
void		*dynarr_erase(t_dynarr *v, size_t pos);
// access.c
void		*dynarr_at(t_dynarr *v, size_t pos);
void		*dynarr_front(t_dynarr *v);
void		*dynarr_back(t_dynarr *v);

#endif // DYNARR_H

