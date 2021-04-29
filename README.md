# Badminton Player Tracker

**Overview**

This project is aimed toward tracking players movements on a badminton court. It uses the OpenCV framework and standard, readily available footage for doing this. It will be able to track player position on the court and calculate metrics such velocity, acceleration, heat maps. Future plans involve combining this with OpenPose to get a more refined set of metrics.

**Installation and Running Instruction**

1. Clone the repo
   1. `git clone https://gitlab.com/kyle_c/badminton_player_tracker`
   2. `cd badminton_player_tracker`
2. Create a build directory
   1. `mkdir build && cd build`
3. run cmake and then make
   1. `cmake ..`
   2. `make`
4. `./Capstone --video=../lee.mp4`

Requirements include OpenCV version 3.4 or greater as well as Cmake.

1. [OpenCV Install](https://docs.opencv.org/trunk/d7/d9f/tutorial_linux_install.html)
2. [CMake install](https://cmake.org/install/)

Different videos can be used. Ensure that they are in .mp4 format and that the file name is changed accordingly - line 66.

**Directory Structure**

![Directory](/media/tree.png)

**Expected Output**

Right now functionality is limited. The players can be selected by the user and will be tracked over the length of the video. From there, several masks are applied to filter out the court and then to filter out the court lines and to determine the lines. A mapping is generated from the image frame to real world frame and then a simple position and velocity measuring system is implemented.

![Court](/media/court.png)

![Tracking](/media/tracker.png)

**Rubric attempts**

This is a work in progress...
