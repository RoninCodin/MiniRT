#include "color.h"
#include "libft.h"
#include "vector.h"
#include <stddef.h>

__attribute__((unused))
static t_vec3	_clamp_vec3(t_vec3 v)
{
	if (v.x > 1)
		v.x = 1;
	if (v.y > 1)
		v.y = 1;
	if (v.z > 1)
		v.z = 1;
	return (v);
}

t_color	vec3_to_color(t_vec3 v)
{
	t_color	c;

	v = _clamp_vec3(v);
	c.r = v.x * 255;
	c.g = v.y * 255;
	c.b = v.z * 255;
	c.a = 255;
	return (c);
}

int	vec3_valid_color(const char *str)
{
	int	n;
	int	i;

	i = 0;
	while (*str)
	{
		n = ft_atoi(str);
		if (n < 0 || n > 255)
			return (0);
		while (ft_isdigit(*str))
			++str;
		if (++i >= 3)
			return (*str == '\0');
		else if (*(str++) != ',')
			return (0);
	}
	return (0);
}

t_vec3	color_multiply(const t_vec3 a, const t_vec3 b)
{
	return ((t_vec3){.v = a.v * b.v});
}

t_color	bg_checkerboard(const int x, const int y)
{
	const int	size = 10;
	int			_x;
	int			_y;

	_x = (x / size) % 2;
	_y = (y / size) % 2;
	if (!(_x && _y) && (_x || _y))
		return ((t_color){.r = 50, .g = 50, .b = 50, .a = 255});
	return ((t_color){.r = 200, .g = 0, .b = 200, .a = 255});
}

