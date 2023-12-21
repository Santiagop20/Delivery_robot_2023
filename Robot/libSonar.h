#ifndef DEF_SONAR
#define DEF_SONAR


extern float ultra_distance;
void  sonar_distance(void );

class Sonar
{
  public:
    Sonar();
    void init(int trigger, int echo);
    double distance(int timeout);

  private:
    void recordPulseLength();
    int trigger;
    int echo;
    volatile long startTimeUsec;
    volatile long endTimeUsec;
    double distanceMeters;
    long travelTimeUsec;
    long now;
};

#endif
