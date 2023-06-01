//Test

#include <stdio.h>

//#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "data.h" //structs 
#include "calc_features.h" //functions for calculating features
#include "stage.h" //training functions
#include "validate.h"



using namespace std;
/*
struct img_data {
	int weight; //image weight
    bool face; //True = contains face
	bool correct;
    bool include_img; //True = include image in dataset
};

struct feature{ //per feature
	bool include_feature;
	char type; //A, B, C, D, or E
	// position
	vector<int> val; //feature value for each image
};
*/

void normalize_weights(vector<img_data>& images){
	
	float sum = 0;
	
	//Loop through each img in the vector of images
	for(img_data img : images){
		//calculate the sum of weights accross all images
		sum += img.weight;
	}
	
	//Loop through each img in the vector of images
	for(img_data img : images){
		//adjust each of the weights to normalize them
		img.weight = img.weight/sum;
	}
}

//updates the weights for the next iteration
void update_weights(vector<img_data>& images, float error){

	float beta = error / (1-error);
	
	//Loop through each img in the vector of images
	for(img_data img : images){
		
		//If the image is not classified correctly
		if(img.weight == false){
			
			//adjust the image weight for next iteration
			img.weight = img.weight*beta;
		}
	}
	
}

//Need to convert our image lists to a config.txt file
//Will be added here later

void write_config(feature** features, std::vector<opt_feat>& optimal_values, std::string& filename){
	
	std::ofstream output_file(filename);
	if (!output_file.is_open()) {
		std::cout << "Failed to open file\n";
		return;
	}
	
	//initialize a new string
	std::string new_string;
	
	//first write the number of features we have as the first thing in the config file
	output_file << optimal_values.size();
	output_file << "\n";
	
	for(int i=0;i<optimal_values.size();i++){
		//clear the contents of the string
		new_string.clear();
		
		
		
		new_string += features[optimal_values[i].index]->type;
		new_string += " ";
		
		new_string += features[optimal_values[i].index]->pos[0];
		new_string += " ";
		
		new_string += features[optimal_values[i].index]->pos[1];
		new_string += " ";
		
		new_string += features[optimal_values[i].index]->size[0];
		new_string += " ";
		
		new_string += features[optimal_values[i].index]->size[1];
		new_string += " ";
		
		new_string += std::to_string(optimal_values[i].parity);
		new_string += " ";
		
		new_string += std::to_string(optimal_values[i].thresh);
		new_string += "\n";
		
		output_file << new_string;
	}
	
	output_file.close();
}

int main(int argc, char *argv[])
{
    
	//input test cases here
	
	feature* features[] = {
    new feature{ 'A',
		{'1','2'},
		{'1','2'},
		NULL
		},
    new feature{ 'B',
		{'3','4'},
		{'4','3'},
		NULL
		}
	};
	
	std::vector<opt_feat> opt = {
    { NULL, 0, 2.0f , 3 },
    { NULL, 1, 2.0f , 3 }
	};
  
  std::string string = "file.txt";
  
  write_config(features,opt,string);
	
}



