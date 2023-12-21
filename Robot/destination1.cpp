#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <fstream>
#include <string>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <cstring>
#include <eigen3/Eigen/Dense>
#include <sstream>
#include "gps_data.h"
#include "main.h"
#include "destination.h"
#include "libSonar.h"

int destination;
int destination_reached;

#define warehouse_x 335
#define warehouse_y 1050

#define point1_x 350
#define point1_y 510

#define point2_x 590
#define point2_y 505

#define point3_x 583
#define point3_y 755

#define point4_x 340
#define point4_y 750


using namespace std;
using namespace Eigen;

void writeToSerial()
{
	if(write(serial_port, &uni_mode, sizeof(uni_mode)) < 0)
	{
		cout << "Error in serial \n";
	}	
}



void go_dest_1(void )
{
    int p1 = 0, p2 = 0, p3 = 0, p4 = 0, w = 0, range_p = 50;
    while(destination_reached == 0)
    {
		if(no_obstacle == 1)
		{
			pthread_mutex_lock(&lock);
			uni_mode = 'd'; 
			writeToSerial();
			pthread_mutex_unlock(&lock);
			
        //cout << "X is " << r_pos_gps(0) << endl << "Y is "<<r_pos_gps(1) << endl;
		if((w == 0) &&(r_pos_gps(0) < warehouse_x + range_p) && (r_pos_gps(0) > warehouse_x - range_p)
        && (r_pos_gps(1) < warehouse_y + range_p) && (r_pos_gps(1) > warehouse_y - range_p))
        {
			cout << "inside warehouse ---------------------------------------------> \n";
            pthread_mutex_lock(&lock);
            uni_mode = 'r'; // to turn right
			//sleep(0.5);
			writeToSerial();
			cout << "Wrote in point 1 :" <<uni_mode<< "\n";
            
            pthread_mutex_unlock(&lock);
			//sleep(1);
			p1 = 0;
			p3 = 0;
			p2 = 0;
			p4 = 0;
			w = 1;
        }
		
        if((p1 == 0) &&(r_pos_gps(0) < point1_x + range_p) && (r_pos_gps(0) > point1_x - range_p)
        && (r_pos_gps(1) < point1_y + range_p) && (r_pos_gps(1) > point1_y - range_p))
        {
			cout << "inside point 1 ---------------------------------------------> \n";
            pthread_mutex_lock(&lock);
            uni_mode = 'r'; // to turn right
			//sleep(0.5);
			writeToSerial();
			cout << "Wrote in point 1 :" <<uni_mode<< "\n";
            
            pthread_mutex_unlock(&lock);
			//sleep(1);
			p1 = 1;
			p3 = 0;
			p2 = 0;
			p4 = 0;
			w = 0;
        }
		
		if((p2 == 0)&&(r_pos_gps(0) < point2_x + 50) && (r_pos_gps(0) > point2_x - 50)
        && (r_pos_gps(1) < point2_y + 50) && (r_pos_gps(1) > point2_y - 50))
        {
			cout << "inside point 2 ---------------------------------------------> \n";
            pthread_mutex_lock(&lock);
            uni_mode = 'r'; // to turn right
            
			writeToSerial();
            cout << "Wrote in point 2 :" <<uni_mode<< "\n";
           
            pthread_mutex_unlock(&lock);
			//sleep(1);
			p1 = 0;
			p2 = 1;
			p3 = 0;
			p4 = 0;
			w = 0;
			
        }
		if((p4 == 0)&&(r_pos_gps(0) < point4_x + 50) && (r_pos_gps(0) > point4_x - 50)
        && (r_pos_gps(1) < point4_y + 50) && (r_pos_gps(1) > point4_y - 50))
        {
			cout << "inside point 4 ---------------------------------------------> \n";
            pthread_mutex_lock(&lock);
            uni_mode = 'r'; // to turn right
            
			writeToSerial();
            cout << "Wrote in point 4 :" <<uni_mode<< "\n";
            
            pthread_mutex_unlock(&lock);
			//sleep(1);
			p4 = 1;
			p2 = 0;
			p1 = 0;
			p3 = 0;
			w = 0;
        }
		if((p3 == 0)&&(r_pos_gps(0) < point3_x + 50) && (r_pos_gps(0) > point3_x - 50)
        && (r_pos_gps(1) < point3_y + 50) && (r_pos_gps(1) > point3_y - 50))
        {
			cout << "inside point 3 ---------------------------------------------> \n";
            pthread_mutex_lock(&lock);
            uni_mode = 'r'; // to turn right
            
			writeToSerial();
            cout << "Wrote in point 3 :" <<uni_mode<< "\n";
           
            pthread_mutex_unlock(&lock);
			//sleep(1);
			p3 = 1;
			p2 = 0;
			p1 = 0;
			p4 = 0;
			w = 0;
        }
    }
	}
}


void * check_destination(void *)
{
    while(1)
    {
        if(destination == 0)
        {
            go_dest_1();
        }
    }    
}



