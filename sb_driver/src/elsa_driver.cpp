/*
*  driver node to communicate and control arduino
*  sorts incoming sensor information and outputs correct ros messages 
*/

#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <ros/ros.h>
#include <std_msgs/Bool.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include "sb_msgs/RobotState.h"
#include "sb_msgs/IMU.h"
#include "SerialCommunication.h"
#include "elsa_driver.h"

#include <tf/transform_broadcaster.h>//odom stuff
#include <nav_msgs/Odometry.h>//http://wiki.ros.org/navigation/Tutorials/RobotSetup/Odom reference


using namespace std;
using namespace ros;

//global constants
static const string ROS_NODE_NAME = "driver";
static const int ROS_LOOP_RATE = 20; //hz

static const int BAUD_RATE = 115200;
//static const string PORT_NAME = "/dev/ttyUSB";
static const string UNO_PORT_NAME = "/dev/ttyACM";
static const string BLUETOOTH_PORT_NAME = "/dev/rfcomm";

static const string CAR_COMMAND_TOPIC = "lidar_nav";
static const string TURRET_COMMAND_TOPIC = "turret_command";
static const string ESTOP_TOPIC = "eStop";
static const string ROBOT_STATE_TOPIC = "robot_state";
static const string GPS_STATE_TOPIC = "gps_state";
static const string COMPASS_STATE_TOPIC = "compass_state";

static const string INIT_STRING = "BG";
static const char IDENTIFIER_BYTE = 'B';

static const int SECOND = 1000000;

//global variables
//ServoControl servo;
MechControl mech;
char twist_y[3]={'1','2','5'};
char twist_z[3]={'1','2','5'};

bool eStop = false;


int main(int argc, char** argv)
{
    //initialize ros
    init(argc, argv, ROS_NODE_NAME);
	  NodeHandle n;
	  Rate loop_rate(ROS_LOOP_RATE);

	  ros::Time current_time, last_time;
	  current_time = ros::Time::now();//odom stuff
	  last_time = ros::Time::now();
	
	  double x = 0.0;
	  double y = 0.0;
  	double th = 0.0;
  
  	double vx = 0;
  	double vy = 0;
  	double vth = 0;



	//initialize serial communication
	SerialCommunication link;
	for (int i = 0; ; i++)
	{
	    stringstream ss;
	    ss << i;	    
	    if (link.connect(BAUD_RATE,(UNO_PORT_NAME + ss.str())))
	    {
	        cout << "connected on port " << UNO_PORT_NAME << i << endl;
	        break;
		} else if (link.connect(BAUD_RATE,(BLUETOOTH_PORT_NAME + ss.str()))) {
			cout << "connected on bluetooth port " << BLUETOOTH_PORT_NAME << i << endl;
			break;
	    } else if (i > 15) {
	        cout << "unable to find a device" << endl;
	        return 0;
	    }
	}
	usleep(10*SECOND);
		
	//subscribers and publishers

	Subscriber car_command = n.subscribe(CAR_COMMAND_TOPIC, 1, car_command_callback);

//	Subscriber turret_command = n.subscribe(TURRET_COMMAND_TOPIC, 1, turret_command_callback);
	Subscriber eStop_topic = n.subscribe(ESTOP_TOPIC, 1, eStop_callback);
	
	Publisher robot_state = n.advertise<sb_msgs::RobotState>(ROBOT_STATE_TOPIC,1);
	Publisher gps_state = n.advertise<std_msgs::String>(GPS_STATE_TOPIC,1);
	

	ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);//odom stuff
	tf::TransformBroadcaster odom_broadcaster;
	
	sb_msgs::IMU imu;
	sb_msgs::RobotState state;

	std_msgs::String gps_data;
	
	ROS_INFO("arduino_driver ready");
	mech.twist_y=125;
	mech.twist_z=125;

	//while ros is good
	ros::Time begin = ros::Time::now();
	int counter = 0;
	link.clearBuffer();
	while(ok())
	{
		if (int((ros::Time::now() - begin).toSec()) >= 1) {
			cout << counter << endl;
			counter = 0;
			begin = ros::Time::now();
		}
		counter++;
	    //write to arduino	 
		stringstream ss;
	    if (eStop)
	    {	
			cout << "eStop on" << endl;
			ss << (char)IDENTIFIER_BYTE << "125125125";
	    } else {  
            //use carCommand and turretCommand
			ss << (char)IDENTIFIER_BYTE<< twist_y[0] << twist_y[1] << twist_y[2] << twist_z[0] << twist_z[1] << twist_z[2];

	    }
			cout << ss.str() << endl;
	    link.writeData(ss.str(), 7); 
	    //delay for sync
	    usleep(2000000);
	    
	    //publish data
			char test[24];
	 link.readData(24, test);
			cout << test;
	    processData(test,state);//" -19,      0,      0."
	    robot_state.publish(state);
     
	    
	    vth=th+state.compass*M_PI/180;
	    th=state.compass*M_PI/180;//check units is degres what you need?
	    vy=(state.RightVelo+state.LeftVelo)/2;

	    /*
	    //compute odometry in a typical way given the velocities of the robot
	    double dt = (current_time - last_time).toSec();
  	double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
  	double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
  	x += delta_x;
  	y += delta_y;
	    
	    current_time = ros::Time::now();//odom stuff
	    //since all odometry is 6DOF we'll need a quaternion created from yaw
	    geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);//units of compass right? (degrees)
  	
  	//first, we'll publish the transform over tf
  	geometry_msgs::TransformStamped odom_trans;
  	odom_trans.header.stamp = current_time;
  	odom_trans.header.frame_id = "odom";
  	odom_trans.child_frame_id = "base_link";
  	
  	odom_trans.transform.translation.x = x;
  	odom_trans.transform.translation.y = y;
  	odom_trans.transform.translation.z = 0.0;
  	odom_trans.transform.rotation = odom_quat;
  
  	//send the transform
  	odom_broadcaster.sendTransform(odom_trans);
  	
  	//next, we'll publish the odometry message over ROS
  	nav_msgs::Odometry odom;
  	odom.header.stamp = current_time;
  	odom.header.frame_id = "odom";
  	
  	//set the position
  	odom.pose.pose.position.x = x;
  	odom.pose.pose.position.y = y;
  	odom.pose.pose.position.z = 0.0;
  	odom.pose.pose.orientation = odom_quat;
  	//set the velocity
  	odom.child_frame_id = "base_link";
  	odom.twist.twist.linear.x = vx;
  	odom.twist.twist.linear.y = vy;
  	odom.twist.twist.angular.z = vth;
  	//publish the message
  	odom_pub.publish(odom);
	    */
	    
	    //clear buffer (MAY NOT WORK)
	    link.clearBuffer();
	    
	    //log and loop
	   // ROS_INFO("%i,%i,%i",mech.twist_x, mech.twist_y, mech.twist_z);
	    spinOnce();
		loop_rate.sleep();
	}
	
	//killing driver
	ROS_INFO("shutting down arduino_driver");
	string s = "ED" ;
    link.writeData(s, 2);
	
	return 0;
}

