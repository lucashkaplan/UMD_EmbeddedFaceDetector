#include "stage.h"


// initialize image weights
void initialize_weights(vector<img_data>& img){
	float pos = 0; //positive sample counts 
	float neg = 0; //negative sample counts
	int i;
	
	for(i=0;i<img.size();i++){
		if (img[i].face && img[i].include_img){
			pos++;
		}
		else if(!img[i].face && img[i].include_img){
			neg++;
		}
	}
	//initialize
	for(i=0;i<img.size();i++){
		if (img[i].face && img[i].include_img){
			img[i].weight = 1/(2*pos);
		}
		else if (!img[i].face && img[i].include_img){
			img[i].weight = 1/(2*neg);
		}
		else{ //don't include image
            img[i].weight = 0;
        }
			
	}	
	/*
	for(i=0;i<img.size();i++){
		printf("img.weight:%f  img.face:%d \n", img[i].weight, img[i].face);
	}
	*/
}

// normalize training image weights
void normalize_weights(vector<img_data>& img){
	float sum = 0;
	int i;

	for(i=0;i<img.size();i++){ //through images
		if(img[i].include_img){
			sum += img[i].weight;
		}
	}
	
	for(i=0;i<img.size();i++){ //through images
		if(img[i].include_img){
			img[i].weight = img[i].weight/sum; //
		}
	}
}


//updates the training image weights for training next weak classifier
void update_weights(vector<img_data>& img, float error){
	float beta = error/(1-error);
	int i;
	//cout << error << endl;
	for(i=0;i<img.size();i++){ //through images
		if((img[i].face && img[i].result) || (!img[i].face && !img[i].result)){ 
			//adjust the image weight for next iteration
			img[i].weight = img[i].weight*beta;
		}
		
	}
}

// Revise training set between stages
void revise_dataset(vector<img_data> &img){
   	for(int i=0;i<img.size();i++){ //through images
    	if(!img[i].include_img){
            continue;
        }

		if (img[i].result && img[i].face){ //true positive
        	img[i].include_img = true;
    	}
    	else if (img[i].result && !img[i].face){//false positive
        	img[i].include_img = true;
    	}
		else { //true negative or false negative
        	img[i].include_img = false;
		}
	}
}

//simulate single stage
void stage(vector<img_data> &img,
           	 feature **feature_ptrs,
             const vector<opt_feat>& SC){
	
	float C1, C2;
	int i, j, n;
	//cout << img.size() << endl;	
	for(i=0;i<img.size();i++){ //through images
        if(img[i].include_img == false){
            continue;
        }
        C1 = 0;
        C2 = 0;
	
		for(j=0;j<SC.size();j++){ //through all weak classifiers in stage
    		
			n = SC[j].index;
			if(n > 162336 || n < 0){
				cerr << "stage error: index out of range" << endl;
			}
	
        	if(SC[j].parity*feature_ptrs[n]->val[i]
                                < SC[j].parity*SC[j].thresh){
        	// passes this weak classifier
        		C1 += SC[j].alpha;
        	}
        	C2 += SC[j].alpha;
    	}

    	if(C1 >= .5*C2){
        	img[i].result = true; //image is classified as face
			//cout << "true" << endl;
    	}
    	else{
        	img[i].result = false;
			//cout << "false" << endl;
    	}
	}
}


eval eval_TPR_FPR(const vector<img_data> &img){
	int i;
	int tp_count = 0;
    int fp_count = 0;
    int pos_count = 0;
    int neg_count = 0;
	
	for(i=0;i<img.size();i++){ //through images
		if(!img[i].include_img){
            continue;
        }

		if (img[i].result && img[i].face){ //true positive
            tp_count++;
        }
        else if (img[i].result && (!img[i].face)){//false positive
            fp_count++;
        }
		
		// overall count of positive and negative samples
        if(img[i].face){
            pos_count++;
        }
        else{
            neg_count++;
        }
	}
	struct eval TPR_FPR = {};
	//cout << tp_count << endl;
	//cout << pos_count << endl;
	//cout << fp_count << endl;
	//cout << neg_count << endl;

	TPR_FPR.TPR = float(tp_count)/pos_count;
    TPR_FPR.FPR = float(fp_count)/neg_count;
	TPR_FPR.num_faces = pos_count;
	TPR_FPR.num_nonfaces = neg_count;
	return TPR_FPR;
}



