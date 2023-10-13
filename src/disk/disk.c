#include "io/io.h"
int disk_read_sector(int lba, int total, void* buf)
{
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, total);
    outb(0x1F3, (unsigned char)(lba & 0xff));
    outb(0x1F4, (unsigned char)(lba >> 8));
    outb(0x1F5, (unsigned char)(lba >> 16));
    outb(0x1F7, 0x20);

    unsigned short* ptr = (unsigned short*) buf;
    for (int i = 0; i < total; i++)
    {
        // Wait to the buffer to be ready
        char c = insb(0x1F7);
        while(!(c & 0x80))
        {
            c = insb(0x1F7);
        }

        // Copy from hard disk to memory
        for (int j = 0; i < 256; i++)
        {
            *ptr = insw(0x1F0);
            ptr++;
        }
    }
    return 0;
}