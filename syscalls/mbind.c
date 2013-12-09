/*
 * SYSCALL_DEFINE6(mbind, unsigned long, start, unsigned long, len,
	unsigned long, mode, unsigned long __user *, nmask,
	unsigned long, maxnode, unsigned, flags)
 */

#include <linux/mempolicy.h>
#include "arch.h"
#include "maps.h"
#include "random.h"
#include "sanitise.h"
#include "shm.h"
#include "utils.h"	// page_size

#define MPOL_F_STATIC_NODES     (1 << 15)
#define MPOL_F_RELATIVE_NODES   (1 << 14)

static void sanitise_mbind(int childno)
{
	struct map *map;
	unsigned long maxnode;

	map = (struct map *) shm->a1[childno];
	shm->scratch[childno] = (unsigned long) map;    /* Save this for ->post */

	shm->a1[childno] = (unsigned long) map->ptr;
	shm->a2[childno] = map->size;           //TODO: Munge this.

retry_maxnode:
	shm->a5[childno] &= ~((page_size * 8) - 1);

	maxnode = shm->a5[childno];

	if (maxnode < 2 || maxnode > (page_size * 8)) {
		shm->a5[childno] = rand32();
		goto retry_maxnode;
	}
}


struct syscall syscall_mbind = {
	.name = "mbind",
	.num_args = 6,
	.arg1name = "start",
	.arg1type = ARG_MMAP,

	.arg2name = "len",

	.arg3name = "mode",
	.arg3type = ARG_LIST,
	.arg3list = {
		.num = 4,
		.values = { MPOL_DEFAULT, MPOL_BIND, MPOL_INTERLEAVE, MPOL_PREFERRED },
	},

	.arg4name = "nmask",
	.arg4type = ARG_ADDRESS,

	.arg5name = "maxnode",
	.arg5type = ARG_RANGE,
	.low5range = 0,
	.hi5range = 32,

	.arg6name = "flags",
	.arg6type = ARG_LIST,
	.arg6list = {
		.num = 2,
		.values = { MPOL_F_STATIC_NODES, MPOL_F_RELATIVE_NODES },
	},
	.sanitise = sanitise_mbind,
	.group = GROUP_VM,
};
