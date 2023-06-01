#ifndef _welter_example_graph_h
#define _welter_example_graph_h

/*******************************************************************************
@ddblock_begin copyright

Copyright (c) 1997-2023
Maryland DSPCAD Research Group, The University of Maryland at College Park 

Permission is hereby granted, without written agreement and without
license or royalty fees, to use, copy, modify, and distribute this
software and its documentation for any purpose, provided that the above
copyright notice and the following two paragraphs appear in all copies
of this software.

IN NO EVENT SHALL THE UNIVERSITY OF MARYLAND BE LIABLE TO ANY PARTY
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF
THE UNIVERSITY OF MARYLAND HAS BEEN ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.

THE UNIVERSITY OF MARYLAND SPECIFICALLY DISCLAIMS ANY WARRANTIES,
INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE
PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND THE UNIVERSITY OF
MARYLAND HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
ENHANCEMENTS, OR MODIFICATIONS.

@ddblock_end copyright
*******************************************************************************/

extern "C" {
#include "welt_c_basic.h"
#include "welt_c_fifo.h"
#include "welt_c_util.h"
}

#include "welt_cpp_actor.h"
#include "welt_cpp_graph.h"
#include "file_sink.h"
#include "classifier.h"
#include "ImgRead.h"
#include "IIS.h"

/* The capacity of all FIFOs in the graph. */
#define BUFFER_CAPACITY (1024)

/* An enumeration of the actors in this application. */
#define ACTOR_IMG (0)

/* An enumeration of the edges in this application. The naming convention
for the constants is FIFO_<source actor>_<sink actor>. */
#define FIFO_IMG  (0)

/* Graph class definition*/
class face_detector : public welt_cpp_graph{
public:

	
    face_detector(int num_class,
				  char* input_img_dir,
				  char* output);

    ~face_detector();

    /* Scheduler for this graph */
    void scheduler() override;

    /*Set the value of the Iters parameter.*/
    void setIters(int num_iter);

    /*Set the value of the Iters parameter.*/
    int getIters();

private:

	int count;
	char *img_dir;
	char *out_file;

	//Vector to hold all of the config files
	//for every strong classifier in the graph
	vector<string> cfg_files;

	int iterations;
	
};

#endif
