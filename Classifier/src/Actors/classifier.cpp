#include <iostream>

#include "classifier.h"

using namespace std;

// constructor
classifier::classifier(char* config_file,
		welt_c_fifo_pointer input_IIS,
        welt_c_fifo_pointer out_abort,
		welt_c_fifo_pointer out_continue) {

	this->cfg_file = config_file;
	this->IIS_in = input_IIS;
	this->abort_out = out_abort;
	this->continue_out = out_continue;

	mode = CLASS_CONFIG;
}

// enable function
bool classifier::enable() {
    bool result = false;

    switch (mode) {
        case CLASS_CONFIG:
			result = true;
            break;
		case CLASS_READ://We can read if there is a token on the input IIS FIFO
			result = (welt_c_fifo_population(IIS_in) >=1);
			break;
		case CLASS_CLASSIFY://Always enabled to calssify
			result = true;
			break;
		case CLASS_TRUE: //Can only write if FIFO is not at capacity
			result = (welt_c_fifo_population(continue_out) < welt_c_fifo_capacity(continue_out));
			break;
		case CLASS_FALSE:
			result = (welt_c_fifo_population(abort_out) < welt_c_fifo_capacity(abort_out));
			break;
		case CLASS_ERROR:
			result = true;
        default:
            result = false;
            break;
    }
    return result;
}

