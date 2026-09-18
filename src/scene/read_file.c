#include "dynarr.h"
#include "libft.h"
#include "scene.h"
#include "scene_types.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

t_element	parse_sphere(const char **s);
t_element	parse_plane(const char **s);
t_element	parse_camera(const char **s);
t_element	parse_ambient(const char **s);
t_element	parse_light(const char **s);
t_element	parse_cylinder(const char **s);

static t_scene	_init(int *fd, const char *file)
{
	t_scene	res;

	*fd = open(file, O_RDONLY);
	if (*fd < 0)
	{
		perror(__FUNCTION__);
		exit(1);
	}
	res = scene_new();
	if (!res._)
	{
		perror("can't allocate scene");
		exit(1);
	}
	res.ambient.type = RT_ELEM_INVALID;
	res.camera.type = RT_ELEM_INVALID;
	res.light.type = RT_ELEM_INVALID;
	return (res);
}

static char	*str_spacify(const char *line)
{
	int		i;
	char	*string;

	string = (char *)line;
	i = 0;
	while (string[i] != '\0')
	{
		if (ft_isspace(string[i]))
			string[i] = ' ';
		i++;
	}
	return (string);
}

static t_element	*_parse_line(const char *line, t_element *dest)
{
	static t_elem_parser	table[256] = {
	['A'] = parse_ambient, ['C'] = parse_camera, ['L'] = parse_light,
	['p'] = parse_plane, ['s'] = parse_sphere, ['c'] = parse_cylinder,
	};
	char					**split;
	t_elem_parser			fn;

	dest->type = RT_ELEM_INVALID;
	line = str_spacify(line);
	line = ft_strtrim(line, " ");
	split = ft_split(line, ' ');
	free((void *)line);
	if (!split || !*split)
		return (NULL);
	fn = table[(unsigned char)split[0][0]];
	if (!fn && !ft_split_free(split))
		return (NULL);
	*dest = fn((const char **)split);
	ft_split_free(split);
	return (dest);
}

static bool	_scene_from_file(t_scene *dest, char *line)
{
	t_element	next_elem;

	if (!_parse_line(line, &next_elem))
		return (ft_printf("Error: invalid line format: %s\n", line),
			scene_free(dest), false);
	if (next_elem.type == RT_ELEM_INVALID)
		return (ft_printf("Error: invalid element at line: %s\n", line),
			scene_free(dest), false);
	next_elem.scene = dest;
	if (scene_add(dest, &next_elem) <= 0)
		return (ft_printf ("Error: unique element appears twice at line: %s\n",
				line), scene_free(dest), false);
	return (true);
}

bool	scene_from_file(const char *file, t_scene *dest)
{
	int			fd;
	char		*line;

	*dest = _init(&fd, file);
	line = ft_gnl(fd);
	while (line && *line)
	{
		if (line[0] == '\n' || line[0] == '#')
		{
			free(line);
			line = ft_gnl(fd);
			continue ;
		}
		if (!_scene_from_file(dest, line))
			return (free(line), close(fd), scene_free(dest), (false));
		free(line);
		line = ft_gnl(fd);
	}
	free(line);
	close(fd);
	if (dest->camera.type != RT_ELEM_CAMERA)
		return (ft_printf("Error: No camera in .rt file\n"),
			scene_free(dest), (false));
	dynarr_claim(dest->_, &dest->_);
	return (true);
}

