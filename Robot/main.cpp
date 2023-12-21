#include <iostream>

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
#include "drop_data.h"


// compile
// g++ main.cpp destination.cpp gps_data.cpp -lpthread




using namespace std;
using namespace Eigen;

int serial_port;
char uni_mode;
int no_obstacle = 1;
int traffic_stop = 0;
int traffic_go = 0;
pthread_mutex_t lock;



void initalize_serial_port()
{ 
  
 serial_port = open("/dev/ttyUSB0", O_RDWR);
 usleep(2000000);  
  // Create new termios struct, we call it 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      cout << "Error from tcgetattr: " << errno << endl;
      
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
  tty.c_cc[VMIN] = 0;

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B115200);
  cfsetospeed(&tty, B115200);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      cout << "Error from tcsetattr: " << errno << endl;
      
  }
}

void check_for_point(Vector2d point)
{
	int range_p = 50;
	
	if((r_pos_gps(0) < point(0) + range_p) && (r_pos_gps(0) > point(0) - range_p)
				&& (r_pos_gps(1) < point(1) + range_p) && (r_pos_gps(1) > point(1) - range_p))
	{
		cout <<"Inside junction\n";		
		//pthread_mutex_lock(&lock);
		uni_mode = 's';
		writeToSerial();
		cout << "Stop at traffic light\n";
		//pthread_mutex_unlock(&lock);	
		traffic_stop = 1;
	}

}

void retrieve_traffic()
{
	ifstream traffic_file("/home/autobot/tollgate_3/traffic_file_read.txt");
	string line;
	if(traffic_file.is_open())
	{
		while(getline(traffic_file, line))
		{
			if(line.length() > 0)
			{
				if(line.compare("100") == 0)
				{
					traffic_stop = 0;
					traffic_go = 1;
					cout << "cleared traffic \n";
					break;
				}
			}
		}
	}
	
}


void * check_traffic_light(void *)
{
	Vector2d point_1_stop(125, 511), point_2_stop(585, 378), point_3_stop(712, 756);
	int range_p = 50;
	//ifstream traffic_file("/home/autobot/tollgate_3/traffic_file_read.txt");
	//string line;
	while(1)
	{
		if(traffic_stop == 0 && traffic_go == 0)
		{
			//cout <<"checking for point\n";
			check_for_point(point_1_stop);
			check_for_point(point_2_stop);
			check_for_point(point_3_stop);			
		}
		else if(traffic_stop == 1)
		{
			
			retrieve_traffic();
				
		}
		
				
	}
}   

int main() {
  char read_buf[8] = {0};
  int num_bytes;  
  initalize_serial_port();
  
  pthread_t t1, t2, t3,  t4;

  t2 = pthread_create(&t2, NULL, file_read_gps, NULL);
  t1 = pthread_create(&t1, NULL, check_destination, NULL); 
  t3 = pthread_create(&t3, NULL, file_read_drop, NULL);
  t4 = pthread_create(&t4, NULL, check_traffic_light, NULL);
  
  uni_mode = 'd';
  //memset(&read_buf, '\0', sizeof(read_buf));
  pthread_mutex_lock(&lock);
  writeToSerial();
  pthread_mutex_unlock(&lock); 
  ofstream file("/home/autobot/tollgate_3/write_dataWarehouse.txt");
  if(file.is_open())
	{
		file << "reached warehouse" << endl;
		file.close(); // to clear the contents of the file. 
	}
  
  while(1)
  {	
	
	if(ultra_distance < 50 )
	{
		
		
		
		no_obstacle = 0;
		cout << "Found obstacle in path \n";
	}
	else if(no_obstacle == 0 && ultra_distance > 60)
	{
		no_obstacle = 1;
		cout << "obstacle cleared\n";
		cout << "distance is : " << ultra_distance <<endl;
	}
	
	
  }  
  close(serial_port);	   
  return 0; // success
}






  

