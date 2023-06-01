#ifndef ImgRead_h
#define ImgRead_h

/*******************************************************************************
DESCRIPTION:
This is a header file of the ImgRead actor.
*******************************************************************************/
extern "C" {
#include "welt_c_basic.h"
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}

#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "IIS.h"

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

/*******************************************************************************
Actor Modes:
*******************************************************************************/

#define READ 0
#define ERROR 1

class ImgRead : public welt_cpp_actor{
public:
    ImgRead(filesystem::path inputImgFilepath,
			welt_c_fifo_pointer output_IIS);
			
    ~ImgRead() override;
	
	string filepath2name(filesystem::path imgFilepath);
    
	bool enable() override;
    void invoke() override;
    void reset() override;
	void connect(welt_cpp_graph* graph);
	void showIIS();
	void accuracyFile(filesystem::path imgFilepath);

private:
	filesystem::path imgDir; // dir to input img
	filesystem::path imgFilepath; // filepath of img considering
	string imgNameNoExt = ""; // name of input img w/o extension
	IIS internal_IIS; 
	welt_c_fifo_pointer IIS_out;
	int k; // img index
	ofstream outStream; // outstream to file to check accuracy
	string file_nameStr = "imgLabels.txt";
	const char* file_name = file_nameStr.c_str();
};

#endif
