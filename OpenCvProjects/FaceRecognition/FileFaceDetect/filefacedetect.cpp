#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core.hpp"
#include "opencv2/face.hpp"

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;
using namespace cv::face;

/** Function Headers */
void detectAndDisplay( Mat frame );

/** Global variables */
String face_cascade_name = "data/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "data/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

/** @function main */
int main( int argc, const char** argv )
{
    Mat frame;
    
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    
    //frame = imread("data/ballplayers.png");
    frame = imread(argv[1]);
    detectAndDisplay(frame);
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{
    std::vector<Rect> faces;
    Mat frame_gray;
    
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    
    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
    
    Ptr<LBPHFaceRecognizer> model = createLBPHFaceRecognizer();
    model->load("lbphfaces_at.yml");
    
    for( size_t i = 0; i < faces.size(); i++ )
    {
        //Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
        //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        rectangle(frame, faces[i], Scalar(255,0,255));
        
        Mat faceROI = frame_gray( faces[i] );
        std::vector<Rect> eyes;
        
        //-- In each face, detect eyes
        eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        
        for( size_t j = 0; j < eyes.size(); j++ )
        {
            Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
        }
        
        int predictedLabel = model->predict(frame_gray(faces[i]));
        string personName;
        switch (predictedLabel) {
            case 0:
                personName = "Will";
                break;
            case 1:
                personName = "Chris";
            default:
                break;
        }
        
        int baseline = 0;
        Size label_size = getTextSize(personName, FONT_HERSHEY_PLAIN, 1.0, 1, &baseline);
        
        putText(frame, personName, Point((faces[i].x + (0.5*faces[1].width)) - (0.5*label_size.width), faces[i].y), FONT_HERSHEY_PLAIN, 1.0, Scalar(127, 255, 0));
    }
    
    
    
    imwrite("output/pic.png", frame);
}