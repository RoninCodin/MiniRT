#ifndef UTILS_H
# define UTILS_H

# include "color.h"
# include "vector.h"

# include <stddef.h>
# include <unistd.h>

t_vec3	a_to_vec3(const char *s);
int		vec3_valid(const t_vec3 v);
int		vec3_valid_normal(const t_vec3 v);
int		vec3_valid_color(const char *str);
int		vec3_abs_lte(const t_vec3 v, const t_real max);
t_color	bg_checkerboard(const int x, const int y);
int		ft_rand(void);
size_t	get_time(void);
t_real	rt_atof(const char *str);
// __attribute__((format(printf, 1, 2)))
// ssize_t	ft_log(const char *format, ...);

#endif

