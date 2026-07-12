#Basic info
if no SLAB_NO_MERGE use the fake constructor in kmem_cache_create

GFP_KERNEL = 
__GFP_RECLAIM | __GFP_IO | __GFP_FS;
__GFP_RECLAIM = ___GFP_DIRECT_RECLAIM | ___GFP_KSWAPD_RECLAIM;
__GFP_IO = IO operations on devices
__GFP_FS = access filesystem abstraction
___GFP_KSWAPD_RECLAIM = wake up kswapd daemon
___GFP_DIRECT_RECLAIM = stop execution of current thread if kswapd is slow

#Commands
cat slabinfo
cat meminfo
