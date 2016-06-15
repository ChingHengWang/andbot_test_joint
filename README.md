# andbot_test_joint

## joint frame 
![](https://github.com/ChingHengWang/andbot_test_joint/blob/master/ref/joint_frame.jpeg)

## Step
* cd ~/catkin_ws/src/metal1
* git pull
* rospack profile
* roslaunch andbot_arm.launch (ther arm arm.launch file in this two launches)
* rostopic list
  * check topic alive

* rosrun andbot_test_joint teleop_home 0 0.1
  * left arm 0 Axis & 1 Axis (speed = 0.1rad/s)
  * Press '↑' and '↓'to control the 0 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Press '←' and '→'to control the 1 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'p' to pause the mvoement
  
* rosrun andbot_test_joint teleop_home 1 0.1
  * left arm 2 Axis & 3 Axis (speed = 0.1rad/s)
  * Press '↑' and '↓'to control the 3 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Press '←' and '→'to control the 2 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'p' to pause the mvoement
  
* rosrun andbot_test_joint teleop_home 2 0.1
  * right arm 0 Axis & 1 Axis (speed = 0.1rad/s)
  * Press '↑' and '↓'to control the 0 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Press '←' and '→'to control the 1 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'p' to pause the mvoement
  
* rosrun andbot_test_joint teleop_home 3 0.1
  * right arm 2 Axis & 3 Axis (speed = 0.1rad/s)
  * Press '↑' and '↓'to control the 3 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Press '←' and '→'to control the 2 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'p' to pause the mvoement

* Finally press 'Ctrl+C' to kill this node
