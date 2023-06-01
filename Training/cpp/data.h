#ifndef data_h
#define data_h

#include <vector>
#include <chrono>

struct img_data {
    float weight; //image weight
    int val;
    bool face; //True = contains face
	bool result;
    bool include_img; //True = include image in dataset
};

struct feature{ //per feature
    char type; //A, B, C, D, or E
    char size[2]; //size of feature
    char pos[2]; // position of feature (top left)
    std::vector<int> val; //feature value for each training image	
};

struct eval{
    float TPR = 0;
    float FPR = 0;
	float avg_num_WC = 0;
	int num_faces = 0;
	int num_nonfaces = 0;
	std::chrono::duration<double> runtime = std::chrono::seconds(0);
	
};

struct opt_feat{
    float error;
    int index;
    float thresh;
    int parity; //-1 or 1
	float alpha;
};


#endif

