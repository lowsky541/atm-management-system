#include "helper.h"

bool terminal_echo(bool enable, struct termios *context)
{
    if (enable)
    {
        if (tcsetattr(fileno(stdin), TCSANOW, context) != 0)
            return false;
    }
    else
    {
        struct termios flags;
        tcgetattr(fileno(stdin), &flags);
        *context = flags;

        flags.c_lflag &= ~ECHO;
        flags.c_lflag |= ECHONL;

        if (tcsetattr(fileno(stdin), TCSANOW, &flags) != 0)
            return false;
    }

    return true;
}

char immediate_getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}
