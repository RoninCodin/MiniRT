#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

size_t	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		printf("gettimeofday Error\n");
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

