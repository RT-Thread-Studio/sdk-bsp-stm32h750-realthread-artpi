#include <stdint.h>
#include <stdio.h>

int16_t char_to_int(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';

    if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;

    if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;

    return -1;
}

char int_to_char(uint8_t i)
{
    if (i >= 0 && i <= 9)
        return '0' + i;

    if (i >= 10 && i <= 15)
        return 'A' + i - 10;

    return 0;
}

int16_t uint8_to_hex(uint8_t i, char *s)
{
    s[0] = int_to_char(i / 16);
    s[1] = int_to_char(i % 16);

    if (s[0] == 0 || s[1] == 0)
        return -1;
    return 0;
}

int16_t hex_to_uint8(char *s)
{
    int16_t a = char_to_int(s[0]);
    if (a == -1)
        return -1;
    int16_t b = char_to_int(s[1]);
    if (b == -1)
        return -1;

    return a * 16 + b;
}