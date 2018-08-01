#include "sys_public.h"
#include <QString>
#include <QDebug>

sys_public::sys_public()
{

}

unsigned char sys_public::Is_true(unsigned char x)
{
    if(x)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


