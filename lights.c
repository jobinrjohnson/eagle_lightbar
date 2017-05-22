#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define TIMEOUT 1000
#define ERR -1

char const* const RED_ILLUMINATION_FILE = "/sys/class/illumination/7";
char const* const GREEN_ILLUMINATION_FILE = "/sys/class/illumination/8";
char const* const BLUE_ILLUMINATION_FILE = "/sys/class/illumination/6";
char const* const ILLUMINATION_BLINK_ON_FILE = "/sys/class/illumination/2";
char const* const ILLUMINATION_BLINK_OFF_FILE = "/sys/class/illumination/1";
char const* const ILLUMINATION_FILE = "/sys/class/illumination/4";

int IS_PLAYING = 0;

static int write_int(char const* path, int value) {
	int fd;
//	printf("%d,",value);
	fd = open(path, O_WRONLY);
	if (fd >= 0) {
		char buffer[20] = { 0, };
		int bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
		ssize_t amt = write(fd, buffer, (size_t) bytes);
		close(fd);
		return amt == -1 ? -1 : 0;
	} else {
//		printf("write_int failed to open %s\n", path);
		return -1;
	}
}

int random_number(int min_num, int max_num) {
	int result = 0, low_num = 0, hi_num = 0;

	if (min_num < max_num) {
		low_num = min_num;
		hi_num = max_num + 1; // include max_num in output
	} else {
		low_num = max_num + 1; // include max_num in output
		hi_num = min_num;
	}
	//srand(time(NULL));
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

int main() {


	int fh;
	char buffer[15];
	int gotten;
	for (;;) {
//		char status[1000] = "";
//		while ((gotten = read(fh, buffer, 64))) {
//			buffer[gotten] = '\0';
//			strcat(status, buffer);
//		}
		fh = open("/proc/asound/card0/pcm0p/sub0/status", O_RDONLY);
		gotten = read(fh, buffer, 14);
		buffer[gotten] = '\0';

		trim(buffer);
		printf("%s", buffer);


		}
		fflush(stdout);
//		fflush(stderr);
		sleep(2);
	}
	return 0;

}