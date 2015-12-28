#include <iostream>
#include "FaceRecognizer.h"
#include "LBPHTrainer.hpp"

using namespace std;
using namespace cv;

int main() {

    const string model_file = "data/face_model.yml";

    bool retrain = false;

    if(!std::ifstream(model_file).good() || retrain) {
        LBPHTrainer::train("data/trainingdata/dir.txt", model_file);
    }

    FaceRecognizer recg(model_file);
    //Mat m = imread("data/testdata/tom.jpg");
    Mat m = imread("data/testdata/grad.png");
    recg.detect_eyes = false;
    recg.showImage = true;
    recg.setMinimumFaceSize(Size(30,30));
    recg.DetectFaces(m);
    recg.RecognizeFaces();
    recg.DrawFaces();
    recg.logDetectedFaces(false);

    return 0;
}