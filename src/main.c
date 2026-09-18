#include "MLX42.h"
#include "ray.h"
#include "scene.h"
#include "scene_types.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utils.h>

const int			g_default_width = 1600;
const int			g_default_height = 900;
const char			*g_default_title = "miniRT";

static void	_init(mlx_t **mlx, mlx_image_t **image)
{
	*mlx = mlx_init(g_default_width, g_default_height, "MiniRT", false);
	if (!mlx)
		exit(1);
	*image = mlx_new_image(*mlx, g_default_width, g_default_height);
	if (!image)
		exit(1);
}

void	key_hook(mlx_key_data_t key_data, void *param)
{
	mlx_t	**mlx;

	mlx = ((void **)param)[0];
	if (key_data.key == MLX_KEY_ESCAPE && key_data.action == MLX_RELEASE)
		mlx_close_window(*mlx);
}

void	loop_hook(void *params)
{
	static bool	finished = false;
	mlx_image_t	**image;
	t_scene		*scene;

	image = ((void **)params)[1];
	scene = ((void **)params)[2];
	if (!finished)
		finished = ray_trace(*image, scene, get_time());
}

int	main(int argc, char *argv[])
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_scene		scene;
	const void	*params[] = {&mlx, &image, &scene};

	if (argc != 2)
		exit(1);
	if (!scene_from_file(argv[1], &scene))
		exit(1);
	_init(&mlx, &image);
	printf("Tracing...\n");
	mlx_loop_hook(mlx, loop_hook, params);
	mlx_key_hook(mlx, key_hook, params);
	mlx_image_to_window(mlx, image, 0, 0);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	scene_free(&scene);
	return (0);
}

