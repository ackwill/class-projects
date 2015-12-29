#include <iostream>
#include "FaceRecognizer.hpp"
#include "LBPHTrainer.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

    bool retrain = false;
    bool showImage = false;
    bool detectEyes = false;
    string csv_file;
    string fileToRec;
    const string model_file = "data/face_model.yml";

    // convert arguments to vector
    vector<string> arguments(argv, argv + argc);

    if((argc < 2) || (arguments[1] == "--help")) {
        cout << endl << "Usage: " << arguments[0] << " <testFile>" << endl;
        cout << "Required:" << endl;
        cout << "   <testFile> file to recognize faces" << endl;
        cout << "Optional:" << endl;
        cout << "   -t <csvTrainingFile> csv directory file to train new LBPH model" << endl;
        cout << "   -s show recognized face" << endl;
        cout << "   -eyes " << endl << endl;
        cout << "--help for this page" << endl;
        //TODO: Add optional cascade paths and model output path and output log path
        // "** -c_face <faceCascadePath> new face cascade path"
        // "** -c_eyes <eyesCascadePath> new eyes cascade path
        exit(1);
    }

    fileToRec = arguments[1];

    // Not really sure if this works right but no errors so far
    for(size_t i = 2; i < arguments.size(); i++) {
        if(arguments[i] == "-t") {
            retrain = true;
            csv_file = arguments[i+1];
            arguments.erase(arguments.begin() + i + 1);
        } else if(arguments[i] == "-s") {
            showImage = true;
        } else if(arguments[i] == "-eyes") {
            detectEyes = true;
        } else {
            cout << "ignored: " << arguments[i] << endl;
        }
    }

    if(!std::ifstream(model_file).good() || retrain) {
        LBPHTrainer::train(csv_file, model_file);
    }

    // Add output for recognized stuff maybe
    FaceRecognizer recg(model_file);
    Mat m = imread(fileToRec);
    if(!m.data) {
        cout << "--(!)Picture to recognize faces was unable to be loaded." << endl;
        exit(1);
    }

    recg.detectEyes = detectEyes;
    recg.showImage = showImage;
    recg.setMinimumFaceSize(Size(30,30));
    recg.DetectFaces(m);
    recg.RecognizeFaces();
    recg.DrawFaces();
    recg.logDetectedFaces(false);

    return 0;
}