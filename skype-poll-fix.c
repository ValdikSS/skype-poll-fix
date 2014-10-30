#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <poll.h>


char* SET_POLL_C;
char* MIN_POLL_C;

unsigned int MIN_POLL, SET_POLL = 300;

static int (*poll_orig)(struct pollfd *fds, nfds_t nfds, int timeout);

int poll(struct pollfd *fds, nfds_t nfds, int timeout) {
	if (poll_orig == NULL) {
		poll_orig = dlsym(RTLD_NEXT, "poll");
		MIN_POLL_C = getenv("MIN_POLL");
		SET_POLL_C = getenv("SET_POLL");
		if (MIN_POLL_C) {
			MIN_POLL = atoi(MIN_POLL_C);
		}
		if (SET_POLL_C) {
			SET_POLL = atoi(SET_POLL_C);
		}
	}
	if (timeout < MIN_POLL) {
		timeout = SET_POLL;
	}
	return poll_orig(fds, nfds, timeout);
}
