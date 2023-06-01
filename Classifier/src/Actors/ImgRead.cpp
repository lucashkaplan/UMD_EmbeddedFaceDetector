/* Code to read in image and output Indexed Img. Subwindow (IIS)
input: img filename
output: IIS = {img[][], k = img index}
*/

#include "ImgRead.h"

using namespace std;

// constructor
ImgRead::ImgRead(filesystem::path inputImgDir, //filepath to dir. storing input imgs
		welt_c_fifo_pointer output_IIS) {
	
	imgDir = inputImgDir;
	this->IIS_out = output_IIS;
	
	// initialize img index
	k = 0;
	
	//mode = READ;
	reset();
	
	cout << "\ncreating img. read actor" << endl;
}

void ImgRead::invoke() {
	switch (mode) {
        case READ: {
			int i = 0, j = 0;
			
			// store index in IIS
			internal_IIS.k = k;
			
			for(auto const& iterator : filesystem::recursive_directory_iterator{imgDir}) {
				// iterate until desired img
				if(i <= k){
					imgFilepath = iterator.path();
					i++;
				}
				else{
					break;
				}
			}
			
			cout << "\nFinal imgFilepath: " << imgFilepath << endl;
			cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
			
			// write image index and face/non-face to file
			accuracyFile(imgFilepath);
						
			//// read in all numbers from input img
			//convert file path to string
			string imgFilepathStr = imgFilepath.generic_string();
			
			FILE *imgFilePtr = NULL;
			imgFilePtr = fopen(imgFilepathStr.c_str(), "r");
			
			if (imgFilePtr == NULL) {
				perror("fopen: ");
				exit(EXIT_FAILURE);
			}
			
			for(i = 0; i < 24; i++){
				for(j = 0; j < 24; j++){
					int data = 0;
					if (!fscanf(imgFilePtr, "%d,", &data)){
						perror("fscanf: ");
					}
					internal_IIS.img[i][j] = data;
				}
			}
			
			// write IIS to output
			welt_c_fifo_write(IIS_out, &internal_IIS);
			// increment image index
			k++;
			
            break;
		}
			
		default:
			break;
	}
}

bool ImgRead::enable() {
    bool result = false;
	
    switch (mode) {
		case READ:
			result = true; // existence of file checked w/in READ mode
		case ERROR:
			result = true;
			break;
        default:
            result = false;
            break;
    }
	
    return result;
}

void ImgRead::reset() {
    /* Close the file it is open, and then open/re-open the file so that
    subsequent invocations of the PROCESS MODE write from the beginning of 
    the file. 
    */
    if (outStream.is_open()) {
        outStream.close();
    }
    outStream.open(file_name);
    if (outStream.fail()) {
        cerr << "Could not open file \"" << file_name << "\"" << endl;
        mode = ERROR;
    }
	else {
		mode = READ;
	}
}

ImgRead::~ImgRead() {
    cout << "\ndeleting img. read actor" << endl;
}

void ImgRead::accuracyFile(filesystem::path imgFilepath) {
	bool face;
	
	string imgNameNoExt = filepath2name(imgFilepath);
	
	// labels corresponding to non-faces
	char nonFaceLabels[7] = {'1', '2', '5', 'e', 'o', 'r', 'U'};
	
	for(int i = 0; i < 7; i++){
		if(imgNameNoExt[1] == nonFaceLabels[i]){
			face = 0;
		}
	}
	if(imgNameNoExt[1] == 'a'){
		face = 1;
	}
	
	if(face){
		outStream << k << " F" << endl;
	}
	else{
		outStream << k << " NF" << endl;
	}
}


// obtaining string filename from path
string ImgRead::filepath2name(filesystem::path imgFilepath){
	string imgNameExt; //img name w/ extension
	
	string imgFilepathStr = imgFilepath.generic_string();; // convert path to string
	
	// create stringstream object that references imgFilepathStr
	stringstream imgFilepathSS(imgFilepathStr);
	
	//iterate through filepath until last substring separated by '/'
	while(getline(imgFilepathSS, imgNameExt, '/')){
	}
	
	// find '.' in string and include return name until '.'
	size_t lastIdx = imgNameExt.find(".");
	return imgNameExt.substr(0, lastIdx);
}

void ImgRead::connect(welt_cpp_graph* graph) {
	int port_index;
    int direction;

    /* output 1*/
    direction = GRAPH_OUT_CONN_DIRECTION;
    port_index = 0;
    graph->add_connection(this, port_index, direction);
}

void ImgRead::showIIS() {
	// outputting k
	cout << "\nk (img. label) = " << internal_IIS.k << endl << endl;
	
	// displaying img
	for(int i = 0; i < 24; i++){
		for(int j = 0; j < 24; j++){
			cout << "internal_IIS.img[" << i << "][" << j << "]: " << internal_IIS.img[i][j] << endl;
		}
	}
}
