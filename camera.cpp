
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "math.h"

#include <pylon/PylonIncludes.h> 
#ifdef PYLON_WIN_BUILD 
#include <pylon/PylonGUI.h> 
#endif 

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace cv; //Namespace for opencv objects 
using namespace Pylon; // Namespace for using pylon objects 
static const uint32_t c_countOfImagesToGrab = 100; // Number of images to be grabbed 

int H_min = 22, S_min = 115, V_min = 107; //Declare min and max HSV values 
int H_max = 180, S_max = 256, V_max = 256;
const double offset = 50.0; //tcp measure to center the robot effector 
const double z_ori = -32.0; //Z value for base pose robot 


string message;
int main(int argc, char** argv)
{
	int exitCode = 0;
	Pylon::PylonAutoInitTerm autoInitTerm; // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system is initialized during the lifetime of this object 
	CGrabResultPtr ptrGrabResult; //This smart pointer will receive the grab result data 
	//	namedWindow("CV_Image", WINDOW_AUTOSIZE); //Create an opencv display window 
	ofstream archivo;
	archivo.open("Input.txt", ios::out); //abriendo el archivo
	try
	{
		CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice()); //Create an instant camera object with the camera device found first 
		cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl; //Print the model name of the camera 
		camera.Open(); //Open the camera before accesing any parameters

		GenApi::CIntegerPtr width(camera.GetNodeMap().GetNode("Width"));
		GenApi::CIntegerPtr height(camera.GetNodeMap().GetNode("Height"));
		camera.MaxNumBuffer = 5; //Count of buffers allocated for grabbing (max 10) 
		camera.StartGrabbing();

		CImageFormatConverter fc, fc1; //Create a pylon ImageFormatConverter object 

		fc.OutputPixelFormat = PixelType_BGR8packed; //Specify the output pixel format 
		fc1.OutputPixelFormat = PixelType_BGR8packed; //Specify the output pixel format 

		CPylonImage image, image1; //Create a pylonImage that will be used to create a opencv images later 


		namedWindow("Control", CV_WINDOW_AUTOSIZE);
		namedWindow("ControlN", CV_WINDOW_AUTOSIZE);

		int iLowH = 0;
		int nlowH = 0;

		int iHighH = 20;
		int nhighH = 179;

		int iLowS = 148;
		int nlowS = 33;

		int iHighS = 241;
		int nhighS = 255;

		int iLowV = 90;
		int nlowV = 23;

		int iHighV = 236;
		int nhighV = 75;

		//CREATE TRACKBARS
		cvCreateTrackbar("LowH", "Control", &iLowH, 179);
		cvCreateTrackbar("HighH", "Control", &iHighH, 179);

		cvCreateTrackbar("LowS", "Control", &iLowS, 255);
		cvCreateTrackbar("HighS", "Control", &iHighS, 255);

		cvCreateTrackbar("LowV", "Control", &iLowV, 255);
		cvCreateTrackbar("HighV", "Control", &iHighV, 255);

		//VENTANA PARA EL COLOR NEGRO
		cvCreateTrackbar("LowH", "ControlN", &nlowH, 179);
		cvCreateTrackbar("HighH", "ControlN", &nhighH, 179);

		cvCreateTrackbar("LowS", "ControlN", &nlowS, 255);
		cvCreateTrackbar("HighS", "ControlN", &nhighS, 255);

		cvCreateTrackbar("LowV", "ControlN", &nlowV, 255);
		cvCreateTrackbar("HighV", "ControlN", &nhighV, 255);
		
		int iLastX = -1;
		int iLastY = -1;

		//captura temporal de imagen de camara
		camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException); //Wait for an image an then retrieve it. A timeout of 5000ms is used 
		fc1.Convert(image1, ptrGrabResult); //Convert the grabbed buffer to a pylon image
		Mat origen = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t*)image1.GetBuffer());

		//imagen oscura del mismo tamano de la salida de la camara
		Mat imgLines = Mat::zeros(origen.size(), CV_8UC3);

		while (camera.IsGrabbing())
		{
			camera.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException); //Wait for an image an then retrieve it. A timeout of 5000ms is used 

			if (ptrGrabResult->GrabSucceeded())
			{
				fc.Convert(image, ptrGrabResult); //Convert the grabbed buffer to a pylon image
				Mat ori_img = cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t*)image.GetBuffer()); //Create an opencv image from a pylon image 



				Mat imgHSV,imgN;

				cvtColor(ori_img, imgHSV, COLOR_BGR2HSV);
				cvtColor(ori_img, imgN, COLOR_BGR2HSV);

				Mat imgThresholded;
				Mat imgnegro;

				inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded);
				//	threshold(imgHSV, imgThresholded, 0, 255, THRESH_BINARY);
				erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				//PARTE DE COLOR NEGRO
				inRange(imgN, Scalar(nlowH, nlowS, nlowV), Scalar(nhighH, nhighS, nhighV), imgnegro);
				//	threshold(imgHSV, imgThresholded, 0, 255, THRESH_BINARY);
				erode(imgnegro, imgnegro, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				dilate(imgnegro, imgnegro, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				dilate(imgnegro, imgnegro, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
				erode(imgnegro, imgnegro, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

				//Calcula los momentos de la imagen color rojo
				Moments oMoments = moments(imgThresholded);

				double dM01 = oMoments.m01;
				double dM10 = oMoments.m10;
				double dArea = oMoments.m00;


				//Calcula los momentos de la imagen color negro
				Moments oMomentsN = moments(imgnegro);

				double dM01N = oMomentsN.m01;
				double dM10N = oMomentsN.m10;
				double dAreaN = oMomentsN.m00;
				//si el area es menor a 10000, considerar que no hay objeto en imagen
				if (dArea > 10000)
				{
					//calcula la posicion
					int posX = dM10 / dArea;
					int posY = dM01 / dArea;

					if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
					{
						//dibuja una linea roja del punto anterior al actual
						line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
					}
					double distancia; //se calcula la distancia entre el punto actual y el anterior
					distancia = sqrt(((posX - iLastX)*(posX - iLastX)) +((posY - iLastY)*(posY - iLastY)));

					int tempx, tempy;
					tempx = 0.37*(posX - 636);
					tempy = 0.30*(posY - 450);
					cout << "posicion en x es: " << posX << endl;
					cout << "posicion en y es: " << posY << endl;
					cout << "distancia total es: " << distancia << endl;
					archivo << tempx << ',' << tempy <<','<< 0 << endl;

					iLastX = posX;
					iLastY = posY;

					if (archivo.fail())
					{
						cout << "NO SE PUDO ABRIR EL ARCHIVO" << endl;
						exit(1);
					}

				}

				else if (dAreaN > 10000)
				{
					archivo <<0<<','<< 0<<','<<1<< endl;
				}

				imshow("Thresholded Image", imgThresholded);
				ori_img = ori_img + imgLines;
				imshow("Original", ori_img);
				imshow("Ventana color negro", imgnegro);
				if (waitKey(30) == 27)
				{
					cout << "esc key pressed" << endl;
					break;
				}

				if (waitKey(30) == 32)
				{
					imgLines = 0;
				}
			}
		}
	}
	catch (GenICam::GenericException &e)
	{
		cerr << "An exception occurred." << endl << e.GetDescription() << endl;
		exitCode = 1;
	}
	archivo.close();
	return exitCode;
}



