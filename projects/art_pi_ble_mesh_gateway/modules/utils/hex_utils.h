#ifndef HEX_UTILS_H
#define HEX_UTILS_H


int16_t char_to_int(char c);

char int_to_char(uint8_t i);

int16_t uint8_to_hex(uint8_t i, char *s);

int16_t hex_to_uint8(char *s);

#endif