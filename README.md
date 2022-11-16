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

The video will be color filtered using the *OpenCv* library. The threshold values were obtained using a [color detection script](https://toptechboy.com/tracking-an-object-based-on-color-in-opencv/) from a tutorial.  

The “erode” process will erase a lot of the noise that gets in the recording, however this also makes the objects smaller. It is necessary to apply “dilate” to the video as well.


Once all the above methods are applied, two masks are created. One for the orange color and another for the green color.


Moments are computed for each mask. This is done so the application knows the moment any of them show into the recording. This will end with two results:


- If the orange color is on the screen, the coordinates of the center of the color circle (x,y,0) will be saved inside a file called “movements.csv”.


- If the green color is on the screen, the coordinates (0,0,1) will be saved in the previous csv file.


![points](https://user-images.githubusercontent.com/103103116/202120386-37b40f48-54ed-479f-a98a-ae1c9d2fcca3.PNG)


### Graph module

It is important to know if the capture procedure worked as intended. A picture made from the coordinates "x" and "y" is a good way to tell.
This script will take the data file "movements.csv" and put the coordinates on a Pandas data frame. Then it will draw the figure using the library *Matplotlib.* 


![Figure_draw](https://user-images.githubusercontent.com/103103116/202120869-04f62e48-f306-443e-9973-24d8eaeff5a5.jpeg)

## Transform module

Once the coordinates are obtained from the previous module, it is now time to turn them into robot language. 


This project uses the robot Kawasaki model RS03N. The language used to program this kind of robot is called AS language and has two basic commands: JAPPRO and JMOVE.
The command to use will be determined by the third coordinate obtained in the previous step:
- If it's 0, the command will be **JMOVE**.
- If it's 1, the command will be **JAPPRO**.


![instructions](https://user-images.githubusercontent.com/103103116/202120469-6a3951b7-0e0f-4845-b9bd-584ded69c0b8.PNG)


Every AS script will need to begin with **SPEED**. This command specifies the speed of the robot during its movements. 

Finally, each robot file ends with the command **END**. With all of this in mind, a file named "robot.csv" will be created with the previous instructions inside.
For more information about the AS language, please check the [manual.](https://kt-segment.ru/wp-content/uploads/2018/09/90209-1022DEC_E-Series-AS-Language_Reference_Manual.pdf)


This file is ready to send to the robot using TCP/IP protocol. However, most often than not, the robot can't be easily physically accessed. In such cases, the file must reach it by other means.


One way proposed here is using a NoSQL database hosted inside the cloud: **[MongoDB Atlas.](https://www.mongodb.com/atlas/database)** In this case, each instruction file will be a collection inside the database.

To send the instructions, the script uses the library *Pymongo* and the users credentials. Creating an account is free as of November 2022.
![mongodb](https://user-images.githubusercontent.com/103103116/202111315-ea3a2417-5320-4b59-ac29-9a675a06ce60.PNG)


## Load module

The computer connected to the robot must receive the instruction set file in order to proceed. The library *Pymongo* is once again used to download the whole collection inside a Pandas data frame. Then a new file will be created, this time inside the target computer.

### Client/server connection
 
To send the instructions, a TCP/IP connection will be required. This is done using the library *socket* and knowing the IP of the robot in advance.
Since there is not a robot to use the script directly, the instructions will be sent to an Ubuntu Server that will serve as a target.


The file was downloaded successfully:


![downloaded_file](https://user-images.githubusercontent.com/103103116/202116068-e982178e-51f1-4ead-952f-4721354c38c9.PNG)


The commands were sent to the target:
![client](https://user-images.githubusercontent.com/103103116/202116091-f1e0f427-4cf2-4673-b6ec-c8d6b70961bb.PNG)


## Technology used

- Python version 3.11.0.
- Libraries: OpenCV, numpy, pandas, matplotlib, pymongo, socket.
- MongoDB Atlas.
- Oracle VM VirtualBox with Ubuntu server. 

## Acknowledgements

- OpenCV tutorial from https://toptechboy.com/tracking-an-object-based-on-color-in-opencv/
- All of this project is an improvement of an electrical engineering graduation capstone project made in ESPOL-Ecuador called ***"Implementation of a vision system and assisted control of an industrial robotic arm"***. The authors were Luis Malusin and myself, David Freire.
