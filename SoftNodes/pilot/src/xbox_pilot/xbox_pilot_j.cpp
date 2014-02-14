
 /** *********************************************** **/
 /** Name: xbox_pilot.cpp                            **/
 /**                                                 **/
 /** Function: To recieve values from the xbox       **/
 /** 		       controller and convert it into motor  **/
 /**           values and target vector values.      **/
 /**                                                 **/
 /** Author: Unknown, edited by James Killick        **/
 /**                                                 **/
 /** Last Date Modified: 11/02/2014                  **/
 /**                                         				 **/
 /**  ********************************************** **/

#include <stdio.h>
#include "ros/ros.h"
#include <custom_msg/MotorConfig.h>
#include <sensor_msgs/Joy.h>
#include <custom_msg/TargetVector.h>
#include <std_msgs/Float32.h>
#include <custom_msg/PIDValues.h>
#include <custom_msg/IMUData.h>
#include "std_msgs/Bool.h"


void joyCallback(const sensor_msgs::Joy::ConstPtr&); //declaration of call back functions
float map(float, float, float, float, float);

ros::Publisher motorMsg;	//global declaration of the publisher "motorMsg"
custom_msg::MotorConfig motorCfg;

ros::Publisher targetMsg;
custom_msg::TargetVector TargetVector;
static std_msgs::Float32 z; 
float yaw_output = 0;       //initialise values for yaw, pitch, depth.
float pitch_output = 0;
float depth_output = 0;
float dt = 0.02;

float depth_rate = 0.05; 		//m per second
float depth_Kp = 400;	//400	//3 variables for depth PID
float depth_Ki = 0.01;      //1 
float depth_Kd = 1;         //0
float depth_input = 0;			//depth value recieved from the SVP
float depth_target_raw = 0;	
float depth_target = 0;		  //desired depth value
float depth_error = 0;			//difference between desired and actual values.
float depth_previous_error = 0;
float depth_integral = 0;
float depth_derivative = 0;

float pitch_change = 0.0;
float pitch_rate = 3;      //degrees per second
float pitch_Kp = 2;        //3 variables for pitch PID
float pitch_Ki = 1;
float pitch_Kd = 0;
float pitch_input = 0;     //values recieved from IMU
float pitch_target_raw = 0;
float pitch_target = 0;    //desired pitch value
float pitch_error = 0;     //difference between desired and actual pitch value.
float pitch_previous_error = 0;
float pitch_integral = 0;
float pitch_derivative = 0; 

float rise = 0.0;         //initialise variables for depth.
float dive = 0.0;
float depthChange = 0.0;
float x_axis = 0.0;
float y_axis = 0.0;
bool crisis = false; 

/** *********************************************** **/
/** Name: pidGuiCallBack                            **/
/**                                                 **/
/** Function: To recieve PID values from the GUI.   **/
/**                                                 **/
/** *********************************************** **/

void pidGuiCallBack(const custom_msg::PIDValues& data) 
  {
    pitch_Kp = data.pitch_Kp;  //recieves values for PID from the GUI and sets it to the values for pitch.
    pitch_Ki = data.pitch_Ki;
    pitch_Kd = data.pitch_Kd;
  
    //std::cout << "PID values updated "  << std::endl;

    pitch_error = 0;    //variables for pitch PID calculation are reset.
    pitch_previous_error = 0;
    pitch_integral = 0;
    pitch_derivative = 0;
  }

 /** *********************************************** **/
 /** Name: vectorCallBack                            **/
 /**                                                 **/
 /** Function: To recieve yaw, pitch and depth data  **/
 /** when the valut of the target vector changes.    **/
 /** *********************************************** **/

 void vectorCallBack(const custom_msg::TargetVector& data) 
  {
    if (data.set_pitch == true) 
      {
        pitch_target = data.vector_pitch;
        std::cout << "Pitch target updated "  << std::endl;
      }  //if the pitch has changed, read in the new value.
  }

  /** *********************************************** **/
  /** Name: imuCallBack                               **/
  /**                                                 **/
  /** Function: To recieve yaw and pitch data and     **/
  /** set two variables to these values.              **/
  /** *********************************************** **/

 void imuCallBack(const custom_msg::IMUData& data) 
  {
    pitch_input = data.pitch;
  }

 /** *********************************************** **/
 /** Name: depthCallBack                             **/
 /**                                                 **/
 /** Function: To recieve depth data and set the     **/
 /** value of "depth_input" to this value.           **/
 /** *********************************************** **/

