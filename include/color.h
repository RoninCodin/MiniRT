#ifndef COLOR_H
# define COLOR_H

# include "vector.h"

# include <stdint.h>

# ifdef __BYTE_ORDER__
#  if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

typedef union u_color
{
	struct
	{
		uint8_t	a;
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
	};
	uint32_t	c;
}	t_color;
#  else

typedef union u_color
{
	struct
	{
		uint8_t	r;
		uint8_t	g;
		uint8_t	b;
		uint8_t	a;
	};
	uint32_t	c;
}	t_color;
#  endif // __BYTE_ORDER == __ORDER_LITTLE_ENDIAN__
# endif //__BYTE_ORDER__

t_color	vec3_to_color(t_vec3 v);
t_vec3	color_multiply(const t_vec3 a, const t_vec3 b);

#endif // COLOR_H

