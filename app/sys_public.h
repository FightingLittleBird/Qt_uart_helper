#ifndef SYS_PUBLIC_H
#define SYS_PUBLIC_H

#include <QString>
#include <QDebug>

class sys_public
{

#define bit(n) (1 << n)

#define bit_true(x,mask) (x) |= (mask)
#define bit_false(x,mask) (x) &= ~(mask)
#define bit_istrue(x,mask) ((x & mask) != 0)
#define bit_isfalse(x,mask) ((x & mask) == 0)

#define MASTER_BIT  bit(0) // bitmask 00000001
#define CPHA_BIT    bit(1) // bitmask 00000010
#define CPOL_BIT     bit(2) // bitmask 00000100
#define CS_BIT      bit(3) // bitmask 00001000
#define LSB_BIT          bit(4) // bitmask 00010000
#define EXEC_SAFETY_DOOR    bit(5) // bitmask 00100000
#define EXEC_MOTION_CANCEL  bit(6) // bitmask 01000000

public:
    sys_public();
    unsigned char Is_true(unsigned char x);
    void UI_Display_HEX_Data(QString str, unsigned char *pData, int DataSize);       //UI显示HEX数据
};

#endif // SYS_PUBLIC_H
