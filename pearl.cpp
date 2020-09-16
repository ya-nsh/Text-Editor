#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>

//terminal Settings
struct termios orig_termios;
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);
    struct termios raw = orig_termios; //disabling ctrl-s and ctrl-q
    raw.c_iflag &= ~(IXON);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

    /*
    ICANON flag turns off canonical mode. So, input will be read byte-by-byte, instead of line-by-line.
    
    By default, Ctrl-C sends a SIGINT signal to the current process which causes terminal to terminate, and Ctrl-Z sends a SIGTSTP signal to the current process which causes it to suspend. So, turning off the sending of both of these symbols. Thus, ISIG is used.

    */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int argc, char const *argv[])
{
    enableRawMode();

    char c;

    //displaying keypresses and quitting once 'q' is encountered.
    while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q')
    {
        //iscntrl checks whether a character is a control character (non-printable characters. 0-31 and 127 ASCII Values)
        if (iscntrl(c))
        {
            printf("%d\n", c);
        }
        else
        {
            std::cout << +c << " " << (c) << std::endl;
        }
    }
    /*
    read one byte from the standard imput into the variable c until no more bytes left to read. read() returns number of bytes it read and will return 0 when it reaches the end of the file. To quit this program, you will have to type a line of text that includes a q in it, and then press enter. The program will quickly read the line of text one character at a time until it reads the q, at which point the while loop will stop and the program will exit.
    */
    return 0;
}
