# ETL_computer_vision
## Introduction

This project aims to program a Kawasaki Robotic Arm using a video camera. ***How do you do that?*** Keep reading to find out.

Basically, the repository contains code to do the following:

- Extracts coordinates from a video and prints them into a file.
- Transforms the previous data into AS language.
- Loads the resulting file into a MongoDB collection.
- Downloads the code into the computer connected to the robot and sends the instructions.

All the code written in this repository uses the Python programming language.

## Extract module

The application will take a video, where it will show the operator wearing a glove with colored fingertips (one orange and one green). The operator then draws anything on a flat surface using one of the colors.

The video will be color filtered using the OpenCv library. The threshold values were obtained using a [color detection script](https://toptechboy.com/tracking-an-object-based-on-color-in-opencv/) from a tutorial.  

The “erode” process will erase a lot of the noise that gets in the recording, however this also makes the objects smaller. It is necessary to apply “dilate” to the video as well.


Once all the above methods are applied, two masks are created. One for the orange color and another for the green color.


Moments are computed for each mask. This is done so the application knows when any of them show into the recording. This will end with two results:


- If the orange color is on the screen, the coordinates of the center of the color circle (x,y,0) will be saved inside a file called “movements.csv”.


- If the green color is on the screen, the coordinates (0,0,1) will be saved in the previous csv file.
