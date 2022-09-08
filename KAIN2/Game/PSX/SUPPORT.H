#ifndef SUPPORT_H
#define SUPPORT_H


//extern int printf(char *fmt); // 0x80074170

//extern int sprintf(char *string, char *fmt); // 0x800741C0

//extern int vsprintf(char *str, char *fmtstr, void *argptr); // 0x800741F0

extern char * my_itoa(unsigned long value, char *str, int radix); // 0x8007446C

//extern int atoi(char *str); // 0x8007453C

extern int mytolower(int c); // 0x800745AC

//extern int strcmpi(char *s1, char *s2); // 0x800745C8


#endif
