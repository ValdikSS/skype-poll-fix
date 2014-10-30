#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#ifdef linux
#include <poll.h>
#endif

#ifdef __APPLE__
#include <sys/event.h>
#include <sys/time.h>
#endif

char* SET_POLL_C;
char* MIN_POLL_C;

int MIN_POLL = 300, SET_POLL = 300;

#ifdef linux
static int (*pollmethod_orig)(struct pollfd *fds, nfds_t nfds, int timeout);
int poll(struct pollfd *fds, nfds_t nfds, int timeout) {
#endif

#ifdef __APPLE__
static int (*pollmethod_orig)(int kq,  const struct kevent *changelist, int nchanges,
		struct kevent *eventlist, int nevents, const struct timespec *timeout);
int kevent(int kq,  const struct kevent *changelist, int nchanges, 
		struct kevent *eventlist, int nevents, const struct timespec *timeout) {
#endif

	if (pollmethod_orig == NULL) {
#ifdef linux
		pollmethod_orig = dlsym(RTLD_NEXT, "poll");
#endif
#ifdef __APPLE__
		pollmethod_orig = dlsym(RTLD_NEXT, "kevent");
#endif
		MIN_POLL_C = getenv("MIN_POLL");
		SET_POLL_C = getenv("SET_POLL");
		if (MIN_POLL_C) {
			MIN_POLL = atoi(MIN_POLL_C);
		}
		if (SET_POLL_C) {
			SET_POLL = atoi(SET_POLL_C);
		}
	}
#ifdef linux
	if (timeout < MIN_POLL) {
		timeout = SET_POLL;
	}
	return pollmethod_orig(fds, nfds, timeout);
#endif
#ifdef __APPLE__
	struct timespec new_timeout;
	new_timeout.tv_nsec = SET_POLL * 1000000;
	if (timeout->tv_nsec < MIN_POLL * 1000000) {
		return pollmethod_orig(kq, changelist, nchanges, eventlist, nevents, &new_timeout);
	}
#endif
}
