//
// Created by Will Pittman on 12/28/15.
//

#include "FaceRecognizer.hpp"

// Load cascades and LBPHFaceRecognizer model from files
void FaceRecognizer::loadFiles() {
    if( !face_cascade.load( face_cascade_name ) ) {
        cout << "--(!)Error loading face cascade" << endl;
        exit(1);
    };
    if( !eyes_cascade.load( eyes_cascade_name ) ) {
        cout << "--(!)Error loading eyes cascade" << endl;
        exit(1);
    };

    model->load(recognizer_model_file);
}

FaceRecognizer::FaceRecognizer(const string &recognizer_file) {
    recognizer_model_file = recognizer_file;
    loadFiles();

}

FaceRecognizer::FaceRecognizer(const string &recognizer_file, const string &face_cascade, const string &eyes_cascade) {
    recognizer_model_file = recognizer_file;
    // TODO: check to make sure they are not empty
    face_cascade_name = face_cascade;
    eyes_cascade_name = eyes_cascade;
    loadFiles();
}

void FaceRecognizer::DetectFaces(Mat picture) {
    frame = picture;

    cvtColor( frame, gray_frame, CV_BGR2GRAY );
    equalizeHist( gray_frame, gray_frame );

    vector<Rect> tmpFaces;

    // Detect faces and store them in tmpFaces
    face_cascade.detectMultiScale( gray_frame, tmpFaces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, minimum_face_size );

    // Create a FaceRec from the faces found in the image and find their center
    faces.resize(tmpFaces.size());
    for(size_t i = 0; i < tmpFaces.size(); i++) {
        faces[i].rect = tmpFaces[i];
        faces[i].center = Point(tmpFaces[i].x + tmpFaces[i].width*0.5, tmpFaces[i].y + tmpFaces[i].height*0.5 );
    }

    //TODO: Make captured face to scale with trained images
    // WHY? Because it might help
    /* Makes capture face to scale with trained images
    int width = faces[i].rect.width;
    int height = (450 * width)/320;
    Rect scaledROI(faces[i].center.x-(width/2), faces[i].center.y-(height/2), width, height);
    */

    // Detect eyes if told to do so
    if(detectEyes) {
        for (size_t i = 0; i < faces.size(); i++) {
            Mat faceMat = frame(faces[i].rect);
            eyes_cascade.detectMultiScale(faceMat, faces[i].eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
        }
    }

}

void FaceRecognizer::RecognizeFaces() {
    for(size_t i = 0; i < faces.size(); i++) {
        Mat predMat;

        // Resize to size of trained images
        //TODO: this distorts the images and probably should be scaled above
        resize(gray_frame(faces[i].rect), predMat, Size(320, 450));

        // Predict the face
        int predictedLabel = -1;
        double confidence = 0.0;
        model->predict(predMat, predictedLabel, confidence);

        // Assign values to the face
        faces[i].label = predictedLabel;
        faces[i].confidence = confidence;
        faces[i].info = model->getLabelInfo(predictedLabel);
    }
}

void FaceRecognizer::DrawFaces() {
    for (size_t i = 0; i < faces.size(); i++) {
        // Draw the face rectangle
        rectangle(frame, faces[i].rect, Scalar(255, 0, 255), 2*getTextScaleFromHeight());

        // Draw the eyes if told to do so
        if (detectEyes) {
            for (size_t j = 0; j < faces[i].eyes.size(); j++) {
                Point center(faces[i].rect.x + faces[i].eyes[j].x + faces[i].eyes[j].width * 0.5,
                             faces[i].rect.y + faces[i].eyes[j].y + faces[i].eyes[j].height * 0.5);
                int radius = cvRound((faces[i].eyes[j].width + faces[i].eyes[j].height) * 0.25);
                circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
            }
        }

        //int baseline = 0;
        //Size label_size = getTextSize(faces[i].info, FONT_HERSHEY_PLAIN, 1.0, 1, &baseline);
        double scale = getTextScaleFromHeight();
        putText(frame, faces[i].info, Point(faces[i].rect.x, faces[i].rect.y + faces[i].rect.height), CV_FONT_NORMAL, scale, Scalar(127, 255, 0), 2*scale, true);

    }

    //TODO: Change this for when detected from camera
    // Write image to file and display it if asked to do so
    imwrite("output/detected.png", frame);

    if(showImage) {
        while (1) {

            imshow("Faces Detected", frame);

            char key = (char) waitKey(20);
            // Exit this loop on escape or 'q'
            if (key == 27 || key == 'q') break;

        }
    }
}

// Logs all faces or just the ones it recognizes depending on what it's told
void FaceRecognizer::logDetectedFaces(bool justRecognized) {
    const string fileName = "output/DetectedFaces.txt";
    ofstream logfile(fileName);
    if(logfile.is_open()) {
        for(size_t i = 0; i < faces.size(); i++) {

            if(faces[i].label != -1 || (justRecognized && faces[i].label != -1)) {
                logfile << "Found " << faces[i].info << " at (" << faces[i].center.x << ", " << faces[i].center.y << ") with " << faces[i].confidence << "% confidence." << endl;
            } else {
                logfile << "Found a face at (" << faces[i].center.x << ", " << faces[i].center.y << ")" << endl;
            }
        }
        logfile.close();
        cout << "Saved data to " << fileName << endl;
    } else cout << "Unable to save detected faces" << endl;
}

// Used to find scale for text and line thickness (thickness *2)
double FaceRecognizer::getTextScaleFromHeight() {
    // rows = height, cols = width
    return (0.5 * frame.rows) / 640;
}