#ifndef classifier_h
#define classifier_h

/*******************************************************************************
DESCRIPTION:
This is a header file of the classifier actor.
*******************************************************************************/
extern "C" {
#include "welt_c_util.h"
#include "welt_c_fifo.h"
}

#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "IIS.h"
#include <cmath>
#include <fstream>

/*******************************************************************************
Actor Modes:
*******************************************************************************/

#define CLASS_CONFIG   0
#define CLASS_READ     1
#define CLASS_CLASSIFY 3
#define CLASS_TRUE     4
#define CLASS_FALSE    5
#define CLASS_ERROR	   6

/*
 * Feature Types
 */

#define FEAT_A 0
#define FEAT_B 1
#define FEAT_C 2
#define FEAT_D 3
#define FEAT_E 4

class classifier : public welt_cpp_actor{
public:
    classifier(char* config_file,
			   welt_c_fifo_pointer in_IIS,
			   welt_c_fifo_pointer out_abort,
			   welt_c_fifo_pointer out_continue);
    ~classifier() override;

    bool enable() override;
    void invoke() override;
	void connect(welt_cpp_graph* graph);

	void set_diagnostic(bool d);

    void reset() override;

	void print_weak_classifiers();
    void print_curr_int_img();
	void print_IIS();
	int get_feat_val();
	float get_C();

private:
	char* cfg_file;
    welt_c_fifo_pointer IIS_in;
	welt_c_fifo_pointer abort_out;
	welt_c_fifo_pointer continue_out;

	//variable to hold information on current image
	IIS indexed_image;
	int (*img)[24];
	int int_img[24][24];
	int k;

	//variables on actor ID and diagnostic mode
	int class_num;
	bool diagnostic;

	int num;
	int feat_val;
	float C=0;
	//float alpha, beta;
	float total_thresh=0;
	//Struct which holds all necessary information for a weak
	//classifier
	typedef struct weak_class {
		int type;
    	int p;
    	float thresh;
		float alpha;
    	int width;
    	int height;
    	int x;
		int y;
	} feature;
	//vector of all weak classifiers
	vector<feature> weak_classifiers;

	void calc_int_img();

	int calc_rect(int x, int y, int width, int height);
	
	int calc_feat_A(int anchor_x, int anchor_y, int width, int height);
	int calc_feat_B(int anchor_x, int anchor_y, int width, int height);
	int calc_feat_C(int anchor_x, int anchor_y, int width, int height);
	int calc_feat_D(int anchor_x, int anchor_y, int width, int height);
	int calc_feat_E(int anchor_x, int anchor_y, int width, int height);
	
};

#endif
