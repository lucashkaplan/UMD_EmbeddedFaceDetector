#ifndef stage_h
#define stage_h

#include <stdio.h>
//#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

#include "data.h"

using namespace std;

void initialize_weights(vector<img_data>&);
void normalize_weights(vector<img_data>&);
void update_weights(vector<img_data>&, float);
void revise_dataset(vector<img_data>&);
void stage(vector<img_data>&, feature**, const vector<opt_feat>&);
eval eval_TPR_FPR(const vector<img_data>&);
bool compareVals(const img_data&, const img_data&);
opt_feat feature_select(feature**, vector<img_data>&,const int, const vector<int>&);


#endif
