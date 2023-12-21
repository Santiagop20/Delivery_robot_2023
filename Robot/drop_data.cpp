#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <fstream>
#include <string>
// Linux headers
#include <cstring>
#include <eigen3/Eigen/Dense>
#include <sstream>
#include "main.h"
#include "destination.h"
#include "drop_data.h"






using namespace std;


void retrieveDropPoint(string line)
{
	if(line.compare("hemp_AB") == 0)
	{
		destination = 1;
	}
	else if(line.compare("police_HQ") == 0)
	{
		destination = 2;
	}
	else if(line.compare("zeus_AB") == 0)
	{
		destination = 3;
	}
	else if(line.compare("warehouse") == 0 )
	{
		if(destination != 10)
		{
			destination = 0;
		}		
	}
	else if(line.compare("start") == 0)
	{
		destination = 5;
	}
	else if(line.compare("location set") == 0)
	{
		destination = 7;
	}

	else
	{
		destination = 11;
	}
}


void * file_read_drop(void *)
{	
	while(1)
	{
		string line;
		char toSend;
		ifstream file("/home/autobot/tollgate_3/drop_data.txt");
	   if (file.is_open())
	   {
	     	while(getline(file, line)) 
	     	{
	     	   retrieveDropPoint(line);	     	  
	        }			
	   }		
	}	
}