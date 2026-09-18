#include "libft.h"
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RAND_BUFF_SIZE	1000

int	ft_rand(void)
{
	static int		ret[RAND_BUFF_SIZE] = {0};
	static size_t	i = RAND_BUFF_SIZE;
	int				fd;

	if (i >= RAND_BUFF_SIZE)
	{
		i = 0;
		fd = open("/dev/urandom", O_RDONLY);
		if (fd < 0)
		{
			ft_print_err(strerror(errno), __FILE_NAME__, __FUNCTION__, NULL);
			exit(EXIT_FAILURE);
		}
		read(fd, ret, RAND_BUFF_SIZE * sizeof(ret[0]));
	}
	return (ret[i++]);
}

