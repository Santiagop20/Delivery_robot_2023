#!/usr/bin/env python3
import sys
import os
import subprocess
import signal

import threading

log_file_gps = '/home/bumblebee/ros2_ws/src/py_warehouse_pubsub/py_warehouse_pubsub/GPS_log.txt'  # gps_listener
log_file_warehouse_tx = '/home/bumblebee/ros2_ws/src/py_warehouse_pubsub/py_warehouse_pubsub/warehouse_pub_log.txt'  # warehouse message_publisher
log_file_warehouse_rx = '/home/bumblebee/ros2_ws/src/py_warehouse_pubsub/py_warehouse_pubsub/warehouse_sub_log.txt'  # warehouse message_listener


# proc = subprocess.Popen("source start_ros.sh", shell=True, executable="/bin/bash")

def cmd_handler_1():
    global proc1
    proc1 = subprocess.Popen("source start_ros_gps.sh", shell=True, stdout=subprocess.PIPE, executable="/bin/bash", preexec_fn=os.setsid)

def cmd_handler_2():
    global proc2
    proc2 = subprocess.Popen("source start_ros_talker_warehouse.sh", shell=True, stdout=subprocess.PIPE, executable="/bin/bash", preexec_fn=os.setsid)

def cmd_handler_3():
    global proc3
    proc3 = subprocess.Popen("source start_ros_listener_warehouse.sh", shell=True, stdout=subprocess.PIPE, executable="/bin/bash", preexec_fn=os.setsid)


def write_to_file(filename, thing_2_write = None):
    
    with open(filename, "w") as f:
        if thing_2_write:
            thing_2_write = str(thing_2_write)
            f.write(thing_2_write)
        else:
            pass
        f.close()



def main():
    try:
        sys.stdout.write("Hello\n")
        print('-----------------------------------------------------------------------\n')
        print('Welcome to Warehouse ROS INITIALIZER\n')
        print('-----------------------------------------------------------------------\n\n')


        

        write_to_file(log_file_warehouse_rx)
        write_to_file(log_file_warehouse_tx)
        write_to_file(log_file_gps)
        


        threading.Thread(target=cmd_handler_1, args=(), daemon=True).start()
        threading.Thread(target=cmd_handler_2, args=(), daemon=True).start()
        threading.Thread(target=cmd_handler_3, args=(), daemon=True).start()
        
        #cmd_handler()
        #input("Press Enter to cancel...\n")

        print('Processes launched, wait a few seconds until they start working...\n')
        print('Type cntrol + c to cancel\n')

        while True:
            pass


    except KeyboardInterrupt:
        try:
            os.killpg(os.getpgid(proc1.pid), signal.SIGTERM) 
            print('GPS closed\n')
        except:
            pass
        try:
            os.killpg(os.getpgid(proc2.pid), signal.SIGTERM)
            print('warehouse talker closed\n')
        except:
            pass
        try:
            os.killpg(os.getpgid(proc3.pid), signal.SIGTERM)
            print('warehouse listener closed\n')
        except:
            pass
        print('closing program\n')
        

if __name__ == '__main__':
    main()
