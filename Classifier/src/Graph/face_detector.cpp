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

#include <iostream>
#include "face_detector.h"

using namespace std;
//Constructor. Graph needs the number of strong classifiers,
//The input image directory, and the output file name
face_detector::face_detector
        (int num_class,
		char* input_img_dir,
		char* output
		) {
	
	count = num_class;
	// might need to convert char * -> filepath
	img_dir = input_img_dir;
	
	out_file = output;

    /*Set default number of graph iterations*/
    iterations = 1;

    /* the number of actors in this graph */
    actor_count = count + 2; //every classifier plus img read and file sink

    /* the number of fifos in this graph */
    fifo_count = count + 2; //fifos between classifiers/sink ( count ) + abort + img read output

    /* Initialize fifos. */
    int token_size;
    token_size = sizeof(IIS);
    /* Reserve spaces for fifos in the graph*/
    fifos.reserve(fifo_count);

    /* create new fifos and put them into the graph class */
	//0th fifo is the img output. last fifo is the abort
	for(int i=0; i<fifo_count; i++){
		fifos[i] = 
			((welt_c_fifo_pointer)welt_c_fifo_new(
			BUFFER_CAPACITY, token_size,
			i));
	}

    /***************************************************************************
    Create actors in the actors vector and put descriptions
    for each actor in the descriptions vector.
    ***************************************************************************/

    actors.reserve(actor_count);
    descriptors.reserve(actor_count);

    //0th actor is img read
	actors[ACTOR_IMG]=(new ImgRead(
			img_dir,
            fifos[FIFO_IMG]));
    descriptors[ACTOR_IMG] =
            ((char*)"txt img read");

	cfg_files.resize(count);
	//Create each strong classifier
	//input fifo is the previous fifo. output is the current fifo.
	//abort fifo is the last fifo
	for(int i=1; i<=count; i++){
		//build the config file name
		cfg_files[i-1] = "config";
		cfg_files[i-1].append(to_string(i));
		cfg_files[i-1].append(".txt");
	
		actors[i]=(new classifier(
			(char *)cfg_files[i-1].c_str(),
			fifos[i-1],
			fifos[fifo_count-1],
			fifos[i]));
		//build the description
		string desc("strong classifier ");
		descriptors[i] = 
			((char *)desc.append(to_string(i)).c_str());
	}
	//creat the file sink actor
	actors[count + 1]=(new file_sink(
			fifos[fifo_count-1],
			fifos[count],
			out_file));
    descriptors[count + 1] =
            ((char*)"file sink");


}
//Scheduler
//This runs the full cascade for each image in the input directory.
void face_detector::scheduler() {
    int i;
    int iter;

	//Configure each classifier
	for(i=1; i<= count; i++){
		if(actors[i]->enable()){
    		actors[i]->invoke();
			cout << "class " << i << " configed" << endl;
  		}
	}

	//Classify each image
    for (iter=0; iter<getIters(); iter++) {
		
		//Read in the image
		if(actors[ACTOR_IMG]->enable()){
			actors[ACTOR_IMG]->invoke();
			cout << "img read" << endl;
		}

		//Activate each strong classifier in the cascad3
		for(i=1; i<=count; i++){
			//Read the new image
			if(actors[i]->enable()){
				actors[i]->invoke();
			} else {
				break; //if a strong classifier it ever not enabled
					   //it must be due to no IIS token being passed to i
					   //This means we can jump out of the cascade as the image
					   //does not contain a face
			}

			//Classify
			if(actors[i]->enable()){
        		actors[i]->invoke();
    		}

			//Write the image
			if(actors[i]->enable()){
            	actors[i]->invoke();
        	}
		}

		//Write output from strong classifier cascade
		if(actors[count+1]->enable()){
			actors[count+1]->invoke();
		}
		
    }

    for (i=0; i<actor_count; i++) {
        delete actors[i];
    }

	cerr.flush();
}

void face_detector::setIters(int num_iter) {
    iterations = num_iter;
}
int face_detector::getIters() {
    return iterations;
}

/* destructor */
face_detector::~face_detector() {
}
