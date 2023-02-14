import sys
import os
import termios, fcntl
import select

def input_tab_as_enter(prompt=''):
    fd = sys.stdin.fileno()

    # Save the current terminal settings
    oldterm = termios.tcgetattr(fd)
    oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)

    # Set the terminal to raw mode (disable canonical mode and echoing)
    newattr = termios.tcgetattr(fd)
    newattr[3] = newattr[3] & ~termios.ICANON
    newattr[3] = newattr[3] & ~termios.ECHO
    termios.tcsetattr(fd, termios.TCSANOW, newattr)

    # Set non-blocking mode on the input file descriptor
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)

    # Write the prompt and read input
    sys.stdout.write(prompt)
    sys.stdout.flush()
    user_input = ''
    while True:
        r, _, _ = select.select([fd], [], [])
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
            elif char == '\x7f':
                # Handle backspace (delete last character from input)
                if user_input:
                    user_input = user_input[:-1]
                    sys.stdout.write('\b \b')
                    sys.stdout.flush()
            else:
                user_input += char
                sys.stdout.write(char)
                sys.stdout.flush()

    # Restore the original terminal settings and blocking mode
    termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)
    
    return user_input

