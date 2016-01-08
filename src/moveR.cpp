#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Vector3.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <sensor_msgs/Joy.h>
#include <stdlib.h>
#define PI 3.14159
struct point{
  std_msgs::Float64 x;
  std_msgs::Float64 y;
  std_msgs::Float64 z;
};

struct arm_angle{
  geometry_msgs::Vector3 th_0;
  geometry_msgs::Vector3 th_1;
  geometry_msgs::Vector3 th_2;
  geometry_msgs::Vector3 th_3;
};
 
class MoveR
{
public:
  point Target_left;
  arm_angle left_arm_angle;
  double length_link_up,length_link_down;
  void goalCallback(const geometry_msgs::Vector3& msg);
  MoveR();
  void IK_loop();
  void IK(const point& p);

  ros::NodeHandle nh_;
  ros::Publisher axis_l0_pub,axis_l1_pub,axis_l2_pub,axis_l3_pub;
  ros::Subscriber goal_sub; 
};

 void MoveR::goalCallback(const geometry_msgs::Vector3& vector){
  //ROS_INFO("subscribe\n");
  Target_left.x.data=vector.x;
  Target_left.y.data=vector.y;
  Target_left.z.data=vector.z;
}
void MoveR::IK(const point& p)
{
  //ROS_INFO("IK\n");
  double th0=0,th1=0;
  double x=p.x.data;
  double y=p.y.data;
  double z=p.z.data;
  double temp1=pow(x,2)+pow(y,2)+pow(z,2);
  double len_E=pow(temp1,0.5);
  if(x>=0&&y<0)
    th0=atan((double)abs(x)/abs(y));
  else if(x>=0&&y>=0)
      th0=PI-atan((double)abs(x)/abs(y));
    else if(x<0&&y>=0)
        th0=PI+atan((double)abs(x)/abs(y));
      else if(x<0&&y<0)
          th0=2*PI-atan((double)abs(x)/abs(y));
  double temp2=pow(pow(x,2)+pow(y,2),0.5);
  if(z>=0)
    th1=-atan((double)abs(z)/temp2);
  else if(z<0)
    th1=atan((double)abs(z)/temp2);

   ROS_INFO("IK:th0 %f\n",th0);
   ROS_INFO("IK:th1 %f\n",th1);

  if(isnan(th0)||isnan(th1)||th0>PI||th0<-1.57||th1<0||th1>1.57){
    ROS_INFO("Fail and dont move\n");
   }
  else{
    ROS_INFO("go move\n");
    left_arm_angle.th_0.x=th0;
    left_arm_angle.th_0.y=0;
    left_arm_angle.th_1.x=th1;
    left_arm_angle.th_1.y=0;
    left_arm_angle.th_2.x=0;
    left_arm_angle.th_2.y=0;
    left_arm_angle.th_3.x=0;
    left_arm_angle.th_3.y=0;



  }
}
  MoveR::MoveR()
{
  length_link_up=220,length_link_down=270;    

  left_arm_angle.th_0.x=0.0;
  left_arm_angle.th_0.y=0.0;

  left_arm_angle.th_1.x=0.0;
  left_arm_angle.th_1.y=0.0;

  left_arm_angle.th_2.x=0.0;
  left_arm_angle.th_2.y=0.0;

  left_arm_angle.th_3.x=0.0;
  left_arm_angle.th_3.y=0.0;

  Target_left.x.data=0.0,Target_left.y.data=0.0,Target_left.z.data=0.0;

  axis_l0_pub = nh_.advertise<geometry_msgs::Vector3>("andbot/joint/L0/cmd/position", 1);
  axis_l1_pub = nh_.advertise<geometry_msgs::Vector3>("andbot/joint/L1/cmd/position", 1);
  axis_l2_pub = nh_.advertise<geometry_msgs::Vector3>("andbot/joint/L2/cmd/position", 1);
  axis_l3_pub = nh_.advertise<geometry_msgs::Vector3>("andbot/joint/L3/cmd/position", 1);
  goal_sub = nh_.subscribe("/andbot/left_arm/goal", 1000, &MoveR::goalCallback,this);


}


int main(int argc, char** argv)
{

  ros::init(argc, argv, "moveR_left_arm");
  MoveR move_R;
//  move_R.IK_loop();
  ros::Rate r(0.5);

  while(move_R.nh_.ok())
  {
/*
    ROS_INFO("x %f\n",move_R.Target_left.x.data);i
    ROS_INFO("y %f\n",move_R.Target_left.y.data);
    ROS_INFO("z %f\n",move_R.Target_left.z.data);
*/    
    ros::spinOnce();
    move_R.IK(move_R.Target_left);
    move_R.axis_l0_pub.publish(move_R.left_arm_angle.th_0);
    move_R.axis_l1_pub.publish(move_R.left_arm_angle.th_1);
    move_R.axis_l2_pub.publish(move_R.left_arm_angle.th_2);
    move_R.axis_l3_pub.publish(move_R.left_arm_angle.th_3);

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
    axis_l0_pub.publish(Target_left.x);
    axis_l1_pub.publish(left_arm_angle.th_1);
    axis_l2_pub.publish(left_arm_angle.th_2);
    axis_l3_pub.publish(left_arm_angle.th_3);

    r.sleep();
  }


  return;
}


