#include "f280015x_device.h"

void globalInterruptEnable(void)
{
    EINT;
}

void globalInterruptDisable(void)
{
    DINT;
}
