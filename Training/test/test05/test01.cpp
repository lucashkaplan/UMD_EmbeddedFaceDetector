#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <vector>
#include "data.h"
#include "calc_features.h"

using namespace std;

//#define nRows 3
//#define nCols 3

void output_features(feature **feature_ptrs, vector<img_data> img, char *out_file, int num_features){
    // output features to file
    int i, j;
    ofstream ofs(out_file,ofstream::out); //calculated feature set
    for(i=0;i<num_features;i++){ //for all features
        ofs << setw(6) << i;
        ofs << setw(3) << feature_ptrs[i]->type;
        ofs << setw(3) << int(feature_ptrs[i]->size[0]);
        ofs << setw(3) << int(feature_ptrs[i]->size[1]);
        ofs << setw(3) << int(feature_ptrs[i]->pos[0]);
        ofs << setw(3) << int(feature_ptrs[i]->pos[1]);
        ofs << endl;
        for(j=0;j<feature_ptrs[i]->val.size();j++){ //through images
            ofs << setw(6) << " ";
            ofs << setw(6) << feature_ptrs[i]->val[j];
            if (img[j].face == true){
                ofs << " T" << endl;
            }
            else{
                ofs << " F" << endl;
            }
        }
    }
    cout << "done writing to file" << endl;
    ofs.close();
}


int main(int argc, char **argv){
	int num_features = 162336;
    feature **feature_ptrs;
    try{
        feature_ptrs = new feature *[num_features];
        for(int j=0;j<num_features;j++){
            feature_ptrs[j] =  new feature;
        }
    }
    catch(const bad_alloc& e){
        cerr << "error: memory allocation failed" << e.what() << endl;
    }
	filesystem::path dataDir = "../test_images/";	
	vector<img_data> img; //one per image
    calc_features calc_feature(feature_ptrs, img, num_features, dataDir);
    calc_feature.calculate();

	output_features(feature_ptrs, img, "output.txt", num_features);
}

