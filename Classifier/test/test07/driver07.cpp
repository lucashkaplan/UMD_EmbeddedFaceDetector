#include <stdio.h>
#include <iostream>
#include <ctime>
#include <queue>
#include <string>
#include <chrono>
extern "C" {
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}
#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "face_detector.h"

using namespace std;

int main(int argc, char **argv) {
	//section 1 input
    int count;
    char *img_dir;
    char *out_file;

    int arg_count = 4;

    /* Check program usage. */
    if (argc != arg_count) {
        cerr << "error: arg count" << endl;
        exit(1);
    }

    /* Open the input and output file(s). */
    int i = 1;
    count=atoi(argv[i++]);
    img_dir=argv[i++];
    out_file=argv[i++];

    // starting timepoint
    auto start = chrono::high_resolution_clock::now();

    auto* graph = new face_detector(count,
                                    img_dir,
                                    out_file);
	
	// iterate through graph 3 times
	int iterations = 3;
	graph->setIters(iterations);
	
	graph->scheduler();

	cerr.flush();
	
	// Get ending timepoint
    auto stop = chrono::high_resolution_clock::now();
	
	// get duration and cast to mins
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	
	cout << "\nRuntime: " << duration.count() << " us" << endl;
	
    return 0;
}
