#ifndef calc_feat_h
#define calc_feat_h

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include "data.h"

using namespace std;

#define nRows 24
#define nCols 24

class calc_features{
public:
    // constructor
    calc_features(feature **feature_ptrs, vector<img_data> &img,
            const int num_features, const filesystem::path data_dir):
		feature_ptrs(feature_ptrs),
		img(img),
		num_features{num_features},
		data_dir(data_dir)
	{}
	filesystem::path data_dir;
    void calculate();

private:
	void calc_integral_img();
    int calc_feat_A(int, int, int, int);
    int calc_feat_B(int, int, int, int);
    int calc_feat_C(int, int, int, int);
    int calc_feat_D(int, int, int, int);
    int calc_feat_E(int, int, int, int);
    int calc_rect(int, int, int, int);

    feature **feature_ptrs;
    vector<img_data>& img;
    int num_features;

    string filename;
    int int_img[nRows][nCols];
};


#endif
