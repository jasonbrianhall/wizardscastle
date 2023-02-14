import sys
import os
import termios, fcntl
import select


# Building a better input then input(); datatype can be string on integer
def input_tab_as_enter(prompt='', max_length=10000, is_integer=False, positive_only=False):
    fd = sys.stdin.fileno()
    newattr = termios.tcgetattr(fd)
    newattr[3] = newattr[3] & ~termios.ICANON
    newattr[3] = newattr[3] & ~termios.ECHO
    termios.tcsetattr(fd, termios.TCSANOW, newattr)

    oldterm = termios.tcgetattr(fd)
    oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)
    sys.stdout.write(prompt)
    sys.stdout.flush()
    user_input = ''
    while True:
        r, w, e = select.select([sys.stdin], [], [], 0)
        if r:
            char = sys.stdin.read(1)
            if char == '\t':
                sys.stdout.write('\n')
                sys.stdout.flush()
                break
            elif char == '\n':
                sys.stdout.write('\n')
                sys.stdout.flush()
                break
            elif char == '\x7f': # handle backspaces
                if user_input:
                    user_input = user_input[:-1]
                    sys.stdout.write('\b \b') # move back and erase character
                    sys.stdout.flush()
            elif len(user_input) == max_length:
                break
            elif is_integer and not (char.isdigit() or (char == '-' and len(user_input) == 0 and positive_only==False)):
                continue
            else:
                user_input += char
                sys.stdout.write(char)
                sys.stdout.flush()
            if len(user_input)>=max_length:
                break
    termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)
    return str(user_input) if is_integer else user_input


