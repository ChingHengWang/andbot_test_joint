#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/UInt8.h>

#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Pose.h>
#include <stdio.h>
#include <stdlib.h>
#define PI 3.14159

unsigned int ch=0;
struct Arm_Pose{
  double x;
  double y;
  double z;
  double fi;
  double t;
};

Arm_Pose home={0,-485,0,1.57,5};
Arm_Pose p0_l={0,-350,0,1.57,5};//p1_l,p2_l,p3_l,p4_l;
Arm_Pose p0_r={0,-350,0,1.57,5};//p1_r,p2_r,p3_r,p4_r;

 void predefineCallback(const std_msgs::UInt8& msg){
   ch=msg.data;
   //ROS_INFO("got it ch =%d",ch);

}


int main(int argc, char** argv)
{

  ros::init(argc, argv, "action");
  ros::NodeHandle nh_;
  ros::Publisher action_left_pub = nh_.advertise<geometry_msgs::Pose>("/andbot/left_arm_goal", 500);
  ros::Publisher action_right_pub = nh_.advertise<geometry_msgs::Pose>("/andbot/right_arm_goal", 500);
  ros::Subscriber pose_sub = nh_.subscribe("/andbot/predefinepose", 1000, predefineCallback);

  geometry_msgs::Pose pose_left,pose_right;

  ros::Rate r(1);
  while(nh_.ok())
  {
//    ROS_INFO("ch =%d",ch);
 
    ros::spinOnce();
    switch(ch)
    {
      ROS_INFO("ch =%d",ch);
	case 0:
 	  {
            ROS_INFO("home");
            pose_left.position.x=home.x; 
            pose_left.position.y =home.y; 
            pose_left.position.z =home.z; 
            pose_left.orientation.x =home.fi; 
            pose_left.orientation.y =home.t; 

            pose_right.position.x =home.x; 
            pose_right.position.y =home.y; 
            pose_right.position.z =home.z; 
            pose_right.orientation.x =home.fi; 
            pose_right.orientation.y =home.t; 
	    break;

          }
	case 1:
 	  {
            ROS_INFO("p0");
            pose_left.position.x =p0_l.x; 
            pose_left.position.y =p0_l.y; 
            pose_left.position.z =p0_l.z; 
            pose_left.orientation.x =p0_l.fi; 
            pose_left.orientation.y =p0_l.t; 

            pose_right.position.x =p0_r.x; 
            pose_right.position.y =p0_r.y; 
            pose_right.position.z =p0_r.z; 
            pose_right.orientation.x =p0_r.fi; 
            pose_right.orientation.y =p0_r.t; 
            break;
          }
    }

    action_left_pub.publish(pose_left);
    action_right_pub.publish(pose_right);


    r.sleep();
  }


  return(0);
}
