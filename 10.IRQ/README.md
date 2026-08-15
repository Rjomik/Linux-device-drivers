cat /proc/interrupts
mod_timer - modify timer, if timer already running it is restarted.
    (when (*expired > clicking frequency) the callback never fires)

schedule_work(struct work_struct*) - protects from double pending, but can result into running&pedning
    if pending bit == 0 
        then pending = 1
    else 
        ignore

Output:
[ 3333.551452] Button pressed!
[ 3333.551544] Starting a work - pedning bit 0, running bit 1
[ 3333.730646] Button pressed! - sets pending bit to 1
[ 3333.895841] Button pressed! - ingored
[ 3334.047796] Button pressed! - ingored
[ 3334.195218] Button pressed! - ingored
[ 3334.336622] Button pressed! - ingored
[ 3334.590270] Button pressed! - ingored
[ 3334.731764] Button pressed! - ingored
[ 3334.914779] Button pressed! - ingored
[ 3335.181388] Button pressed! - ingored
[ 3335.333254] Button pressed! - ingored
[ 3336.671148] Switching a led - sleep end, quits the work, checks the pending bit
[ 3336.671187] Starting a work - Oh sh*t I am pedning, run again
[ 3339.743113] Switching a led - another sleep end