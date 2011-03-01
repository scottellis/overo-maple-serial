/*
 A little test code for Gumstix to Maple serial communications at TTL 
 signal voltages.
 
 Using the first serial port which is TX = pin 10 and RX = pin 9 on
 the Gumstix Tobi expansion board. 

 Voltages are 3.3 on the Maple so need a level converter between the
 Gumstix and the Maple.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <time.h>

// #define DEBUG

#define TTY_PORT "/dev/ttyS0"

#define MAX_RX_RETRIES 5

int msleep(int ms);
int tx(int fd, const char *txbuff);
int rx(int fd, char *rxbuff, int max_chars);
int tty_init(struct termios *old_tio);
void tty_cleanup(int fd, struct termios *old_tio);

int main(int argc, char **argv)
{
	int fd, len;
	struct termios old_tio;
	char buff[64];

	fd = tty_init(&old_tio);

	if (fd < 0) 
		exit(1);

	memset(buff, 0, sizeof(buff));

	if (argc > 1) {
		strncpy(buff, argv[1], sizeof(buff) - 2);
		strcat(buff, "\n");
	}
	else {
		strcpy(buff, "maple-can-you-hear-me?\n");
	}

	printf("Sent: %s", buff);
	len = tx(fd, buff);

	if (len > 0) {
		msleep(50);

		memset(buff, 0, sizeof(buff));

		len = rx(fd, buff, sizeof(buff) - 1);

		if (len > 0)
			printf("Received: %s", buff);
		else
			printf("Maple not responding\n");
	}
	else {
		printf("tx() returned %d\n", len);
	}
	
	tty_cleanup(fd, &old_tio);

	close(fd);

	return 0;
}

int tx(int fd, const char *txbuff)
{
	int n;
	int pos = 0;
	int tx_len = strlen(txbuff);

	while (pos < tx_len) {
		n = write(fd, txbuff + pos, tx_len - pos);

#ifdef DEBUG
		printf("DEBUG: write() returned %d\n", n);
#endif
		if (n < 0) {
			perror("write");
			return -1;
		}

		pos += n;
	}

	tcflush(fd, TCIFLUSH);

	return pos;
}

/*
 Read until we get a '\n'
*/
int rx(int fd, char *rxbuff, int max_chars)
{
	int n, i;
	int pos = 0;

	for (i = 0; i < MAX_RX_RETRIES && pos < max_chars; i++) {
		n = read(fd, rxbuff + pos, max_chars - pos);

		if (n < 0) {
			perror("read");
			return -1;
		}

		if (n > 0) {
			pos += n;

			if (rxbuff[pos - 1] == '\n') {
#ifdef DEBUG
				printf("DEBUG: rx() returning %d chars\n", pos);
#endif
				return pos;
			}
		}

		msleep(50);
	}

#ifdef DEBUG
	printf("DEBUG: rx() timed out: %d chars received\n", pos);
#endif
	return -1;
}

int tty_init(struct termios *old_tio)
{
	int fd;

	struct termios tio;

	if ((fd = open(TTY_PORT, O_RDWR | O_NOCTTY | O_NDELAY)) < 0) {
		perror("open tty");
		return -1;
	}

	if (fcntl(fd, F_SETFL, 0) == -1) {
		perror("fnctl(,F_SETFL,)");
		close(fd);
		return -1;
	}

	tcflush(fd, TCIFLUSH);
	tcgetattr(fd, old_tio);
	bzero(&tio, sizeof(tio));
	tio.c_iflag = IGNPAR | IGNBRK;
	tio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	tcsetattr(fd, TCSANOW, &tio);

	return fd;
}

void tty_cleanup(int fd, struct termios *old_tio)
{
	if (fd && old_tio) {
		tcflush(fd, TCIFLUSH);
		tcsetattr(fd, TCSANOW, old_tio);
	}
}

int msleep(int ms)
{
	struct timespec ts;

	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;

	return nanosleep(&ts, NULL);
}

