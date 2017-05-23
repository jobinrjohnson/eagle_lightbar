#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>

#define TIMEOUT 2

char const* const RED_ILLUMINATION_FILE = "/sys/class/illumination/7";
char const* const GREEN_ILLUMINATION_FILE = "/sys/class/illumination/8";
char const* const BLUE_ILLUMINATION_FILE = "/sys/class/illumination/6";
char const* const ILLUMINATION_BLINK_ON_FILE = "/sys/class/illumination/2";
char const* const ILLUMINATION_BLINK_OFF_FILE = "/sys/class/illumination/1";
char const* const ILLUMINATION_FILE = "/sys/class/illumination/4";

int IS_PLAYING = 0;

static int write_int(char const* path, int value) {
	int fd;
	fd = open(path, O_WRONLY);
	if (fd >= 0) {
		char buffer[20] = { 0, };
		int bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
		ssize_t amt = write(fd, buffer, (size_t) bytes);
		close(fd);
		return amt == -1 ? -1 : 0;
	} else {
		return -1;
	}
}

int random_number(int min_num, int max_num) {
	int result = 0, low_num = 0, hi_num = 0;

	if (min_num < max_num) {
		low_num = min_num;
		hi_num = max_num + 1;
	} else {
		low_num = max_num + 1;
		hi_num = min_num;
	}
	result = (rand() % (hi_num - low_num)) + low_num;
	return result;
}

void trim(char *str) {
	char *start, *end;
	for (start = str; *start; start++) {
		if (!isspace((unsigned char) start[0]))
			break;
	}
	for (end = start + strlen(start); end > start + 1; end--) {
		if (!isspace((unsigned char) end[-1]))
			break;
	}
	*end = 0;
	if (start > str)
		memmove(str, start, (end - start) + 1);
}

void* write_random_values(void* args) {
	int r = random_number(20, 255);
	int g = random_number(20, 255);
	int b = random_number(20, 255);
	while (IS_PLAYING == 1) {
		write_int(RED_ILLUMINATION_FILE, r--);
		write_int(GREEN_ILLUMINATION_FILE, g--);
		write_int(BLUE_ILLUMINATION_FILE, b--);
		r = r < 1 ? random_number(0, 255) : r;
		g = g < 1 ? random_number(0, 255) : g;
		b = b < 1 ? random_number(0, 255) : b;
		if(IS_PLAYING == 1){
			usleep(50 * 1000);
		}
	}
	return NULL;
}

int main() {

	pthread_t player;
	int fh;
	char buffer[15];
	int gotten;
	for (;;) {
		fh = 0;
		fh = open("/proc/asound/card0/pcm0p/sub0/status", O_RDONLY);
		if (fh > 0) {
			gotten = read(fh, buffer, 14);
			buffer[gotten] = '\0';

			trim(buffer);
			if (strcmp(buffer, "closed") == 0) {
				if (IS_PLAYING == 1) {
					IS_PLAYING = 0;
					write_int(ILLUMINATION_FILE, 0);
					pthread_join(player, NULL);
				}
			} else {

				if (IS_PLAYING != 1) {
					write_int(ILLUMINATION_FILE, 21);
					pthread_create(&player, NULL, write_random_values, NULL);
					IS_PLAYING = 1;
				}

			}
			close(fh);
		}
		sleep(TIMEOUT);
	}
	write_int(ILLUMINATION_FILE, 0);
	pthread_join(player, NULL);
	return 0;

}
