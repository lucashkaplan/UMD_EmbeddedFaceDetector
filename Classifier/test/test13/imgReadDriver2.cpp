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

#include "ImgRead.h"

#define BUFFER_CAPACITY (1024)

using namespace std;

int main(int argc, char **argv) {
   	int arg_count = 2;
	
	// img number
	int k = 103;
	
	/* Check program usage. */
    if (argc != arg_count) {
        cerr << "imgReadTest.exe error: arg count" << endl;
        exit(1);
    }
	
	// open image dir
	std::filesystem::path imgDir(argv[1]);

	int token_size = sizeof(IIS);
	
	// fifos for img read actor
	welt_c_fifo_pointer fifo;
	
	token_size = sizeof(IIS);
	
	fifo = ((welt_c_fifo_pointer)welt_c_fifo_new(
            BUFFER_CAPACITY, token_size,
            0));

	auto actor = (new ImgRead(imgDir, fifo));

	actor->invoke();
	
	actor->showIIS();
	
	cerr.flush();
	
    return 0;
}
