//
// Created by Will Pittman on 12/28/15.
//

#include "LBPHTrainer.hpp"

void LBPHTrainer::train(const string &path_to_csv, const string &path_to_save) {
    vector<Mat> images;
    vector<int> labels;
    vector<string> info;

    read_csv(path_to_csv, images, labels, info);

    if(images.size() <= 1) {
        string error_message = "This needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(Error::StsError, error_message);
    }

    Ptr<face::LBPHFaceRecognizer> model = face::createLBPHFaceRecognizer();

    model->train(images, labels);

    for(size_t i = 0; i < info.size(); i++) {
        model->setLabelInfo(i, info[i]);
    }

    model->save(path_to_save);

    cout << "Created  LPBH model for:" << endl;

    for(int i = 0; i < info.size(); i++) {
        cout << model->getLabelInfo(i) << endl;
    }

    cout << "Saved model to " << path_to_save << endl;
}

void LBPHTrainer::read_csv(const string &file, vector<Mat> &images, vector<int> &labels, vector<string> &info) {
    ifstream csv_file(file);
    string line, path, label;
    if(csv_file.is_open()) {
        while(getline(csv_file,line)) {
            if(line[0] == '#') {
                line.erase(0,1);
                info.push_back(line);
            } else {
                stringstream ss(line);
                getline(ss, path, ';');
                getline(ss, label);
                if (!path.empty() && !label.empty()) {
                    //Load a greyscale image
                    images.push_back(imread(path, 0));
                    labels.push_back(stoi(label));
                }
            }
        }
        csv_file.close();
    } else {
        cout << "File unable to be opened.";
        exit(EXIT_FAILURE);
    }
}
