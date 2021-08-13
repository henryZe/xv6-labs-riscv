#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64 sys_sysinfo(void)
{
  struct proc *p = myproc();
  struct sysinfo info;
  uint64 dst_info;

  if (argaddr(0, &dst_info) < 0)
    return -1;

  info.freemem = cal_kfreepages() * PGSIZE;
  info.nproc = cal_kfreeprocs();

  return copyout(p->pagetable, dst_info, (char *)&info, sizeof(struct sysinfo));
}
