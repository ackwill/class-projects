//
// Created by Will Pittman on 12/28/15.
//

#ifndef FACERECOGNIZER_LBPHTRAINER_HPP
#define FACERECOGNIZER_LBPHTRAINER_HPP

#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

class LBPHTrainer {
    static void read_csv(const string &file, vector<Mat> &images, vector<int> &labels, vector<string> &info);

public:
    static void train(const string &path_to_csv, const string &path_to_save);\
    static void updateCSV(const string &pathToCsv);

};

#endif //FACERECOGNIZER_LBPHTRAINER_HPP
