#if !defined(__HELPER_H__)
#define __HELPER_H__

#include <stdbool.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

bool terminal_echo(bool enable, struct termios* context);
char immediate_getch();

#endif // __HELPER_H__
