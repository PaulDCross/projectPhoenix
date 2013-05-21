#include <stdio.h>
#include "ros/ros.h"
#include <std_msgs/Float32.h>
#include <custom_msg/IMUData.h>
#include <custom_msg/MotorConfig.h>
#include <custom_msg/TargetVector.h>
#include <custom_msg/PIDValues.h>
#include <sensor_msgs/Joy.h>

void joyCallback(const sensor_msgs::Joy::ConstPtr&);
float constrain(float x, float min, float max);
float map(float, float, float, float, float);

ros::Publisher motorMsg;
custom_msg::MotorConfig motorCfg;

static std_msgs::Float32 z;


float yaw_Kp = 1;
float yaw_Ki = 0;
float yaw_Kd = 0;
float yaw_input = 0;
float yaw_target = 0;
/******************************************************
 * 
 *  callbacks; 
 * 
 * ***************************************************/

void depthCallBack(const std_msgs::Float32& depth) {
	z.data = -(depth.data-10.0);
}

void imuCallBack(const custom_msg::IMUData& data) {
	yaw_input = data.yaw;
}

void pidGuiCallBack(const custom_msg::PIDValues& data) {
	yaw_Kp = data.yaw_Kp;
	yaw_Ki = data.yaw_Ki;
	yaw_Kd = data.yaw_Kd;
}

void vectorCallBack(const custom_msg::TargetVector& data) {
	yaw_target = data.vector_yaw;
}
/******************************************************
 * 
 *  Main; the master of functions, the definer of variables.
 * 
 * ***************************************************/
int main( int argc, char **argv )
{
	ros::init(argc, argv, "pid_pilot");
	ros::NodeHandle n;

	/* Publisher for the motor configuration */
	motorMsg = n.advertise<custom_msg::MotorConfig>("motor_config", 100);
	ros::Subscriber imuSub = n.subscribe("imu", 100, imuCallBack);
	ros::Subscriber depthSub = n.subscribe("depth", 100, depthCallBack);
	ros::Subscriber pidguiSub = n.subscribe("pidgui", 100, pidGuiCallBack);
	ros::Subscriber vectorSub = n.subscribe("vector", 100, vectorCallBack);	
	
	ros::Rate r(50);

	float yaw_error = 0;
	float yaw_previous_error = 0;
	float yaw_integral = 0;
	float yaw_derivative = 0;
	float yaw_output = 0;
	float dt = (1/50);

	while(ros::ok())
		{	
			ros::spinOnce(); //Call all waiting callbacks at this point
			//yaw pid
			yaw_error = yaw_target - yaw_input;
  			yaw_integral = yaw_integral + yaw_error*dt;
  			yaw_derivative = (yaw_error - yaw_previous_error)/dt;
  			yaw_previous_error = yaw_error;
  			yaw_output = yaw_Kp*yaw_error + yaw_Ki*yaw_integral + yaw_Kd*yaw_derivative; //fullpid

			
			motorCfg.front_right = constrain(yaw_output, -100, 100);
			motorCfg.front_left = constrain(-yaw_output, -100, 100);
			motorCfg.back_right = constrain(yaw_output, -100, 100);
			motorCfg.back_left = constrain(-yaw_output, -100, 100);
			motorCfg.front = 0;
			motorCfg.back = 0;	
			motorMsg.publish(motorCfg);

//code goes here
			
			r.sleep(); //Sleep for a little while
		}
	

	return 0;
}

float constrain(float x, float min, float max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

float map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



/* OLD MOTOR STUFF FOR REFRENCE
void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) {

	// 0 = Left X Axis
	//	 1 = Left Y Axis
	
	
	float rads = atan2(joy->axes[1], -joy->axes[0]); //X & Y to rads
	float distance = constrain(sqrt(pow(-joy->axes[0],2)+pow(joy->axes[1],2)),0,1); //Get distance
	distance = (distance < 0.12 ? 0 : distance); //Bit of a dead zone
	distance = constrain(map(distance, 0.12, 1, 0, 1), 0, 1); //Re-map so it's smooth from deadzone outwards

	//Direction Control	
	motorCfg.front_right = -(int8_t)(constrain(140 * cos(rads+M_PI/4), -100, 100) * distance);
	motorCfg.front_left = -(int8_t)(constrain(-140 * cos(M_PI/4-rads), -100, 100) * distance);
	motorCfg.back_left = -(int8_t)(constrain(-140 * cos(rads+M_PI/4), -100, 100) * distance);
	motorCfg.back_right = -(int8_t)(constrain(140 * cos(M_PI/4-rads), -100, 100) * distance);

	//Yaw control...
	motorCfg.front_right += (100 * joy->axes[3]);
	motorCfg.front_left += (-100 * joy->axes[3]);
	motorCfg.back_left += (100 * joy->axes[3]);
	motorCfg.back_right += (-100 * joy->axes[3]);

	motorCfg.front_right = constrain(motorCfg.front_right, -100, 100);
	motorCfg.front_left = constrain(motorCfg.front_left, -100, 100);
	motorCfg.back_right = constrain(motorCfg.back_right, -100, 100);
	motorCfg.back_left = constrain(motorCfg.back_left, -100, 100);

	//Leave out depth for now
	motorCfg.front = 0;
	motorCfg.back = 0;	

	motorMsg.publish(motorCfg);

}*/