void classifier::invoke() {

	switch (mode) {
		//Reads in weak classifiers from the config file
        case CLASS_CONFIG: {
			ifstream cfgstream(cfg_file);
			if(!cfgstream.is_open()){
				mode = CLASS_ERROR;
				cerr << "Could not read config file" << endl;
				exit(0);
				break;
			}
			cfgstream >> this->class_num;
			cfgstream >> this->num;
			weak_classifiers.resize(num);
			for(int i=0; i<num; i++){
				cfgstream >> weak_classifiers[i].type;
				cfgstream >> weak_classifiers[i].x;
				cfgstream >> weak_classifiers[i].y;
				cfgstream >> weak_classifiers[i].width;
				cfgstream >> weak_classifiers[i].height;
				cfgstream >> weak_classifiers[i].p;
				cfgstream >> weak_classifiers[i].thresh;
				cfgstream >> weak_classifiers[i].alpha;
			}
			mode = CLASS_READ;
		}
        break;
		//Read the IIS token from the input FIFO
        case CLASS_READ:{
			welt_c_fifo_read(IIS_in, &indexed_image);
			this->img = indexed_image.img;
			this->k = indexed_image.k;

			mode = CLASS_CLASSIFY;
		}
        break;
        case CLASS_CLASSIFY:{
			//Calc integral image nd reset C and total thresh for each image
			calc_int_img();
			C=0;
			total_thresh=0;

			//Iterate over each weak classifier
			for(int i =0; i<num; i++){
				//Calculate appropriate feature values
				switch(weak_classifiers[i].type){
					case FEAT_A: {
						feat_val=calc_feat_A(weak_classifiers[i].x,
								 weak_classifiers[i].y,
								 weak_classifiers[i].width,
								 weak_classifiers[i].height);
						break;
					}
					case FEAT_B:{
						feat_val=calc_feat_B(weak_classifiers[i].x,
                                 weak_classifiers[i].y,
                                 weak_classifiers[i].width,
                                 weak_classifiers[i].height);
						break;
					}
					case FEAT_C:{
						feat_val=calc_feat_C(weak_classifiers[i].x,
                                 weak_classifiers[i].y,
                                 weak_classifiers[i].width,
                                 weak_classifiers[i].height);
						break;
					}
					case FEAT_D:{
						feat_val=calc_feat_D(weak_classifiers[i].x,
                                 weak_classifiers[i].y,
                                 weak_classifiers[i].width,
                                 weak_classifiers[i].height);
						break;
					}
					case FEAT_E:{
						feat_val=calc_feat_E(weak_classifiers[i].x,
                                 weak_classifiers[i].y,
                                 weak_classifiers[i].width,
                                 weak_classifiers[i].height);
						break;
					}
				}
				//Apply parity and check threshold. Add to total threshold and C
				feat_val=feat_val*weak_classifiers[i].p;
				//beta = weak_classifiers[i].e / (1-weak_classifiers[i].e);
                //alpha=std::log10(1/beta);
				if(feat_val < weak_classifiers[i].p*weak_classifiers[i].thresh){
					C+=weak_classifiers[i].alpha;
				}
				total_thresh+=weak_classifiers[i].alpha;
			}
			//Check final value of C to determine if there was a face
			if(C >= total_thresh/2){
				mode = CLASS_TRUE;
			}else{
				mode = CLASS_FALSE;
			}

		}
        break;
        case CLASS_TRUE:{
			//pass image along to next classifier
			welt_c_fifo_write(continue_out, &indexed_image);

			mode=CLASS_READ;
		}
        break;
        case CLASS_FALSE:{
			//Reject the face
			welt_c_fifo_write(abort_out, &indexed_image);

			mode=CLASS_READ;
		}
        break;
        case CLASS_ERROR:{
			mode = CLASS_ERROR;
		}
		break;
    }

}
//Returns the current feat val that was calcuated
int classifier::get_feat_val(){
	return this->feat_val;
}
//Returns the current C that was calculated
float classifier::get_C(){
	return this->C;
}
//Calculated the integral image by going row by row.
//It adds previously calculated entries of the
//integral image to allow us to calculate it in a 
//single pass.
//Rather than 0 padding, if statements are used
//to check if we go out of the bounds of the image.
void classifier::calc_int_img(){
	for(int i=0; i<24; i++){
		for(int j=0; j<24; j++){
			int_img[i][j] = img[i][j];
			if(i!=0 && j != 0){
				int_img[i][j]-=int_img[i-1][j-1];
			}

			if(i!=0){
				int_img[i][j] += int_img[i-1][j];
			}

			if(j!=0){
				int_img[i][j]+= int_img[i][j-1];
			}
		}
	}
}
//This function takes the top left coordinate of a box and its
//height and width. It then return the sum of all pixel values
//in the box.
int classifier::calc_rect(int x, int y, int width, int height){

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

}
//Calculate the value of type A feature
int classifier::calc_feat_A(int anchor_x, int anchor_y, int width, int height){
	
	if(height % 2 != 0){
		cerr << "Invalid height for type A feature" << endl;
		exit(0); //What to do here?
	}

	int h = height/2;
	int top, bottom;

	top = calc_rect(anchor_x, anchor_y, width, h);
	bottom = calc_rect(anchor_x, anchor_y + h, width, h);


	if(this->diagnostic){
		cout << "Top: " << top << " Bot: " << bottom << endl;
	}

	return bottom - top;

}
//Calculate value of type B feature
int classifier::calc_feat_B(int anchor_x, int anchor_y, int width, int height){

	if(width % 2 != 0){
		cerr << "Invalid width for type B feature" << endl;
		exit(0); //What to do here?
	}

	int w = width/2;
	int left, right;

	left = calc_rect(anchor_x, anchor_y, w, height);
	right = calc_rect(anchor_x + w, anchor_y, w, height);

	if(this->diagnostic){
		cout << "Left: " << left << " Right: " << right << endl;
	}

	return left - right;
}
//Calculate value of type C feature
int classifier::calc_feat_C(int anchor_x, int anchor_y, int width, int height){

	if(height % 3 != 0){
		cerr << "Invalid height for type C feature" << endl;
		exit(0); //What to do here?
	}

	int h = height/3;
	int top, mid,  bot;

	top = calc_rect(anchor_x, anchor_y, width,h);
	mid = calc_rect(anchor_x, anchor_y+h, width,h);
	bot = calc_rect(anchor_x, anchor_y+2*h, width,h);

	if(this->diagnostic){
    	cout << "Top: " << top << " Mid: " << mid << " Bot: " << bot << endl;
	}


	return mid - top - bot;

}
//Calculate value of type D feature
int classifier::calc_feat_D(int anchor_x, int anchor_y, int width, int height){

	if(width % 3 != 0){
		cerr << "Invalid width for type D feature" << endl;
		exit(0); //What to do here?
	}

	int w = width/3;
	int left, mid, right;

	left = calc_rect(anchor_x, anchor_y, w, height);
	mid = calc_rect(anchor_x+w, anchor_y, w, height);
	right = calc_rect(anchor_x+2*w, anchor_y, w, height);

	if(this->diagnostic){
    	cout << "Left: " << left << " Mid: " << mid << " Right: " << right << endl;
	}


	return mid - left - right;
}
//Calculate value of type E feature
int classifier::calc_feat_E(int anchor_x, int anchor_y, int width, int height){

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

	if(this->diagnostic){
		cout << "TopL: " << topl << " TopR: " << topr;
		cout << " BotL: " << botl << " BotR: " << botr << endl;
	}

	return topl + botr - topr - botl;
}
//Prints all of the current weak classifiers read form the
//config file
void classifier::print_weak_classifiers(){

	if(num == 0){
		return;
	}

	for(auto it : this->weak_classifiers){
		cout << it.type << " ";
    	cout << it.x << " ";
		cout << it.y << " ";
    	cout << it.width << " ";
		cout << it.height << " ";
		cout << it.p << " ";
		cout << it.thresh << " ";
		cout << it.alpha << endl;
	}

}
//Prints the current integral image
void classifier::print_curr_int_img(){
	for(int i=0; i<24; i++){
		for(int j=0; j<24; j++){
			printf("%3d,", int_img[i][j]);
		}
		cout << endl;
	}
}
//Prinf the current IIS
void classifier::print_IIS(){
	cout << "Index: " << this->k << endl;
	for(int i=0; i<24; i++){
		for(int j=0;j<24;j++){
			printf("%3d,", this->img[i][j]);
		}
		cout << endl;
	}

}
//Resets the classifier
void classifier::reset() {
    mode = CLASS_CONFIG;
}

void classifier::connect(welt_cpp_graph* graph) {
	int port_index;
    int direction;

    /* output 1*/
    direction = GRAPH_OUT_CONN_DIRECTION;
    port_index = 0;
    graph->add_connection(this, port_index, direction);
    //cout << "in_image" << endl;
}
//Sets the diagnostic variable for more verbose output
void classifier::set_diagnostic(bool d){
	this->diagnostic = d;
}

classifier::~classifier() {
    cout << "Deleting Strong Classifier" << endl;
}