//dummy function
void processData(string data,sb_msgs::RobotState &state)
{
//	state.compass.push_back(data[0] << 8|data[1]);
	/*state.compass=data[0] << 8|data[1];//Replaced push_back
	long right=(data[2] << 24|data[3] << 16|data[4] << 8|data[5]);
	long left=(data[6] << 24|data[7] << 16|data[8] << 8|data[9]);*/

	cout << "data: " << data << endl;
	
	int right, left; 
	int compass_d;
	
	if (data.size() >= 5) 
	{
	 compass_d = atoi(data.substr(0,4).c_str());
		cout << "1: " << compass_d << endl;
	}
	
  if (data.size() >= 6)
  {

	  right=atoi(data.substr(5,7).c_str());
    cout << "2: " << right << endl;
  }

  if (data.size() >= 14)
  {
    cout << "3: " << data.substr(13,7) << endl;
	  left=atoi(data.substr(13,7).c_str());
  }
  
	state.RightVelo = (float) (right/1000);
	state.LeftVelo = (float) (left/1000);
	cout << compass_d << endl;
	state.compass = (int) compass_d;

//	state.RightVelo.push_back(float(right)/1000);
//	statee.LeftVelo.push_back(float(left)/1000);
}

//car_command_callback
void car_command_callback(const geometry_msgs::TwistConstPtr& msg_ptr)
{

	cout << *msg_ptr << endl;	
	cout << "y: " << msg_ptr->linear.y << "z: " << msg_ptr->angular.z << endl;
	mech.twist_y = msg_ptr->linear.y * 125+125; 
	mech.twist_z = -msg_ptr->angular.z * 125+125;


	ROS_INFO("car command has been called");
	mech.twist_y = (msg_ptr->linear.y) * 125+125; 
	mech.twist_z = -(msg_ptr->angular.z) * 125+125;
    cout<<"callback function running"<<endl;
    cout<< "linear y:"<<msg_ptr->linear.y <<endl;
    cout<< "angular z:"<< msg_ptr->angular.z <<endl;
    cout<< "mech twist_y:"<<mech.twist_y<<endl;
	cout<< "mech twist_z:"<<mech.twist_z<<endl;
	sprintf(twist_y,"%03d",mech.twist_y);
	sprintf(twist_z,"%03d",mech.twist_z);
	cout<< "twist_y:"<<twist_y<<endl;
	cout<< "twist_z:"<<twist_z<<endl;
	ROS_INFO("Twist_y: %s", twist_y);
	ROS_INFO("Twist_z: %s", twist_z);
}

//eStop_callback
void eStop_callback(const std_msgs::BoolConstPtr& msg_ptr)
{
    eStop = msg_ptr->data;
}
