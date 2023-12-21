#include <iostream>
#include <wiringPi.h>
#include "libSonar.h"
#include <unistd.h>
using namespace std;

int trigger = 14; //pin no 23
int echo = 26; // pin no 32
float ultra_distance;

void  sonar_distance(void )
{
    if (wiringPiSetup() == -1)
        cout << "Error in wiriningPi setup\n";

    Sonar sonar;
    sonar.init(trigger, echo);

    
        //cout << "Distance is " << sonar.distance(30000) << " cm." << endl;
	ultra_distance = sonar.distance(30000);
	sleep(0.3);
    
}