void depthCallBack(const std_msgs::Float32& depth) 
  {
    depth_input = depth.data; //set the value of depth_input to the measured depth value 
  }

 /** *********************************************** **/
 /** Name: constrain                                 **/
 /**                                                 **/
 /** Function: To limit motor values to prevent      **/
 /** them from going over the maximum value.         **/
 /** *********************************************** **/

float constrain(float x, float min, float max) 
  {
	  if (x < min) return min; //limits the minimum value to -100
	  if (x > max) return max; //limits the maximum value to 100
	  return x;
  }

 /** *********************************************** **/
 /** Name: map                                       **/
 /**                                                 **/
 /** Function: Takes value X and known input range   **/
 /** and maps it to a known output range.            **/
 /** *********************************************** **/

float map(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }

 /** *********************************************** **/
 /** Name: main                                      **/
 /**                                                 **/
 /** Function: Runs the main logic for the code,     **/
 /** does the PID calculations and limits the        **/
 /** maximum depth.                                  **/
 /** *********************************************** **/

int main( int argc, char **argv )
  {
    
    ros::init(argc, argv, "xbox_pilot");
    ros::NodeHandle n;

	  motorMsg = n.advertise<custom_msg::MotorConfig>("motor_config", 100); //Publisher for the motor configuration
	  ros::Publisher emergency = n.advertise<std_msgs::Bool>("emergency", 1000);  //Sets up publisher for emergency signal.

    ros::Subscriber joy_sub = n.subscribe<sensor_msgs::Joy>("joy", 10, joyCallback); // Subscribe to joystick
    ros::Subscriber depthSub = n.subscribe("depth", 100, depthCallBack);	//subscriber for the depth.
    ros::Subscriber pidGuiSub = n.subscribe("pidGui", 100, pidGuiCallBack); //subscriber for the GUI.
    ros::Subscriber vectorSub = n.subscribe("vector", 100, vectorCallBack); //subscriber for the target vectors.
    ros::Subscriber imuSub = n.subscribe("imu", 100, imuCallBack);      //subscriber for the IMU.
    
	  ros::Rate loop_rate(50); //sets the rate to 50Hz

    float pitch_output = 0;

	while(ros::ok())
	  {	
      std_msgs::Bool crisis; //Sets the message type to Boolean
      depth_target += depthChange; //depth target vector = the input target value + change in depth
      
      if(depth_target > (depth_input + 0.5))
        {
          depth_target = (depth_input + 0.5);
        }

      if(depth_target < (depth_input - 0.5))
        {
          depth_target = (depth_input - 0.5);
        }

      if(depth_target <= -5) //checks to see if the depth target is less than 5m down.
        {
          depth_target = -5; //limits the maximum depth to 5m
        }

      if(depth_target >= 0.1) //checks to see if the submarine is trying to fly
        {
          depth_target = 0.1; // limits this hight to 5m
        }
      
      //PID calculations      
      depth_error = depth_target - depth_input;
      depth_integral = depth_integral + (depth_error*dt);
      depth_derivative = (depth_error - depth_previous_error)/dt;
      depth_previous_error = depth_error;
      depth_output = (depth_Kp*depth_error) + (depth_Ki*depth_integral) + (depth_Kd*depth_derivative);
      
      if(depth_output > 25) //prevents the pitch from going above the maximum set motor value.
        {
          depth_output = 25;
        }

      //pitch PID calculations
      pitch_target += pitch_change;

      if(pitch_target > (pitch_input + 10))
        {
          pitch_target = (pitch_input + 10);
        }

      if(pitch_target < (pitch_input - 10))
        {
          pitch_target = (pitch_input - 10);
        }
      
      if(pitch_target > 45) //limits the pitch to 45 degrees
        {
          pitch_target = 45;
        }
      
      if(pitch_target < -45) //limits the pitch to -45 degrees
        {
          pitch_target = -45;
        }
      
      std::cout << "pitch: " << pitch_target  << std::endl;
      std::cout << "depth: " << depth_target  << std::endl;
      
      //Pitch PID calculations
      pitch_error = pitch_target - pitch_input;
      pitch_integral = pitch_integral + (pitch_error*dt);          
      
      if(pitch_integral >= 300) //limits the intergral to prevent integral wind-up
        {
          pitch_integral = 300;
        }
      
      pitch_derivative = (pitch_error - pitch_previous_error)/dt;
      pitch_previous_error = pitch_error;
      pitch_output = (pitch_Kp*pitch_error) + (pitch_Ki*pitch_integral) + (pitch_Kd*pitch_derivative);

      
      if(pitch_output > 25) //prevents the pitch from going above the maximum set motor value.
        {
          pitch_output = 25;
        }

      std::cout << "pitch_output: " << pitch_output  << std::endl;
      std::cout << "depth_output: " << depth_output  << std::endl;
//////////////////////////////////////////////////////////////////////
//// FIND OUT WHY BOTH MOTORS ARENT TURNING WHEN PITCH IT CHANGED.////
//// PROBABLY SOMETHING TO DO WITH DEPTH AD PITCH CANCELING OUT.  ////
//// RUN THE CODE AND SEE WHAT DEPTH_OUTPUT IS.                   ////
//////////////////////////////////////////////////////////////////////

      motorCfg.front =(int8_t)(constrain((depth_output-pitch_output), -100, 100)); 
      motorCfg.back  =(int8_t)(constrain((depth_output+pitch_output), -100, 100));  

      motorMsg.publish(motorCfg); //publish motor values.
      emergency.publish(crisis); //publish emergency message

      ros::spinOnce();
      loop_rate.sleep(); //Sleep to make the main function run at the desired rate.	   
    }
		
    motorCfg.front_right = 0; //stops all of the motors if the program fails
    motorCfg.front_left = 0;
    motorCfg.back_right = 0;
    motorCfg.back_left = 0;
    motorCfg.front = 0;
    motorCfg.back = 0;

  return 0;
  }
 
 /** *********************************************** **/
 /** Name: joyCallback                               **/
 /**                                                 **/
 /** Function: Takes values from the xbox controller **/
 /** converts it to motor values and target vectors. **/
 /** *********************************************** **/

