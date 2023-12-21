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
#include "libSonar.h"

using namespace std;
using namespace Eigen;

Vector2d r_pos_gps;
int gps_obstacle_detected;

void check_distances(VectorXd dis)
{
	int count = 10; 
	for(int i = 0; i < dis.rows(); i ++)
	{
		if(dis(i) < 250 && i != rob_spiral_no)
		{
			//cout << "Obstacle detected" << endl;
			count--;
			
		}
	}
	if(count < 10)
	{
		gps_obstacle_detected = 1;
		sonar_distance();
	}
	else
	{
		ultra_distance = large_value;
		gps_obstacle_detected = 0;
	}
	
}


void obstacle_detection(Vector2d rob, MatrixXd pos)
{
	VectorXd distances(10);
	int i = 0;
	Vector2d diff;
	while(i < pos.rows())
	{
		if(pos(i, 0) < 0)
		{
			distances(i) = large_value;
		}
		else
		{
			Vector2d row_vect = pos.row(i);
			diff = rob - row_vect;
			distances(i) = diff.norm();
		}
		i++;	
	}
	//cout << "The distances are " << distances << endl; 
	check_distances(distances);
	
	
}




void retrieveGPS(string input )
{
	int len = input.length();	
	if(len != 0)
	{
		string input1 = input.substr(1, input.length() - 1);
		//cout << "after substr\n";
		stringstream ss(input1);
		MatrixXd posAll(10, 2);

		char semicolon = ';';
		char space = ' ';
		string token;

		int row = 0;

		while (getline(ss, token, semicolon)) {
			stringstream vector_stream(token);
			//cout << "after token\n";
			double val1, val2;

			// Read the first two values from each vector
			vector_stream >> val1 >> val2;
			//cout << "After vector stream\n";
			// Store the values in the Eigen matrix
			posAll(row, 0) = val1;
			posAll(row, 1) = val2;

			// Move to the next row
			++row;
		}
		//cout << "after while\n";
		
		r_pos_gps(0) = posAll(rob_spiral_no, 0);
		r_pos_gps(1) = posAll(rob_spiral_no, 1);
		obstacle_detection(r_pos_gps, posAll);
			
	}   
}


void * file_read_gps(void *)
{	
	while(1)
	{
		string line;
		char toSend;
		ifstream file("/home/autobot/lab_joseph/gps_data.txt");
	   if (file.is_open())
	   {
	     	while(getline(file, line)) 
	     	{
	     	   retrieveGPS(line);	     	  
	        }			
	   }		
	}	
}
