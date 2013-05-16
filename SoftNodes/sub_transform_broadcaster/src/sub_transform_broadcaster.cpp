#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <std_msgs/Float32.h>
#include <custom_msg/IMUData.h>
#include <custom_msg/MotorConfig.h>


/* RK4 integration */
struct State
{
     double x;          // position
     double v;          // velocity
     double a;					// acceleration
};

struct Derivative
{
     double dx;          // derivative of position: velocity
     double dv;          // derivative of velocity: acceleration
};

Derivative evaluate(const State &initial, double dt, const Derivative &d)
{
     State state;
     state.x = initial.x + d.dx*dt;
     state.v = initial.v + d.dv*dt;

     Derivative output;
     output.dx = state.v;
     output.dv = state.a;
     return output;
}

void integrate(State &state, double dt)
{
     Derivative a = evaluate(state, 0.0f, Derivative());
     Derivative b = evaluate(state, dt*0.5f, a);
     Derivative c = evaluate(state, dt*0.5f, b);
     Derivative d = evaluate(state, dt, c);

     const double dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
     const double dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

     state.x = state.x + dxdt * dt;
     state.v = state.v + dvdt * dt;
}
/* End of RK4 */

static std_msgs::Float32 z;
static float fr_rot, fl_rot, br_rot, bl_rot = 0;

void depthCallBack(const std_msgs::Float32& depth) {
	z.data = -(depth.data-10.0);
}

void motorConfigCallBack(const custom_msg::MotorConfig& config)
{
	fr_rot = config.front_right;
	fl_rot = config.front_left;
	br_rot = config.back_right;
	bl_rot = config.back_left;
	return;
}

void imuCallBack(const custom_msg::IMUData& data) {
	static ros::Time last_time =  ros::Time::now();
	ros::Time current_time =  ros::Time::now();
	double dt = (current_time - last_time).toSec();
	last_time =  ros::Time::now();
	
	static State cur_state_x;
	static State cur_state_y;
  
  static tf::TransformBroadcaster br;
	static float imu_x, imu_y = 0;
	static float fr_roll, fl_roll, br_roll, bl_roll = 0;

  tf::Transform imu_tr;
  tf::Transform svp_tr;
  tf::Transform sonar_tr;
  tf::Transform front_right_tr; //Front Right Motor
  tf::Transform back_right_tr; //Back Right Motor
  tf::Transform front_left_tr; //Front Left Motor
  tf::Transform back_left_tr; //Back Left Motor
  
  /* Configure motor rotations */
	fl_roll+=(fl_rot/200); //The 200 here is just for animation
	fr_roll+=(fr_rot/200);
	br_roll+=(br_rot/200);
	bl_roll+=(bl_rot/200);
  
  /* Sonar transformation */
  sonar_tr.setOrigin(tf::Vector3(-0.12, -0.32, -0.30));
  sonar_tr.setRotation( tf::createQuaternionFromRPY(0,0,M_PI) );
  
  /* SVP transformation */
  float svp_distance_from_imu = -0.07;
  svp_tr.setOrigin(tf::Vector3(0.10, -0.15, svp_distance_from_imu));
  svp_tr.setRotation( tf::createQuaternionFromRPY(0,0,0) );

  /* IMU transformation */
	cur_state_x.v = data.vel_y;
	cur_state_y.v = data.vel_x;
	cur_state_x.a = data.acc_y;
	cur_state_y.a = data.acc_x;
	
	integrate(cur_state_x, dt);
	integrate(cur_state_y, dt);
	
	imu_x = cur_state_x.x;
	imu_y = cur_state_y.x;
	
	imu_tr.setOrigin( tf::Vector3(imu_x, imu_y, z.data-svp_distance_from_imu) );
	imu_tr.setRotation( tf::createQuaternionFromRPY(data.pitch*(M_PI/180), data.roll*(M_PI/180), -data.yaw*(M_PI/180)) );
	
	/* Motors */
	front_right_tr.setOrigin( tf::Vector3(0.20, 0.15, -0.10) );
	front_right_tr.setRotation(  tf::createQuaternionFromRPY(0, fr_roll, M_PI/6) );
	front_left_tr.setOrigin( tf::Vector3(-0.20, 0.15, -0.10) );
	front_left_tr.setRotation(  tf::createQuaternionFromRPY(0, fl_roll, -M_PI/6) );
	back_right_tr.setOrigin( tf::Vector3(0.20, -0.40, -0.10) );
	back_right_tr.setRotation(  tf::createQuaternionFromRPY(0, br_roll, 5*M_PI/6) );
	back_left_tr.setOrigin( tf::Vector3(-0.20, -0.40, -0.10) );
	back_left_tr.setRotation(  tf::createQuaternionFromRPY(0, bl_roll, 7*M_PI/6) );
	
	/* Publish our transformations */
	//IMU relative to the world.
	br.sendTransform(tf::StampedTransform(imu_tr, ros::Time::now(), "/world", "/imu"));
	//SVP relative to IMU
	br.sendTransform(tf::StampedTransform(svp_tr, ros::Time::now(), "/imu", "/svp"));
	//SVP relative to the IMU
	br.sendTransform(tf::StampedTransform(sonar_tr, ros::Time::now(), "/imu", "/sonar"));
	//Thrusters relative to IMU
	br.sendTransform(tf::StampedTransform(front_right_tr, ros::Time::now(), "/imu", "/front_right_thruster"));
	br.sendTransform(tf::StampedTransform(front_left_tr, ros::Time::now(), "/imu", "/front_left_thruster"));
	br.sendTransform(tf::StampedTransform(back_right_tr, ros::Time::now(), "/imu", "/back_right_thruster"));
	br.sendTransform(tf::StampedTransform(back_left_tr, ros::Time::now(), "/imu", "/back_left_thruster"));
}

int main(int argc, char** argv){
  ros::init(argc, argv, "sub_transform_broadcaster");
  
  ros::NodeHandle n;
  
	ros::Subscriber imuSub = n.subscribe("imu", 100, imuCallBack);
	ros::Subscriber depthSub = n.subscribe("depth", 100, depthCallBack);
	ros::Subscriber motorSub = n.subscribe("motor_config", 100, motorConfigCallBack);
  
  ros::spin();
  
  /*while (ros::ok) {  
		ros::spinOnce(); //Call all waiting callbacks at this point
		r.sleep(); //Sleep for a little while
  }*/
  
  return 0;
};
