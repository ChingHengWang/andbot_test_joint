#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Vector3.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <sensor_msgs/Joy.h>
#include <stdlib.h>
struct point{
  std_msgs::Float64 x;
  std_msgs::Float64 y;
  std_msgs::Float64 z;
};

struct arm_angle{
  std_msgs::Float64 th_0;
  std_msgs::Float64 th_1;
  std_msgs::Float64 th_2;
  std_msgs::Float64 th_3;
};
 
class MoveR
{
public:
  ros::Time current_time, last_time;
  point Target_left;
  arm_angle left_arm_angle;
  double length_link_up,length_link_down;
  void goalCallback(const geometry_msgs::Vector3& msg);
  MoveR();
  void IK_loop();
  void IK(const point& p);

  ros::NodeHandle nh_;
  ros::Publisher axis_l0_pub,axis_l1_pub,axis_l2_pub,axis_l3_pub;
protected:
  ros::Subscriber goal_sub; 
};

 void MoveR::goalCallback(const geometry_msgs::Vector3& vector){
  Target_left.x.data=1.0;
  Target_left.y.data=vector.y;
  Target_left.z.data=vector.z;
  return ; 
}
void MoveR::IK(const point& p)
{
  left_arm_angle.th_0.data=p.x.data;
  left_arm_angle.th_1.data=p.y.data;
  left_arm_angle.th_2.data=p.z.data;
  left_arm_angle.th_3.data=0;
}
  MoveR::MoveR()
{
  current_time = ros::Time::now();
  last_time = ros::Time::now();
  length_link_up=220,length_link_down=270;    

  left_arm_angle.th_0.data=0.0;
  left_arm_angle.th_1.data=0.0;
  left_arm_angle.th_2.data=0.0;
  left_arm_angle.th_3.data=0.0;
  Target_left.x.data=0.0,Target_left.y.data=0.0,Target_left.z.data=0.0;

  axis_l0_pub = nh_.advertise<std_msgs::Float64>("andbot/joint/L0/cmd/position", 1);
  axis_l1_pub = nh_.advertise<std_msgs::Float64>("andbot/joint/L1/cmd/position", 1);
  axis_l2_pub = nh_.advertise<std_msgs::Float64>("andbot/joint/L2/cmd/position", 1);
  axis_l3_pub = nh_.advertise<std_msgs::Float64>("andbot/joint/L3/cmd/position", 1);
  goal_sub = nh_.subscribe("andbot/left_arm/goal", 1000, &MoveR::goalCallback,this);


}


int main(int argc, char** argv)
{

  ros::init(argc, argv, "moveR_left_arm");
  MoveR move_R;
//  move_R.IK_loop();
 printf("hello"); 
  ros::Rate r(50.0);

  while(move_R.nh_.ok())
  {
    ROS_INFO("x %f",move_R.Target_left.x.data);
    ros::spinOnce;
    move_R.IK(move_R.Target_left);
    //current_time = ros::Time::now();
    //th_l0.data=current_time.toSec();
    move_R.axis_l0_pub.publish(move_R.Target_left.x);
    move_R.axis_l1_pub.publish(move_R.left_arm_angle.th_1);
    move_R.axis_l2_pub.publish(move_R.left_arm_angle.th_2);
    move_R.axis_l3_pub.publish(move_R.left_arm_angle.th_3);

    //last_time = ros::Time::now(); 
    r.sleep();
  }
  return(0);
}


void MoveR::IK_loop()
{


  ros::Rate r(50.0);

  while(nh_.ok())
  {
    ros::spinOnce; 
    IK(Target_left);
    //current_time = ros::Time::now();
    //th_l0.data=current_time.toSec();
    axis_l0_pub.publish(Target_left.x);
    axis_l1_pub.publish(left_arm_angle.th_1);
    axis_l2_pub.publish(left_arm_angle.th_2);
    axis_l3_pub.publish(left_arm_angle.th_3);

    //last_time = ros::Time::now(); 
    r.sleep();
  }


  return;
}


