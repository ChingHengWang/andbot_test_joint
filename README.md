# andbot_test_joint
* v0.1

## joint frame 
![](https://github.com/ChingHengWang/andbot_test_joint/blob/master/ref/joint_frame.jpeg)

## Steps
* roslaunch andbot andbot.launch or andbot_ekf.launch (ther arm arm.launch file in this two launches)

* rosrun andbot_test_joint teleop_home 0 0.1 left arm 0 Axis & 1 Axis (speed = 0.1rad/s)
  * Pres '↑' and '↓'to control the 0 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Pres '←' and '→'to control the 1 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'P' to pause the mvoement
  
* rosrun andbot_test_joint teleop_home 1 0.1 left arm 2 Axis & 3 Axis (speed = 0.1rad/s)
  * Pres '↑' and '↓'to control the 3 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Pres '←' and '→'to control the 2 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'P' to pause the mvoement
  
* rosrun andbot_test_joint teleop_home 2 0.1 right arm 0 Axis & 1 Axis (speed = 0.1rad/s)
   * Pres '↑' and '↓'to control the 0 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Pres '←' and '→'to control the 1 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'P' to pause the mvoement
  
* rosrun andbot_test_joint teleop_home 3 0.1 right arm 2 Axis & 3 Axis (speed = 0.1rad/s)
  * Pres '↑' and '↓'to control the 3 Axix forward and backward, and move this Axis to 'mechanical origin point'
  * Pres '←' and '→'to control the 2 Axix left and right, and move this Axis to 'mechanical origin point'
  * press 'P' to pause the mvoement

* Finally press 'Ctrl+C' to kill this node
