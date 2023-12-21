
#
#


#!/usr/bin/env python3
import sys
import os
import subprocess


def cmd_handler():
    os.system("start /wait cmd /c {ifconfig}")
    #cmd = subprocess.Popen('cmd.exe')

    '''
    proc = subprocess.Popen(['echo "Geeks 4 Geeks"'], shell = True, stdout=subprocess.PIPE)


    while True:
        line = proc.stdout.readline()
        if not line:
            break
        else:
            # Do whathever you need to do with the last line written on 
            # stdout by your_process
            print("doing some stuff with...", line)
            print("done for this line!")
    '''

def main():
    sys.stdout.write("Hello")
    print("Hello2")
    cmd_handler()
    input("Press Enter to continue...")
    print('Bye')
    exit()

if __name__ == '__main__':
    main()
