#include <iostream>
#include "FaceRecognizer.hpp"
#include "LBPHTrainer.hpp"

using namespace std;
using namespace cv;

bool retrain = false;
bool detectEyes = false;

//picture variables
bool showImage = false;

//video variables
bool doVideo = false;

//camera variables
bool doCamera = false;

string csv_file;
string sourceInput;

const string model_file = "data/face_model.yml";

void help(string progname) {
    cout << endl << "Usage: " << progname << " <sourceInput>" << endl;
    cout << "Required:" << endl;
    cout << "   <sourceInput> source file or camera to search to faces" << endl;
    cout << "Photo arguments:" << endl;
    cout << "   -s show recognized face in window" << endl;
    cout << "Video arguments:" << endl;
    cout << "   -v detects video from specified source file" << endl;
    cout << "Camera arguments:" << endl;
    cout << "   -c detects video from specified camera source" << endl;
    cout << "Optional:" << endl;
    cout << "   -t <csvTrainingFile> csv directory file to train new LBPH model" << endl;
    cout << "   -eyes detect eyes in source" << endl ;
    cout << endl;
    cout << "--help for this page" << endl;
}

void detectFromImage(FaceRecognizer recog) {

    Mat m = imread(sourceInput);
    if(!m.data) {
        cout << "--(!)Picture to recognize faces was unable to be loaded." << endl;
        exit(EXIT_FAILURE);
    }

    recog.showImage = showImage;
    recog.setMinimumFaceSize(Size(30,30));
    recog.detectFaces(m);
    recog.recognizeFaces();
    // Could add to not draw faces
    recog.drawFaces();
    recog.logDetectedFaces(false);
}

void detectFromWebcam(FaceRecognizer recog) {
    if(!isdigit(sourceInput[0])) {
        cout << "--(!) Error: Invalid camera input" << endl;
        exit(EXIT_FAILURE);
    }
    recog.startVideoDetection(stoi(sourceInput), "");
}

void detectFromVideoFile(FaceRecognizer recog) {
    recog.startVideoDetection(-1, sourceInput);
}

int main(int argc, char *argv[]) {


    // convert arguments to vector
    vector<string> arguments(argv, argv + argc);

    if((argc < 2) || (arguments[1] == "--help")) {
        help(arguments[0]);
        //TODO: Add optional cascade paths and model output path and output log path
        // "** -c_face <faceCascadePath> new face cascade path"
        // "** -c_eyes <eyesCascadePath> new eyes cascade path
        exit(EXIT_FAILURE);
    }

    sourceInput = arguments[1];

    // Not really sure if this works right but no errors so far
    for(size_t i = 2; i < arguments.size(); i++) {

        string arg = arguments[i];

        if(arg == "-t") {
            retrain = true;
            csv_file = arguments[i+1];
            arguments.erase(arguments.begin() + i + 1);
        } else if(arg == "-s") {
            showImage = true;
        } else if(arg == "-eyes") {
            detectEyes = true;
        } else if(arg == "-c") {
            doCamera = true;
        } else if(arg == "-v") {
            doVideo = true;
        } else {
            cout << "ignored: " << arguments[i] << endl;
        }
    }

    if(!std::ifstream(model_file).good() || retrain) {
        LBPHTrainer::train(csv_file, model_file);
    }

    // Add output for recognized stuff maybe
    FaceRecognizer recog(model_file);
    recog.detectEyes = detectEyes;

    if(doCamera) {
        detectFromWebcam(recog);
    } else if(doVideo) {
        detectFromVideoFile(recog);
    } else {
        detectFromImage(recog);
    }

    return 0;
}