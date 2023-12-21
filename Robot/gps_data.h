#include <eigen3/Eigen/Dense>

#define rob_spiral_no 8
#define large_value 99999

extern Eigen::Vector2d r_pos_gps;
extern int gps_obstacle_detected;


void * file_read_gps(void *);