void joyCallback(const sensor_msgs::Joy::ConstPtr& joy) 
  {
 	  // 0 = Left X Axis 
    // 1 = Left Y Axis
	
	  x_axis = joy->axes[0];
    y_axis = joy->axes[1];

    x_axis = constrain(x_axis, -0.8, 0.8);
    y_axis = constrain(y_axis, -0.8, 0.8);

    if((joy->axes[0] == 0)&&(joy->axes[1] == 0)) //if no direction is indicated.
      {
        motorCfg.front_right = 0;   //turn off motors
        motorCfg.front_left  = 0;
        motorCfg.back_left   = 0;
        motorCfg.back_right  = 0;
      }

    if(joy->axes[0] >= 0) //if the x axis is posative (right)
      {
        motorCfg.front_right = (x_axis * 50);  //-  strafe right
        motorCfg.front_left  = (x_axis * -50);  //+  multiplied by 2x y value cos of thruster orientation not being 45 degrees
        motorCfg.back_right  = (x_axis * 50); //-
        motorCfg.back_left   = (x_axis * -50);  //+
      }

    if(joy->axes[0] < 0) //if the x axis is negative (left)
      {
        motorCfg.front_right = (x_axis * 50);  //+ strafe left
        motorCfg.front_left  = (x_axis * -50); //- orientation is reversed because joy->axes[0] is negative
        motorCfg.back_right  = (x_axis * 50);  //+
        motorCfg.back_left   = (x_axis * -50); //-
      }

    if(joy->axes[1] > 0) //if the y axis is posative (up)
      {
        motorCfg.front_right += (y_axis * 50);   //+ move forward (also adding this to the x motor values)
        motorCfg.front_left  += (y_axis * 50);  //+
        motorCfg.back_right  += (y_axis * -50); //-
        motorCfg.back_left   += (y_axis * -50); //-
      }

    if(joy->axes[1] <= 0) //if the y axis is negative (down)
      {
        motorCfg.front_right += (y_axis * 50); //- move backwards (also adding this to the x motor values)
        motorCfg.front_left  += (y_axis * 50); //- orientation is reversed because joy->axes[1] is negative
        motorCfg.back_right  += (y_axis * -50);  //+
        motorCfg.back_left   += (y_axis * -50);  //+
      }
	  
    //Yaw control, converts right joy stick x values into motor values.
	  motorCfg.front_right += (20 * joy->axes[3]);
	  motorCfg.front_left += (-20 * joy->axes[3]);
	  motorCfg.back_left += (20 * joy->axes[3]);
	  motorCfg.back_right += (-20 * joy->axes[3]);

	  motorCfg.front_right = constrain(motorCfg.front_right, -100, 100); //constrain front right motor, min = -100, max = +100
	  motorCfg.front_left = constrain(motorCfg.front_left, -100, 100);
	  motorCfg.back_right = constrain(motorCfg.back_right, -100, 100);
	  motorCfg.back_left = constrain(motorCfg.back_left, -100, 100);

	  //Depth control, reads from the right ([5]) and left ([2]) triggers and outputs target vector values.

    if((joy->axes[5] < 1) && (joy->axes[2] == 1)) //if right trigger is down and left trigger is up.
      {
        rise = (-1 * ((joy->axes[5] - 1.0)/2.0));//sets the right trigger to control rise
        depthChange = (rise / 200); //sets the rate of change
        std::cout << "Rise: "  << std::endl;
      }

    else if((joy->axes[5] == 1) && (joy->axes[2] < 1)) //if right trigger is up and left trigger is down
      { 	
	      dive = (1 * ((joy->axes[2] - 1.0)/2.0)); //sets the left trigger to control dive
        depthChange = (dive / 200); //sets the rate of change		
        std::cout << "Dive: " << std::endl;
      }

    else //if none or both of the trigers are held down do not change the depth.
      {
		    depthChange = 0;
      }

    if(joy->buttons[4] == 1) //jumps the depth target down by 0.5m
      {
        std::cout << "skip down: " << std::endl;
        depth_target = depth_target - 0.5;
      }

    if(joy->buttons[5] == 1) //raises the target depth by 0.5m
      {
        std::cout << "skip up: " << std::endl;
        depth_target = depth_target + 0.5;
      }        

    // Pitch control, right joystick y axis controls pitch
    if(joy->axes[4] > 0.8) //means the pitch is only effected when the joystick it tilted completely up.
      {
        pitch_change = (0.2 * joy->axes[4]);
      }  

    if(joy->axes[4] < -0.8) // or completely down
      {
        pitch_change = (0.2 * joy->axes[4]);
      }  
      
    if((joy->axes[4] > -0.79) && (joy->axes[4] < 0.79)) //sets the change in pitch to 0 if the joystick is in the middle area.
      {
        pitch_change = 0;
      }

    //if right joystick button is pressed level out the sub  
    if(joy->buttons[10] == 1)
      {
        std::cout << "reset pitch: " << std::endl;
        pitch_target = 0;
      }  

    if(joy->buttons[9] == 1)
      {
        std::cout << "reset depth: " << std::endl;
        depth_target = 0;
      }  
    if ((joy->buttons[6] == 1) && (joy->buttons[7] == 1))
      {
        crisis = true;
        std::cout << "abort ROV " << std::endl;
      }
  }