// for use in feature_select for sorting
bool compareVals(const img_data &img1, const img_data &img2){
    //used for sorting in feature_select
    return (img1.val < img2.val);
}


// optimize threshold and parity of all features,
// select feature that minimizes error across all training samples
opt_feat feature_select(feature **feature_ptrs, vector<img_data>& img, 
				const int num_features, const vector<int>& used_features){ 
	
	opt_feat opt_feature; //final optimized feature to be returned
	int i,j,n;
	float overall_min_error = .5;
	img_data empty_img;
	
	float tot_pos, tot_neg; //total weight of pos/neg samples in training data
	float pos, neg;	//running sum of weight of pos/neg samples 
	float feat_error; //of current feature
    int feat_parity; //of current feature
    float opt_error, opt_thresh, opt_parity; // optimized feature characteristics
	float s1, s2;

	for(i=0;i<num_features;i++){ //through features
		NEXT:
		//check if feature has been used
		for(j=0;j<used_features.size();j++){
			if(i == used_features[j]){ 
				i++;
				goto NEXT;
			}
		}
		
        
		vector<img_data> sorted_img; //for reordering training images
		tot_pos = 0; //summed weight of all positive samples
        tot_neg = 0; //summed weight of all negative samples
		int n = 0;

		for(j=0;j<img.size();j++){ //through images
			if(img[j].include_img == false){
				continue;
			}
			// copy image data and associated feature value to new sorted_img
			sorted_img.push_back(empty_img);
			sorted_img[n].val = feature_ptrs[i]->val[j];
			sorted_img[n].face = img[j].face;
			sorted_img[n].weight = img[j].weight;

			if(sorted_img[n].face){
                tot_pos += sorted_img[n].weight;
            }
            else{
                tot_neg += sorted_img[n].weight;
            }
			n++;
		}
		//sort training samples by feature value
		sort(sorted_img.begin(), sorted_img.end(), compareVals); 
		
		
		pos = 0; //summed weight of previous positive samples
        neg = 0; //summed weight of previous negative samples
        opt_error = .5; //minimum error of current feature
		
		for(j=0;j<sorted_img.size();j++){ //through sorted training samples
            if(sorted_img[j].face){
				pos += sorted_img[j].weight;
        	}
			else{
				neg += sorted_img[j].weight;			
			}
			//calculate error for current threshold
			s1 = pos + tot_neg - neg;
			s2 = neg + tot_pos - pos;
			//printf("s1:%f  s2:%f", s1, s2);
			if(s1 < s2){
				feat_error = s1;
				feat_parity = -1;
			}
			else{
				feat_error = s2;
				feat_parity = 1;
			}
			//printf("feat_error:%f opt_error:%f \n\n", feat_error, opt_error);
			if(feat_error < opt_error){ 
				//if lowest error out of all feature vals checked so far
				//reassign minimum error for this feature
				opt_error = feat_error; 
				opt_parity = feat_parity;

				//threshold splits current and previous examples feature value
				if(j>0){
					opt_thresh = (float(sorted_img[j-1].val)+sorted_img[j].val)/2;
				}
				else{
					opt_thresh = float(sorted_img[j].val);
				}
				//opt_thresh = float(sorted_img[j].val);
			}
		}
		
		if(opt_error < overall_min_error){ 
			//feature has lowest error so far
			
			overall_min_error = opt_error;
			// update optimal feature parameters -- for output
			opt_feature.index = i;
			opt_feature.parity = opt_parity;
			opt_feature.thresh = opt_thresh;
			opt_feature.error = opt_error;
			opt_feature.alpha = log((1-opt_error)/opt_error);
		}
	}
	
	// check accuracy of optimized feature for all images
	n = opt_feature.index;

	return opt_feature;
}







