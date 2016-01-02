//
// Created by Will Pittman on 12/28/15.
//

#include "LBPHTrainer.hpp"

namespace fs = boost::filesystem;

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
        cout << "Csv file unable to be opened.";
        exit(EXIT_FAILURE);
    }
}

void LBPHTrainer::updateCSV(const string &pathToCsv) {
    fs::path path(pathToCsv);
    ofstream csvFile(pathToCsv);
    vector<fs::path> paths;

    if(csvFile.is_open()) {
        path.remove_leaf();

        //Get all directories in faces directory
        for (fs::directory_iterator dir_iter(path); dir_iter != fs::directory_iterator(); ++dir_iter) {
            fs::path p = dir_iter->path();
            if (fs::is_directory(p))
                paths.push_back(p);
        }

        //Add new faces to csv file
        for (int i = 0; i < paths.size(); i++) {
            string name = paths[i].leaf().string();
            name[0] = (char) toupper(name[0]);
            csvFile << "#" << name << endl;
            for (fs::directory_iterator dir_iter(paths[i]); dir_iter != fs::directory_iterator(); ++dir_iter) {
                fs::path p = dir_iter->path();
                if (fs::is_regular_file(p) && p.filename().string()[0] != '.')
                    csvFile << p.string() << ";" << i << endl;
            }
        }
        csvFile.close();
    } else {
        cout << "Csv file unable to be opened.";
        exit(EXIT_FAILURE);
    }

}
