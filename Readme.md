# andbot_test_joint
* v0.1

## joint frame 
![](https://github.com/ChingHengWang/andbot_test_joint/blob/master/joint_frame.jpeg)


* roslaunch andbot andbot.launch or andbot_ekf.launch (裡面啟動arm.launch)

* rosrun andbot_test_joint teleop_home 0 0.1 左手0軸&1軸 (speed = 0.1rad/s)
  * 前後鍵控制第0軸 前後搖擺 移動至機械原點 
  * 左右鍵控制第1軸 左右搖擺 移動至機械原點 
  
* rosrun andbot_test_joint teleop_home 1 0.1 左手2軸&3軸 (speed = 0.1rad/s)
  * 前後鍵控制第3軸 前後搖擺 移動至機械原點 
  * 左右鍵控制第2軸 左右搖擺 移動至機械原點 
  
* rosrun andbot_test_joint teleop_home 2 0.1 右手0軸&1軸 (speed = 0.1rad/s)
  * 前後鍵控制第0軸 前後搖擺 移動至機械原點 
  * 左右鍵控制第1軸 左右搖擺 移動至機械原點 
  
* rosrun andbot_test_joint teleop_home 3 0.1 右手2軸&3軸 (speed = 0.1rad/s)
  * 前後鍵控制第3軸 前後搖擺 移動至機械原點 
  * 左右鍵控制第2軸 左右搖擺 移動至機械原點

* 結束後 Ctrl+C kill這個node
