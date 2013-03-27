#include "motor.h"
void Motor_step(Motor *m)
{
    if(m->step == 1)
        m->step = 0;
    else
        m->step = 1;
    m->count++;
}
void Motor_enable(Motor *m)
{
    m->enable = 1;
}
void Motor_disable(Motor *m)
{
    m->enable = 0;
}
