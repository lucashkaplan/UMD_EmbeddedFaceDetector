#include "calc_features.h"


void calc_features::calculate(){
		
	int width, height; //dimensions of feature
    int x, y; //top left corner of feature
	bool first_file = true;
	int feat;

	img.resize(0);
	img_data empty_img;
	int n = 0; //image counter

	//iterate through files
    for(auto const& iterator : filesystem::recursive_directory_iterator{data_dir}		){
		
		filename = iterator.path().string();
		
		img.push_back(empty_img);
		img[n].include_img = true;
		
		// classify as face given filename
		if(filename.find("face") != string::npos) {
            img[n].face = true;
        }
        else {
            img[n].face = false;
        }
		
        int k=0; //feature index

		calc_integral_img(); //fills int_img[][]

		for(width=1; width<=nCols; width++){
			for(height=2; height<=nRows; height+=2){
				//move feature around image
				for(y=0; y <= (nRows-height); y++){
           			for(x=0; x <= (nCols-width); x++){
						if(first_file){
                            feature_ptrs[k]->type = 'A';
                            feature_ptrs[k]->size[0] = height;
                            feature_ptrs[k]->size[1] = width;
							feature_ptrs[k]->pos[0] = y; 
							feature_ptrs[k]->pos[1] = x;
							feature_ptrs[k]->val.resize(0);
						}
						feat = calc_feat_A(x, y, width, height);
						feature_ptrs[k]->val.push_back(feat);
						k++;
					}
				}
			}
		}
		// type B
		for(width=2; width<=nCols; width+=2){
            for(height=1; height<=nRows; height++){
                for(y=0; y <= (nRows-height); y++){
                    for(x=0; x <= (nCols-width); x++){
                        if(first_file){
                            feature_ptrs[k]->type = 'B';
                            feature_ptrs[k]->size[0] = height;
                            feature_ptrs[k]->size[1] = width;
                            feature_ptrs[k]->pos[0] = y;
                            feature_ptrs[k]->pos[1] = x;
							feature_ptrs[k]->val.resize(0);				
                        }
						feat = calc_feat_B(x, y, width, height);
						feature_ptrs[k]->val.push_back(feat);
						k++;
					}
				}
			}
		}
		// type C
        for(width=1; width<=nCols; width++){
            for(height=3; height<=nRows; height+=3){
                for(y=0; y <= (nRows-height); y++){
                    for(x=0; x <= (nCols-width); x++){
                        if(first_file){
							feature_ptrs[k]->type = 'C';
                    		feature_ptrs[k]->size[0] = height;
                    		feature_ptrs[k]->size[1] = width;
                            feature_ptrs[k]->pos[0] = y;
                            feature_ptrs[k]->pos[1] = x;
							feature_ptrs[k]->val.resize(0);
                        }
                        feat = calc_feat_C(x, y, width, height);
                    	feature_ptrs[k]->val.push_back(feat);
						k++;
					}
                }
            }
        }
		// type D
        for(width=3; width<=nCols; width+=3){
            for(height=1; height<=nRows; height++){
                for(y=0; y <= (nRows-height); y++){
                    for(x=0; x <= (nCols-width); x++){
                        if(first_file){
                            feature_ptrs[k]->type = 'D';
                            feature_ptrs[k]->size[0] = height;
                            feature_ptrs[k]->size[1] = width;
                            feature_ptrs[k]->pos[0] = y;
                            feature_ptrs[k]->pos[1] = x;
							feature_ptrs[k]->val.resize(0);
                        }
                        feat = calc_feat_D(x, y, width, height);
                    	feature_ptrs[k]->val.push_back(feat);
						k++;
					}
                }
            }
        }
		// type E
        for(width=2; width<=nCols; width+=2){
            for(height=2; height<=nRows; height+=2){
                for(y=0; y <= (nRows-height); y++){
                    for(x=0; x <= (nCols-width); x++){
                        if(first_file){
                            feature_ptrs[k]->type = 'E';
                            feature_ptrs[k]->size[0] = height;
                            feature_ptrs[k]->size[1] = width;
                            feature_ptrs[k]->pos[0] = y;
                            feature_ptrs[k]->pos[1] = x;
							feature_ptrs[k]->val.resize(0);
                        }
                        feat = calc_feat_E(x, y, width, height);
                    	feature_ptrs[k]->val.push_back(feat);
						k++;
					}
                }
            }
        }
		n++;
		first_file = false;
	}
}

