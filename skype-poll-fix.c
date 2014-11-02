
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
#include <ctype.h>
#endif

#define _STRINGIZE(x) \
	#x
#define STRINGIZE(x) \
	_STRINGIZE(x)

#if defined(linux)
#	define POLL_FUNC_SIG struct pollfd *fds, nfds_t nfds, int timeout
#	define POLL_FUNC_NAME poll
#elif defined(__APPLE__)
#	define POLL_FUNC_SIG int kq,  const struct kevent *changelist, int nchanges, \
								struct kevent *eventlist, int nevents, \
								const struct timespec *timeout
#	define POLL_FUNC_NAME kevent
#else
#	error "bad platform"
#endif // if defined(linux)

/***************************************************************************/

static int is_only_digits(const char *str) {
	for ( ; *str; ++str) {
		if ( !isdigit(*str) )
			return 0;
	}

	return 1;
}

/***************************************************************************/

char* SET_POLL_C;
char* MIN_POLL_C;

int MIN_POLL = 300, SET_POLL = 300;

static __typeof__(&POLL_FUNC_NAME) pollmethod_orig = 0;

int POLL_FUNC_NAME(POLL_FUNC_SIG) {
	if (pollmethod_orig == NULL) {
		pollmethod_orig = dlsym(RTLD_NEXT, STRINGIZE(POLL_FUNC_NAME));
		MIN_POLL_C = getenv("MIN_POLL");
		SET_POLL_C = getenv("SET_POLL");
		if (MIN_POLL_C && is_only_digits(MIN_POLL_C)) {
			MIN_POLL = atoi(MIN_POLL_C);
		}
		if (SET_POLL_C && is_only_digits(SET_POLL_C)) {
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

	if (timeout->tv_nsec < MIN_POLL * 1000000) {
		new_timeout.tv_nsec = SET_POLL * 1000000;
	}
	else {
		new_timeout = *timeout;
	}
	return pollmethod_orig(kq, changelist, nchanges, eventlist, nevents, &new_timeout);
#endif
}

/***************************************************************************/
