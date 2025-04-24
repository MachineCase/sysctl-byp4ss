#include <stdio.h>
#include <unistd.h>
#include <sys/sysctl.h>

#define DYLD_INTERPOSE(_replacement,_replacee) \
   __attribute__((used)) static struct{ const void* replacement; const void* replacee; } _interpose_##_replacee \
            __attribute__ ((section ("__DATA,__interpose"))) = { (const void*)(unsigned long)&_replacement, (const void*)(unsigned long)&_replacee };


static int (*orig_systctl)(const  int *,  u_int, void *, size_t *, const void *, size_t);

int my_sysctl(const  int  *name,  u_int	 namelen, void *oldp, size_t *oldlenp, const void *newp, size_t newlen) {
    if (namelen == 4 && name[0] == CTL_KERN && name[1] == KERN_PROC && 
        name[2] == KERN_PROC_PID && name[3] == getpid()) {
            printf("Anti-debug detected and bypassed\n");
            return 0;
        }

        return orig_systctl(name, namelen, oldp, oldlenp, newp, newlen);
}

DYLD_INTERPOSE(my_sysctl, sysctl);