void calc_features::calc_integral_img(){

    ifstream ifs(filename, ifstream::in);
    if (ifs.fail()){
        cerr << "calculate_features.exe error: images not found.";
        cerr << endl;
        return;
    }

    int x,y;
    int i[nRows][nCols]; //store original image
    int s[nRows][nCols]; //row sum
    string temp_data;
    string line;

    for(x=0; x<nRows; x++){
        getline(ifs,line);
        stringstream ss(line);
        for(y=0; y<nCols; y++) {
            //string substr;
            getline(ss, temp_data, ',');
            i[x][y] = stoi(temp_data);

            if(x==0 && y==0){ //top left corner
                s[x][y] = i[x][y];
                int_img[x][y] = s[x][y];
            }
            else if(x==0){ //top edge
                s[x][y] = s[x][y-1] + i[x][y];
                int_img[x][y] = s[x][y];
            }
            else if(y==0){ //left edge
                s[x][y] = i[x][y];
                int_img[x][y] = int_img[x-1][y] + s[x][y];
            }
            else{
                s[x][y] = s[x][y-1] + i[x][y];
                int_img[x][y] = int_img[x-1][y] + s[x][y];
            }
            //cout << int_img[x][y] << " ";
        }
		//cout << endl;
    }
	//cout << endl;
    ifs.close();
};


int calc_features::calc_rect(int x, int y, int width, int height){

	int A=0,B=0,C=0,D=0;
	int Ax = x-1;
	int Ay = y-1;

	int Bx = Ax + width;
	int By = Ay;

	int Cx = Ax;
	int Cy = Ay + height;

	int Dx = Bx;
	int Dy = Cy;

	if(Ay >= 0 && Ax >=0){
		A = this->int_img[Ay][Ax];
	}

	if(By >= 0 && Bx >=0){
        B = this->int_img[By][Bx];
    }

	if(Cy >= 0 && Cx >=0){
        C = this->int_img[Cy][Cx];
    }

	if(Dy >= 0 && Dx >=0){
        D = this->int_img[Dy][Dx];
    }

	return D - C - B + A;
};

int calc_features::calc_feat_A(int anchor_x, int anchor_y, int width, int height){
	if(height % 2 != 0){
		cerr << "Invalid height for type A feature" << endl;
		exit(0); //What to do here?
	}

	int h = height/2;
	int top, bottom;

	top = calc_rect(anchor_x, anchor_y, width, h);
	bottom = calc_rect(anchor_x, anchor_y + h, width, h);

	return bottom - top;
};

int calc_features::calc_feat_B(int anchor_x, int anchor_y, int width, int height){

	if(width % 2 != 0){
		cerr << "Invalid width for type B feature" << endl;
		exit(0); //What to do here?
	}

	int w = width/2;
	int left, right;

	left = calc_rect(anchor_x, anchor_y, w, height);
	right = calc_rect(anchor_x + w, anchor_y, w, height);

	return left - right;
};

int calc_features::calc_feat_C(int anchor_x, int anchor_y, int width, int height){

	if(height % 3 != 0){
		cerr << "Invalid height for type C feature" << endl;
		exit(0); //What to do here?
	}

	int h = height/3;
	int top, mid,  bot;

	top = calc_rect(anchor_x, anchor_y, width,h);
	mid = calc_rect(anchor_x, anchor_y+h, width,h);
	bot = calc_rect(anchor_x, anchor_y+2*h, width,h);

	return mid - top - bot;
};

int calc_features::calc_feat_D(int anchor_x, int anchor_y, int width, int height){

	if(width % 3 != 0){
		cerr << "Invalid width for type D feature" << endl;
		exit(0); //What to do here?
	}

	int w = width/3;
	int left, mid, right;

	left = calc_rect(anchor_x, anchor_y, w, height);
	mid = calc_rect(anchor_x+w, anchor_y, w, height);
	right = calc_rect(anchor_x+2*w, anchor_y, w, height);

	return mid - left - right;
};

int calc_features::calc_feat_E(int anchor_x, int anchor_y, int width, int height){

	if(width % 2 != 0){
		cerr << "Invalid width for type E feature" << endl;
		exit(0); //What to do here?
	} else if(height % 2 != 0){
		cerr << "Invalid height for type E feature" << endl;
		exit(0);
	}

	int h = height/2;
	int w = width/2;
	int topr, topl, botl, botr;

	topl = calc_rect(anchor_x, anchor_y, w, h);
	topr = calc_rect(anchor_x+w, anchor_y, w, h);
	botl = calc_rect(anchor_x, anchor_y+h, w, h);
	botr = calc_rect(anchor_x+w, anchor_y+h, w, h);

	return topl + botr - topr - botl;
};

