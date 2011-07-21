/*
 * SYSCALL_DEFINE4(epoll_ctl, int, epfd, int, op, int, fd, struct epoll_event __user *, event)
 */
#include <sys/epoll.h>

#include "trinity.h"
#include "sanitise.h"

struct syscall syscall_epoll_ctl = {
	.name = "epoll_ctl",
	.num_args = 4,
	.arg1name = "epfd",
	.arg1type = ARG_FD,
	.arg2name = "op",
	.arg2type = ARG_LIST,
	.arg2list = {
		.num = 3,
		.values = { EPOLL_CTL_ADD, EPOLL_CTL_MOD, EPOLL_CTL_DEL },
	},
	.arg3name = "fd",
	.arg3type = ARG_FD,
	.arg4name = "event",
	.arg4type = ARG_ADDRESS,
};
