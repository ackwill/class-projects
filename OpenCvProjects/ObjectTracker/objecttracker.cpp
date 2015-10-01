//
//  objecttracker.cpp
//  XCodeBlurImage
//
//  Created by Will Pittman on 9/22/15.
//  Copyright (c) 2015 Will Pittman. All rights reserved.
//
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

string window_name = "Trackign Window";
Scalar colorLow = Scalar(29, 86, 6);
Scalar colorHigh = Scalar(64, 255, 255);

//Scalar colorLow = Scalar(75, 91, 28);
//Scalar colorHigh = Scalar(76, 100, 100);

int createWindow(VideoCapture& capture);
int processImage(VideoCapture& capture);
void help(char**av);

void help(char** av) {
    
}

int processImage(VideoCapture& capture) {
    
    //int n = 0;
    Mat frame;
    Mat mask;
    
    for(;;) {
        
        capture >> frame;
        
        if(frame.empty()) {
            break;
        }
        
        GaussianBlur(frame, frame, Size(11,11), 0, 0);
        cvtColor(frame, mask, COLOR_BGR2HSV);
        
        inRange(mask, colorLow, colorHigh, mask);
        erode(mask, mask, 2);
        dilate(mask, mask, 2);
        
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        
        findContours(mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
        
        vector<vector<Point> > contours_poly( contours.size() );
        vector<Rect> boundRect( contours.size() );
        vector<RotatedRect> minRect(contours.size());
        vector<Point2f>center( contours.size() );
        vector<float>radius( contours.size() );

        int largestRectangle = 0;
        
        for( int i = 0; i < contours.size(); i++ )
        {
            approxPolyDP( Mat(contours[i]), contours_poly[i], arcLength(cv::Mat(contours[i]), true)*0.02, true );
            boundRect[i] = boundingRect( Mat(contours_poly[i]) );
            minRect[i] = minAreaRect(Mat(contours[i]));
            minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
        }
        
        for( int i = 0; i< contours.size(); i++ )
        {
            if(boundRect[i].area() > boundRect[largestRectangle].area()) {
                largestRectangle = i;
            }
            Scalar color = Scalar(255,255,255);
            //drawContours( frame, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
            if(boundRect[i].area() > 500) {
                rectangle( frame, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );
            }
            /*
            Point2f rect_points[4]; minRect[i].points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( frame, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
             */
            /* Draw Circles
            if((int)radius[i] > 20.0) {
                if(isContourConvex(contours_poly)) {
                circle( frame, center[i], (int)radius[i], color, 2, 8, 0 );
            }
            */
        }
        
        //flip(frame, frame, 1);
        
        //imshow(window_name, frame);
        
        // Zoom to rectangle
        Mat roi;
        if(boundRect[largestRectangle].area() > 500) {
            roi = frame(boundRect[largestRectangle]);
            //resize(roi, roi, frame.size());
        } else {
            roi = frame;
        }
        
         
        
        //resize(roi, roi, Size(1280, 720));
        //cout << roi.size();
        
        imshow(window_name, roi);
        
        char key = (char)waitKey(30); //delay N millis, usually long enough to display and capture input
        
        switch (key) {
            case 'q':
            case 'Q':
            case 27: //escape key
                return 0;
            default:
                break;
        }
    }
    return 1;
}

int createWindow(VideoCapture& capture) {
    
    namedWindow(window_name, WINDOW_FREERATIO);
    
    return processImage(capture);
}

int main(int ac, char** av) {
    VideoCapture capture(0);
    if(!capture.isOpened()) {
        cout << "--Error opening camera--";
        return 1;
    }
    return createWindow(capture);
}