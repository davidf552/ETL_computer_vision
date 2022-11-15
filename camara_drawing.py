# Import essential libraries
import cv2 as cv
import numpy as np

webcam = cv.VideoCapture('record.mp4')

#orange color threshold
o_low_H=9
o_high_H=20
o_low_S=64
o_high_S=250
o_low_V=97
o_high_V=250

#green color threshold
g_low_H=39
g_high_H=79
g_low_S=54
g_high_S=203
g_low_V=62
g_high_V=217


kernel =  cv.getStructuringElement(cv.MORPH_ELLIPSE,(5,5))

file = open("movements.csv","w")

while True:
    
    ret,frame = webcam.read()

    if frame is None:
        break

    orange_frame = cv.cvtColor(frame,cv.COLOR_BGR2HSV)
    green_frame = cv.cvtColor(frame,cv.COLOR_BGR2HSV)

    #Threshold limits for each color
    
    o_lowerBound=np.array([o_low_H,o_low_S,o_low_V])
    o_upperBound=np.array([o_high_H,o_high_S,o_high_V])
    
    g_lowerBound=np.array([g_low_H,g_low_S,g_low_V])
    g_upperBound=np.array([g_high_H,g_high_S,g_high_V])

    #Apply the color filter to each frame. Erode and dilate
    #are needed to eliminate noise on the video

    o_myMask=cv.inRange(orange_frame,o_lowerBound,o_upperBound)
    g_myMask=cv.inRange(green_frame,g_lowerBound,g_upperBound)
 
    o_myMask = cv.erode(o_myMask,kernel,iterations=1)
    g_myMask = cv.erode(g_myMask,kernel,iterations=1)

    o_myMask = cv.dilate(o_myMask,kernel,iterations=1)
    g_myMask = cv.dilate(g_myMask,kernel,iterations=1)
    

    #Calculating x and y coordinates
    
    O_Moments = cv.moments(o_myMask)
    G_Moments = cv.moments(g_myMask)

    #If the orange color is on the screen:
    if (O_Moments["m00"] > 10000):
        
        x = int(O_Moments["m10"] / O_Moments["m00"])
        y = int(O_Moments["m01"] / O_Moments["m00"])
        
        tempx = str(int(0.37*(x - 636)))
        tempy = str(int(0.30*(y - 450)))
        
        file.write(tempx + "," + tempy + "," + "0" + "\n")


    #If the green color is on the screen:
    elif (G_Moments["m00"] > 10000):
        file.write("0" + "," + "0" + "," + "1" + "\n")

    # Press Esc key to exit
    key = cv.waitKey(30)
   
    if key == ord('q') or key == 27:
        break
    
webcam.release()
file.close()
cv.destroyAllWindows()





