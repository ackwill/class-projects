#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "FaceRecognizer.hpp"
#include "LBPHTrainer.hpp"

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

bool retrain = false;
bool detectEyes = false;
bool debug = false;

//picture variables
bool showImage = false;

//video variables
bool doVideo = false;

//camera variables
bool doCamera = false;

//Add new faces
bool addFace = false;

string csv_file;
string sourceInput;

const string model_file = "data/face_model.yml";

void help(string progname) {
    cout << endl;
    cout << "Usage: " << progname << " <sourceInput> [arguments] [optional]" << endl;
    cout << "Usage: " << progname << " <csvFile> -createCSV" <<endl;
    cout << "Required:" << endl;
    cout << "   <sourceInput> source file or camera to search to faces" << endl;
    cout << "Photo arguments:" << endl;
    cout << "   -s show recognized face in window" << endl;
    cout << "Video arguments:" << endl;
    cout << "   -v detects video from specified source file" << endl;
    cout << "Camera arguments:" << endl;
    cout << "   -c detects video from specified camera source" << endl;
    cout << "Take New pictures:" << endl;
    cout << "   <sourceInput> should be a csv file path" << endl;
    cout << "   -newPic Adds a new person to the face database " << endl;
    cout << "   -T to train model to new data" << endl;
    cout << "Optional:" << endl;
    cout << "   -t <csvTrainingFile> csv directory file to train new LBPH model" << endl;
    cout << "   -eyes detect eyes in source" << endl ;
    cout << endl;
    cout << "--help for this page" << endl;
}

void prepDirectory() {
    if(!fs::exists("data/")) {
        fs::create_directory("data/");
    }
    if(!fs::exists("output/"))
        fs::create_directory("output/");
}

void detectFromImage(FaceRecognizer recog) {

    Mat m = imread(sourceInput);
    if(!m.data) {
        cout << "--(!) Picture to recognize faces was unable to be loaded." << endl;
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
    recog.doingVideo = true;
    recog.startVideoDetection(stoi(sourceInput), "");
}

void detectFromVideoFile(FaceRecognizer recog) {
    recog.doingVideo = true;
    recog.startVideoDetection(-1, sourceInput);
}

void addNewFaces(FaceRecognizer recog) {
    string person;
    cout << "Enter the name of who you are entering: ";
    cin >> person;

    fs::path p(csv_file);
    p.remove_leaf();
    fs::path path = p.string() + "/" + boost::algorithm::to_lower_copy(person);

    if(!fs::exists(path)) {
        if(!fs::exists(p)) fs::create_directory(p);
        fs::create_directory(path);
    }

    recog.takePictures(path.string());
    LBPHTrainer::updateCSV(csv_file);
}

int main(int argc, char *argv[]) {

    // convert arguments to vector
    vector<string> arguments(argv, argv + argc);

    if((argc < 2) || (arguments[1] == "--help") || (arguments[1] == "-help")) {
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
            csv_file = arguments[i + 1];
            arguments.erase(arguments.begin() + i + 1);
        } else if(arg == "-s") {
            showImage = true;
        } else if(arg == "-eyes") {
            detectEyes = true;
        } else if(arg == "-c") {
            doCamera = true;
        } else if(arg == "-v") {
            doVideo = true;
        } else if(arg == "-newPic") {
            addFace = true;
            csv_file = arguments[1];
        } else if(arg == "-T") {
            retrain = true;
        } else if(arg == "-debug") {
            debug = true;
        } else if(arg == "-createCSV") {
            LBPHTrainer::updateCSV(arguments[1]);
            cout << "Created new csv file to: " << arguments[1] << endl;
            exit(EXIT_SUCCESS);
        } else {
            cout << "ignored: " << arguments[i] << endl;
        }
    }

    //Create missing directories
    prepDirectory();

    if((!std::ifstream(model_file).good() || retrain) && !addFace) {
        LBPHTrainer::train(csv_file, model_file);
    }

    // Add output for recognized stuff maybe
    FaceRecognizer recog(model_file);
    recog.detectEyes = detectEyes;

    if(doCamera) {
        detectFromWebcam(recog);
    } else if(doVideo) {
        detectFromVideoFile(recog);
    } else if(addFace) {
        addNewFaces(recog);
        if(retrain) {
            LBPHTrainer::train(csv_file, model_file);
        }
    } else if(debug) {
        cout << "Debug mode" << endl;
    } else {
        detectFromImage(recog);
    }

    return 0;
}