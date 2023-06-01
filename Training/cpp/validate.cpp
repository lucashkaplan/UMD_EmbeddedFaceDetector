#include "data.h"
#include "validate.h"
#include "calc_features.h"
#include "stage.h"


void write_config(feature** features, vector<opt_feat>& optimal_values, string filename){
	
	ofstream output_file(filename);
	if (!output_file.is_open()) {
		std::cout << "Failed to open file\n";
		return;
	}
	
	//initialize a new string
	string new_string;

	//first write the number of features we have as the first thing in the config file
	output_file << optimal_values.size();
	output_file << "\n";
	
	for(int i=0;i<optimal_values.size();i++){ //through imgs
		//clear the contents of the string
		new_string.clear();
		int j = optimal_values[i].index;

		new_string += features[j]->type;
		new_string += " ";
		
		new_string += to_string(features[j]->pos[1]);
		new_string += " ";
		
		new_string += to_string(features[j]->pos[0]);
		new_string += " ";
		
		new_string += to_string(features[j]->size[1]);
		new_string += " ";
		
		new_string += to_string(features[j]->size[0]);
		new_string += " ";
		
		new_string += to_string(optimal_values[i].parity);
		new_string += " ";
		
		new_string += to_string(optimal_values[i].thresh);
		new_string += " ";
		
		new_string += to_string(optimal_values[i].alpha);
		new_string += "\n";
		
		output_file << new_string;
	}
	
	output_file.close();
}

