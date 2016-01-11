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

void v_scalar_multip(double s,double a[],double c[],int n)
{
  for (int i=0;i<n;i++)
  {
    c[i]=s*a[i];
  }

  return ;
}
double v_dot(double a[],double b[],int n)
{
  double sum=0;
  for(int i=0;i<n;i++)
  {
    sum=sum+a[i]*b[i];

   //ROS_INFO("a[i]= %f\n",a[i]);
   //ROS_INFO("b[i]= %f\n",b[i]);

  }
  return sum;
}
 
void v_sub(double a[],double b[],double c[],int n)
{
  for(int i=0;i<n;i++)
  {
    c[i]=a[i]-b[i];
  }
  return;
}
void v_add(double a[],double b[],double c[],int n)
{
  for(int i=0;i<n;i++)
  {
    c[i]=a[i]+b[i];
  }
  return;
}
void v_cross(double a[],double b[],double c[])
{
 
  c[0]=a[1]*b[2]-a[2]*b[1];
  c[1]=a[2]*b[0]-a[0]*b[2];
  c[2]=a[0]*b[1]-a[1]*b[0];

  return;
}
void v_print(double a[],int n)
{
  for(int i=0;i<n;i++)
  {
    ROS_INFO("p = %f\n",a[i]);
  }
  return;
}

class MoveR
{
public:
  point Target_left;
  arm_angle left_arm_angle;
  double lup,ldn;
  void goalCallback(const geometry_msgs::Vector3& msg);
  MoveR();
  void IK_loop();
  void IK(const point& p);
  void IK4(const point& P);

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
    left_arm_angle.th_0.y=10;
    left_arm_angle.th_1.x=th1;
    left_arm_angle.th_1.y=10;
    left_arm_angle.th_2.x=0;
    left_arm_angle.th_2.y=10;
    left_arm_angle.th_3.x=0;
    left_arm_angle.th_3.y=10;



  }
}

void MoveR::IK4(const point& P)
{
  //ROS_INFO("IK\n");
  double th0=0,th1=0,th2=0,th3=0;
  double x=P.x.data;
  double y=P.y.data;
  double z=P.z.data;
  double p[3]={x,y,z};
  double temp1=pow(x,2)+pow(y,2)+pow(z,2);
  double norm_p=pow(temp1,0.5);

  th3=PI-acos((pow(lup,2)+pow(ldn,2)-pow(norm_p,2))/(2*lup*ldn));
  double a=sin(th3)*asin(ldn/norm_p);
  double s[3]={0,0,0};
  double u_z[3]={0,0,1};
  double p_s[3];
  v_sub(p,s,p_s,3);
  //ROS_INFO("p\n");
  //v_print(p,3);	
  //ROS_INFO("s\n");
  //v_print(s,3);	
  //ROS_INFO("p-s\n");
  //v_print(p_s,3);	

  double norm_p_s=pow(pow(p_s[0],2)+pow(p_s[1],2)+pow(p_s[2],2),0.5);
  double u_n[3]={p_s[0]/norm_p_s,p_s[1]/norm_p_s,p_s[2]/norm_p_s};
  
  //ROS_INFO("u_n\n");
  //v_print(u_n,3);	
  double tmp[3];
  v_scalar_multip(v_dot(u_z,u_n,3),u_n,tmp,3);
  double u_u[3];
  v_add(u_z,tmp,u_u,3);

  //ROS_INFO("u_u \n");
  //v_print(u_u,3);	
  double u_v[3];
  v_cross(u_n,u_u,u_v);
  //ROS_INFO("u_v \n");
  //v_print(u_v,3);	

  double c[3];
  v_scalar_multip(cos(a)*lup,u_n,tmp,3);
  v_add(s,tmp,c,3);
  double r=lup*sin(a);
  double fi=PI/2;
  
  //ROS_INFO("a %f\n",a);
  double tmp1[3],tmp2[3];
  v_scalar_multip(cos(fi),u_u,tmp1,3);
  v_scalar_multip(sin(fi),u_v,tmp2,3);
  v_add(tmp1,tmp2,tmp,3);
  v_scalar_multip(r,tmp,tmp,3);
  double e[3];
  v_add(c,tmp,e,3);
  //ROS_INFO("e \n");
  //v_print(e,3);	

  double ex=e[0],ey=e[1],ez=e[2];
  //ROS_INFO("ex %f\n",ex);
  //ROS_INFO("ey %f\n",ey);
  //ROS_INFO("ez %f\n",ez);

  if(ex>=0&&ey<0)
    th0=atan((double)abs(ex)/abs(ey));
  else if(ex>=0&&ey>=0)
      th0=PI-atan((double)abs(ex)/abs(ey));
    else if(ex<0&&ey>=0)
        th0=PI+atan((double)abs(ex)/abs(ey));
      else if(ex<0&&ey<0)
          th0=-atan((double)abs(ex)/abs(ey));
  double temp2=pow(pow(ex,2)+pow(ey,2),0.5);
  if(ez>=0)
    th1=-atan((double)abs(ez)/temp2);
  else if(ez<0)
    th1=atan((double)abs(ez)/temp2);

   //ROS_INFO("IK:th0 %f\n",th0);
   //ROS_INFO("IK:th1 %f\n",th1);
   //ROS_INFO("th3 %f\n",th3);


  th2=atan2(-x*sin(th0)*sin(th1)+y*cos(th0)*sin(th1)-z*cos(th1),x*cos(th0)+y*sin(th0));
  ROS_INFO("x %f  y %f  z %f\n",x,y,z);

  ROS_INFO("th0 %f  th1 %f  th2 %f  th3 %f\n",th0,th1,th2,th3);

  if(isnan(th0)||isnan(th1)||isnan(th2)||isnan(th3)||th0>PI||th0<-1.57||th1<-0.01||th1>1.57){
    ROS_INFO("Fail and dont move\n");
   }
  else{
    ROS_INFO("go move\n");
    left_arm_angle.th_0.x=th0;
    left_arm_angle.th_0.y=10;
    left_arm_angle.th_1.x=th1;
    left_arm_angle.th_1.y=10;
    left_arm_angle.th_2.x=th2;
    left_arm_angle.th_2.y=10;
    left_arm_angle.th_3.x=th3;
    left_arm_angle.th_3.y=10;

  }
}

  MoveR::MoveR()
{
  lup=235,ldn=250;    

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
  move_R.IK_loop();
  return(0);
}


void MoveR::IK_loop()
{


  ros::Rate r(0.5);

  while(nh_.ok())
  {
    ros::spinOnce();
    IK4(Target_left);
    axis_l0_pub.publish(left_arm_angle.th_0);
    axis_l1_pub.publish(left_arm_angle.th_1);
    axis_l2_pub.publish(left_arm_angle.th_2);
    axis_l3_pub.publish(left_arm_angle.th_3);

    r.sleep();
  }


  return;
}


