//
// Created by Will Pittman on 12/28/15.
//

#include <opencv2/opencv.hpp>
#include "opencv2/face.hpp"

#include <boost/filesystem.hpp>
#include <fstream>

#ifndef FACERECOGNIZER_FACERECOGNIZER_HPP
#define FACERECOGNIZER_FACERECOGNIZER_HPP

using namespace std;
using namespace cv;


struct FaceRect {
    //bool has_eyes = false;
    Rect rect;
    Point center;
    vector<Rect> eyes;
    int label = -1;
    double confidence;
    string info;
};

class FaceRecognizer {

    Mat frame;
    Mat gray_frame;

    vector<FaceRect> faces;

    Size minimum_face_size = Size(30,30);

    string recognizer_model_file;
    Ptr<face::LBPHFaceRecognizer> model = face::createLBPHFaceRecognizer();

    string face_cascade_name = "data/haarcascade_frontalface_alt.xml";
    string eyes_cascade_name = "data/haarcascade_eye_tree_eyeglasses.xml";
    CascadeClassifier face_cascade;
    CascadeClassifier eyes_cascade;

    void loadFiles();
    double getTextScaleFromHeight();
public:
    FaceRecognizer(const string &recognizer_file);
    FaceRecognizer(const string &recognizer_file, const string &face_cascade, const string &eyes_cascade);

    void detectFaces(Mat picture);
    void recognizeFaces();
    void drawFaces();
    void logDetectedFaces(bool justRecognized);
    void startVideoDetection(int camera, string file);

    void takePictures(string path);

    void setMinimumFaceSize(Size size) {minimum_face_size = size;};

    bool detectEyes = false;
    bool showImage = false;
    bool doingVideo = false;
};


#endif //FACERECOGNIZER_FACERECOGNIZER_HPP
