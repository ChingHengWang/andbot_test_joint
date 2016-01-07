#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Vector3.h>
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <sensor_msgs/Joy.h>

struct point{
  double x;
  double y;
  double z;
};


 
class MoveR
{
public:
  ros::Time current_time, last_time;
  point Start_l,Target_l;
  std_msgs::Float64 theta_l0,theta_l1,theta_l2,theta_l3;
  void goalCallback(const geometry_msgs::Vector3& msg);
  MoveR();
  void IK_loop();

private:
  ros::NodeHandle nh_;
  ros::Publisher axis_l0_pub,axis_l1_pub,axis_l2_pub,axis_l3_pub;
protected:
  ros::Subscriber goal_sub; 
};

 void MoveR::goalCallback(const geometry_msgs::Vector3& vector){
  Target_l.x=vector.x;
  Target_l.y=vector.y;
  Target_l.z=vector.z;
  return ; 
}
  MoveR::MoveR()
{
  current_time = ros::Time::now();
  last_time = ros::Time::now();

  Target_l.x=0.0,Target_l.y=0.0,Target_l.z=0.0;
  Start_l.x=0.0,Start_l.y=0.0,Start_l.z=0.0;
  theta_l0.data=0.0,theta_l1.data=0.0,theta_l2.data=0.0,theta_l3.data=0.0;

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

  move_R.IK_loop();
  
  return(0);
}


void MoveR::IK_loop()
{

   puts("IK_loop");

  ros::Rate r(50.0);

  while(nh_.ok())
  {

    ros::spinOnce; 
    //current_time = ros::Time::now();
    //theta_l0.data=current_time.toSec();
    theta_l0.data=last_time.toSec();;
    theta_l1.data=last_time.toSec();;
    axis_l0_pub.publish(theta_l0);
    axis_l1_pub.publish(theta_l1);
    axis_l2_pub.publish(theta_l2);
    axis_l3_pub.publish(theta_l3);

    //last_time = ros::Time::now(); 
    r.sleep();
  }


  return;
}


