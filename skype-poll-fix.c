#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>
#include <poll.h>

static int (*poll_orig)(struct pollfd *fds, nfds_t nfds, int timeout);

int poll(struct pollfd *fds, nfds_t nfds, int timeout) {
    if (poll_orig == NULL) {
        poll_orig = dlsym(RTLD_NEXT, "poll");
    }
    if (timeout < 300) {
        timeout = 300;
    }
    return poll_orig(fds, nfds, timeout);
}
