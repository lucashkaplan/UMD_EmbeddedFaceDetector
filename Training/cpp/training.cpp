#include <stdio.h>

//#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

#include "data.h" //structs 
#include "calc_features.h" //functions for calculating features
#include "stage.h" //training functions
#include "validate.h"

using namespace std;

// operator overload for printing eval
ostream& operator<<(ostream& out, const eval& in){
	
	stringstream out_ss;
	
	out_ss << "TPR: " << in.TPR << endl;
	out_ss << "FPR: " << in.FPR << endl;
	
	if(in.avg_num_WC != 0){
		out_ss <<  "Average number of weak classifiers used: ";
  		out_ss << in.avg_num_WC << endl;
	}
	if(in.num_faces != 0){
		out_ss << "Number of face images in dataset: ";
		out_ss << in.num_faces << endl;
	}
	if(in.num_nonfaces != 0){
        out_ss << "Number of non face images in dataset: ";
        out_ss << in.num_nonfaces << endl;
    }
	if(in.runtime.count() !=0){
		out_ss << "Runtime: ";
		out_ss << in.runtime.count() << " s" << endl;
	}
	return out << out_ss.str();
}

// print struct
ostream& operator<<(ostream& out, const opt_feat& in){
	
	stringstream out_ss;
	
	out_ss << "index: " << in.index << endl;
    out_ss << "parity: " << in.parity << endl;
    out_ss << "thresh: " << in.thresh << endl;
    out_ss << "error: " << in.error << endl;
	out_ss << "alpha: " << in.alpha << endl;

	return out << out_ss.str();
}



int main(int argc, char **argv){
	auto start = chrono::high_resolution_clock::now();
	int i,j;
		 
	// memory for feature data
    int num_features = 162336;
	feature **feature_ptrs;	
	try{
        feature_ptrs = new feature *[num_features];
        for(j=0;j<num_features;j++){
            feature_ptrs[j] =  new feature;
        }
        //cout << "memory allocation successful" << endl;
    }
    catch(const bad_alloc& e){
        cerr << "error: memory allocation failed" << e.what() << endl;
    }
	
	char user_input;
	filesystem::path dataDir;
	// user input
	while(true){
		cout << "Test dataset (1) or full training dataset (2)?" << endl;
		cin >> user_input;
		if(user_input == '1'){
			dataDir = "../Dataset/test/txt/";
			cout << endl << "Test dataset chosen" << endl << endl;
			break;
		}
		else if(user_input == '2'){
			dataDir = "../Dataset/full/txt/";
			cout << endl << "Full dataset chosen" << endl << endl;
			break;
		}
		else
			cout << "Invalid input" << endl << endl;
	}
	cout << setw(80) << setfill('*') << "*" << endl;
	
	vector<img_data> img; //one per image
	calc_features calc_feature(feature_ptrs, img, num_features, dataDir);
	calc_feature.calculate();

	//Strong classifers as array of optimized features (opt_feat);
	int N = 4; //number of strong classifiers
	vector<opt_feat> SC[N]; //array of strong classifiers
	opt_feat empty_WC;
	int num_WC[N] = {1,5};
	vector<int> used_features[N]; //array of features used (index)
	eval eval_prev;

	// 				Training
	
	for(j=0;j<N;j++){ //thru strong classifiers
		cout << setw(80) << setfill('_') << "_" << endl;
		cout << endl << "Strong Classifier " << j << endl << endl; 
		initialize_weights(img);	
		
		//for(i=0; i<SC[j].size(); i++){ //thru weak classifiers	
		for(i=0; i<100; i++){ //weak classifiers
			normalize_weights(img);
			SC[j].push_back(empty_WC);
			// select best optimized feature
			SC[j][i] = feature_select(feature_ptrs, img, 
							num_features, used_features[j]);
			used_features[j].push_back(SC[j][i].index);
			stage(img, feature_ptrs, SC[j]);	
			update_weights(img, SC[j][i].error);
			eval eval_WC = eval_TPR_FPR(img); //find TPR and FPR
			
			cout << "Weak Classifer " << i << endl;
            cout << SC[j][i] << eval_WC << endl; //print structs
			if((eval_WC.TPR > .9) // stop conditions
				|| (eval_WC.TPR > .9 && (j==0))
				|| (eval_WC.TPR > .9 && (j==1))){
				//stage sucicessful
				break;
			}
			if(eval_WC.FPR == 0){
				//SC[j].resize(i);
				break;
			}
			
			eval_prev = eval_WC;
		}
		
		//test full stage & modify training dataset
		stage(img, feature_ptrs, SC[j]); //modifies img[].results
		
		eval eval_SC = eval_TPR_FPR(img);
		cout << eval_SC << endl;
		
		revise_dataset(img); //modifies img[].include_img
		if((eval_SC.num_nonfaces == 0) || (eval_SC.num_faces == 0)){
            //run out of images
			break;
        }
	}
	auto stop = chrono::high_resolution_clock::now();
	chrono::duration<double> runtime = stop - start;
	cout << "Time to train: " << runtime.count() << "s" << endl;
	

	// 		Validation
	cout << endl << setw(80) << setfill('*') << "*" << endl;
	cout << endl << "Validating..." << endl << endl;

	filesystem::path validationDir = "../Dataset/validate/txt/";
    vector<img_data> validation_imgs;
	
	calc_features calc_features2(feature_ptrs, validation_imgs, num_features, validationDir);
    calc_features2.calculate();

	//test trained classifiers with validation images
	for(j=0;j<N;j++){ //thru strong classifiers
		// run all images through stage
		stage(validation_imgs, feature_ptrs, SC[j]); //modifies validation_imgs[].result
		for(i=0; i<validation_imgs.size(); i++){ //thru images
			if(!validation_imgs[i].result){ //classified as nonface in last stage
				validation_imgs[i].include_img = false; //don't include in following stages
			}
		}
	}

	for(i=0; i<validation_imgs.size(); i++){
        validation_imgs[i].include_img = true;
    }

	eval final_eval = eval_TPR_FPR(validation_imgs); //find TPR, FPR
	cout << "Overall performance: " << endl;
	cout << final_eval << endl; //TPR, FPR, etc.

	// push to config files
	cout << endl << "Pushing to config files" << endl;
	for(j=0;j<N;j++){
		string filename = "config_" + to_string(j) + ".txt";
		cout << filename << endl;
		write_config(feature_ptrs, SC[j], filename);
	}

    // clean up
	cout << endl << "Deallocating memory..." << endl << endl;
    for(i=0;i<num_features; i++){
		delete feature_ptrs[i];
    }
    delete[] feature_ptrs;
}
