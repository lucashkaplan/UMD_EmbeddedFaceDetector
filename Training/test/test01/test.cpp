

//Test

#include <stdio.h>

//#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../../cpp/data.h" //structs 
#include "../../cpp/calc_features.h" //functions for calculating features
#include "../../cpp/stage.h" //training functions
#include "../../cpp/validate.h"



/*
struct img_data {
    float weight; //image weight
    int val;
    bool face; //True = contains face
    bool include_img; //True = include image in dataset
};

struct feature{ //per feature
    char type; //A, B, C, D, or E
    char size[2]; //size of feature
    char pos[2]; // position of feature (top left)
    std::vector<int> val; //feature value for each image
};

struct opt_feat{
    float error;
    int index;
    float thresh;
    int parity; //-1 or 1
	float alpha;
};
*/

//This test will test the input and output of weight initialization

int main(int argc, char *argv[])
{
    
	//Create sample struct img_data
	
	vector<img_data> images = {{1.5f, 2, true,true},
	{1.2f, 3, true,true}};

	feature features[2] = {
    {'A', {'1', '2'}, {'3', '4'}, {1, 2}},
    {'B', {'5', '6'}, {'7', '8'}, {3, 4}}};
	
	
	
	//Code to output images vector
	std::ofstream outfile(argv[1]);

    // Iterate over the elements of the featuresay and write each element to the file
    for (int i = 0; i < 2; i++) {
        outfile << images[i].weight << " "
                << images[i].val << " "
                << images[i].face <<  " "
                << images[i].include_img << "\n";
    }

    // Close the output file stream
    outfile.close();
	
	
	//code to output features
	/*
	// Open the output file stream
    std::ofstream outfile(argv[1]);

    // Iterate over the elements of the featuresay and write each element to the file
    for (int i = 0; i < 2; i++) {
        outfile << features[i].type << " "
                << features[i].size[0] << features[i].size[1] << " "
                << features[i].pos[0] << features[i].pos[1] << " "
                << features[i].val[0] << " " << features[i].val[1] << "\n";
    }

    // Close the output file stream
    outfile.close();
	*/
}
