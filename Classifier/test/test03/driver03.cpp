#include <stdio.h>
#include <iostream>
#include <ctime>
#include <queue>
#include <string>
extern "C" {
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "classifier.h"
#include "IIS.h"

using namespace std;

int main(int argc, char **argv) {
   	//section 1 input
    char *cfg_file;

	int arg_count = 2;

    /* Check program usage. */
    if (argc != arg_count) {
        cerr << "demo_hist_gen.exe error: arg count" << endl;
        exit(1);
    }

    /* Open the input and output file(s). */
    int i = 1;
	cfg_file=argv[1];

	int token_size = sizeof(IIS);
	IIS iis;
	for(i=0;i<24;i++){
		for(int j=0;j<24;j++){
			iis.img[i][j]=1;
		}
	}
	iis.k=1;

	welt_c_fifo_pointer fifos[3];
	fifos[0] = ((welt_c_fifo_pointer)welt_c_fifo_new(
            1024, token_size,
            0));

	fifos[1] = ((welt_c_fifo_pointer)welt_c_fifo_new(
            1024, token_size,
            1));

	fifos[2] = ((welt_c_fifo_pointer)welt_c_fifo_new(
            1024, token_size,
            2));

	welt_c_fifo_write(fifos[0], &iis);

	auto actor =(new classifier(
			cfg_file,
			fifos[0], fifos[1],
			fifos[2]));

	actor->invoke();

	actor->invoke();

	actor->invoke();

	actor->print_curr_int_img();

	cerr.flush();
	
    return 0;
}
