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
#include "camera.h"

int destination = 7;
int destination_reached;

static int v_count = 0;


using namespace std;
using namespace Eigen;


Vector2d  in_warehouse(95, 980), point_3(583, 755), point_2(590, 505), point_1(350, 510),
 point_4(340, 740), dest_3(583, 890), dest_1(355, 270), dest_2(800, 500), warehouse(335, 1030);
 
 


void writeToSerial()
{
	
	
	if(write(serial_port, &uni_mode, sizeof(uni_mode)) < 0)
	{
		cout << "Error in serial \n";
	}
    usleep(20000);
}


void action_at_location(Vector2d pos, char act)
{
	int w = 0;
	int range_p = 50;
	while(w == 0)
	{
		if(no_obstacle == 1 && traffic_stop == 0)
		{
			if((w == 0) &&(r_pos_gps(0) < pos(0) + range_p) && (r_pos_gps(0) > pos(0) - range_p)
			&& (r_pos_gps(1) < pos(1) + range_p) && (r_pos_gps(1) > pos(1) - range_p))
			{
				cout << "inside point ---------------------------------------------> \n";
				pthread_mutex_lock(&lock);
				uni_mode = act; // to turn right
				//sleep(0.5);
				writeToSerial();
				cout << "Wrote in point :" <<uni_mode<< "\n";
				//writeToSerial();
				if(uni_mode != 's')
				{
					//cout << "Writing drive mode :" <<uni_mode<< "\n";
					uni_mode = 'd';
					writeToSerial();	
					cout << "Wrote after action :" <<uni_mode<< "\n";
				}
				pthread_mutex_unlock(&lock);
				
				//sleep(1);
				w = 1;
			}
			else if(destination != 0)
			{
				pthread_mutex_lock(&lock);
				uni_mode = 'd';
				writeToSerial();	
				//cout << "Wrote after action :" <<uni_mode<< "\n";
				pthread_mutex_unlock(&lock);
			}
			
		}
		else
		{
			pthread_mutex_lock(&lock);
			uni_mode = 's';
			writeToSerial();	
			//cout << "Wrote after action :" <<uni_mode<< "\n";
			pthread_mutex_unlock(&lock);
		}
		
	}
}






void stay_in_warehouse()
{
	int w = 0, range_p = 50;
	while(w == 0)
	{
		if((r_pos_gps(0) < in_warehouse(0) + range_p) && (r_pos_gps(0) > in_warehouse(0) - range_p)
			&& (r_pos_gps(1) < in_warehouse(1) + range_p) && (r_pos_gps(1) > in_warehouse(1) - range_p))
		{
			cout << "inside point ---------------------------------------------> \n";
			pthread_mutex_lock(&lock);
			uni_mode = 's'; // to turn right
			//sleep(0.5);
			writeToSerial();
			cout << "Wrote in point :" <<uni_mode<< "\n";		
			pthread_mutex_unlock(&lock);
			w = 1;
			//sleep(1);
		}
		else
		{
			pthread_mutex_lock(&lock);
			uni_mode = 'd'; 
			//sleep(0.5);
			writeToSerial();
			cout << "Wrote in point :" <<uni_mode<< "\n";		
			pthread_mutex_unlock(&lock);
		}		
	}			//To call camera 
}
void write_message_warehouse(string message)
{
	ofstream file("/home/autobot/tollgate_3/write_dataWarehouse.txt");
	if(file.is_open())
	{
		file << message;
		file.close();
	}
	
}


void * check_destination(void *)
{
	int picked = 0;
    while(1)
    {
		//writeToSerial();
	if(destination == 0)
        {	
			//action_at_location(in_warehouse, 's');
			stay_in_warehouse();
			pthread_mutex_lock(&lock);
			uni_mode = 'i'; // to drop the claws
			writeToSerial();
			cout <<"dropping claws\n";
			camera_detect();
			cout <<"camera function over \n";
			if(collected == 1)
			{
				uni_mode = 'p'; // to pick the package
				writeToSerial();
				cout << "Picking package\n";
				write_message_warehouse("picked up object");
				pthread_mutex_unlock(&lock); 
			}
			destination = 10; //waiting for new destination
			
            
        }
        if(destination == 1)
        {
			
			action_at_location(point_1, 'r');			
			action_at_location(dest_1, 'v');
			traffic_go = 0;
			action_at_location(warehouse, 'r');
			destination = 5; //waiting for gates to open
            
        }
		else if(destination == 2)
		{
			
			action_at_location(dest_2, 'v');
			traffic_go = 0;
			action_at_location(point_3, 'r');
			action_at_location(warehouse, 'r');
			destination = 5; 
			
		}
		else if(destination == 3)
		{
			
			action_at_location(point_2, 'l');
			action_at_location(dest_3, 'v');
			traffic_go = 0;
			action_at_location(warehouse, 'r');
			destination = 5;
		}
		else if(destination == 4)
		{
			action_at_location(point_1, 'r');	
			action_at_location(dest_1, 'v');
			action_at_location(point_2, 'r');
			action_at_location(dest_2, 'v');
			action_at_location(point_3, 'r');
			action_at_location(dest_3, 'v');
			action_at_location(warehouse, 'r');
			destination = 5;
			
			
		}
		else if(destination == 5)
		{
			picked = 0;
			while(destination != 0)
			{
				pthread_mutex_lock(&lock);
				uni_mode = 's';
				writeToSerial();	
				pthread_mutex_unlock(&lock);
				write_message_warehouse("reached warehouse");
			}
		}
		else if(destination == 10)
		{			
			//write_message_warehouse("picked up object");			
		}
		else if(destination == 7)
		{
			write_message_warehouse("received");
		}
		else if(destination == 11)
		{
			pthread_mutex_lock(&lock);
			writeToSerial();
			pthread_mutex_unlock(&lock);
		}
		
    }    
}



