
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "data.hpp"
#include <chrono>


// variable for time measure
std::chrono::system_clock::time_point start, end;

// dataset identifier
unsigned int dataset_id = 0;

// sampling rate
float sampling_rate = 1;

// cutoff distance
float cutoff = 0;
float local_density_min = 0;

// parameter for cluster center
float delta_min = 0;

// system parameter and variables
float coord_max = 100000;
unsigned int core_no = 1;
float sampling_rate_cell = 1.0;

// result
double cpu_offline = 0;
double cpu_grid_mapping = 0;
double cpu_grid_partition = 0;
double cpu_local_density = 0;
double cpu_dependency = 0;
double cpu_label = 0;
double rand_index = 0;
std::vector<unsigned int> cluster_centers;
std::unordered_map<unsigned int, unsigned int> label_set;


// parameter input
void input_parameter() {

	std::ifstream ifs_cutoff("../_parameter/cutoff.txt");
	std::ifstream ifs_dataset_id("../_parameter/dataset_id.txt");
	std::ifstream ifs_core_no("../_parameter/thread_num.txt");
	std::ifstream ifs_sampling_rate("../_parameter/sampling_rate.txt");
	std::ifstream ifs_sampling_rate_cell("../_parameter/sampling_rate_cell.txt");

	if (ifs_cutoff.fail()) {
		std::cout << " cutoff.txt does not exist." << std::endl;
		std::exit(0);
	}
	else if (ifs_dataset_id.fail()) {
		std::cout << " dataset_id.txt does not exist." << std::endl;
		std::exit(0);
	}
	else if (ifs_core_no.fail()) {
		std::cout << " thread_numo.txt does not exist." << std::endl;
		std::exit(0);
	}
	else if (ifs_sampling_rate.fail()) {
		std::cout << " sampling_rate.txt does not exist." << std::endl;
		std::exit(0);
	}
	else if (ifs_sampling_rate_cell.fail()) {
		std::cout << " sampling_rate_cell.txt does not exist." << std::endl;
		std::exit(0);
	}

	while (!ifs_cutoff.eof()) { ifs_cutoff >> cutoff; }
	while (!ifs_dataset_id.eof()) { ifs_dataset_id >> dataset_id; }
	while (!ifs_core_no.eof()) { ifs_core_no >> core_no; }
	while (!ifs_sampling_rate.eof()) { ifs_sampling_rate >> sampling_rate; }
	while (!ifs_sampling_rate_cell.eof()) { ifs_sampling_rate_cell >> sampling_rate_cell; }

	// determine parameters
	if (dataset_id == 0) {

		delta_min = 10;
		local_density_min = 1000;
	}
	std::cout<<delta_min<<" "<<local_density_min<<" "<<cutoff<<std::endl;
}

// compute directory
std::string directory_comp() {

	if (dataset_id == 0) return "0-synthetic";

	return "";
}

// data input
void input_data() {

	// point variable
	point p;
	pt p_;

	// id variable
	unsigned int id = 0;

	// position & id variables
	std::vector<float> d(dimensionality);
	std::vector<float> d_max(dimensionality);
	std::vector<float> d_min(dimensionality);
	for (unsigned int i = 0; i < dimensionality; ++i) d_min[i] = 1000000;

	// sample probablility
	std::mt19937 mt(1);
	std::uniform_real_distribution<> rnd(0, 1.0);

	// dataset for visual check
	if (dataset_id == 0) {

		// position & id variables
		float x = 0, y = 0;

		std::string f_name;
		if (dataset_id == 0) f_name = "../_dataset/synthetic.txt";
		
		// file input
		std::ifstream ifs_file(f_name);

		// error check
		if (ifs_file.fail()) {
			std::cout << " data file does not exist." << std::endl;
			std::exit(0);
		}

		// file read
		while (!ifs_file.eof()) {

			// input
			ifs_file >> x >> y;

			p[0] = x;
			p[1] = y;

			if (x > d_max[0]) d_max[0] = x;
			if (x < d_min[0]) d_min[0] = x;
			if (y > d_max[1]) d_max[1] = y;
			if (y < d_min[1]) d_min[1] = y;

			// update pt
			p_.update_id(id);
			p_.update_val(p);

			// insert into dataset
			dataset_pt.push_back(p_);

			// increment identifier
			++id;
		}
	}

	// normalization
	bool flag = 0;
	if (dataset_id == 0) flag = 1;
	if (flag) {

		for (unsigned int i = 0; i < dimensionality; ++i) d_max[i] -= d_min[i];

		for (unsigned int i = 0; i < dataset_pt.size(); ++i) {
			for (unsigned int j = 0; j < dimensionality; ++j) {
				dataset_pt[i][j] -= d_min[j];
				dataset_pt[i][j] /= d_max[j];
				dataset_pt[i][j] *= coord_max;
			}
		}
	}

	// reserve memory
	rnn.reserve(dataset_pt.size());
}

// label input
void input_label() {

	std::string str = "../Ex-DPC/result/" + directory_comp() + "/Label/label_id(" + std::to_string(dataset_id) + ")_sampling_rate(" + std::to_string(sampling_rate) + ")_cutoff(" + std::to_string(cutoff) + ").txt";
	std::ifstream ifs_label(str);

	// error check
	if (ifs_label.fail()) {
		std::cout << " label file does not exist." << std::endl;
		std::exit(0);
	}

	unsigned int id = 0;
	unsigned int label = 0;

	// file read
	while (!ifs_label.eof()) {

		// input
		ifs_label >> id >> label;

		// store
		label_set[id] = label;
	}
}

// output label with coordinate
void output_coord_label() {

	// partition by label
	std::unordered_map<unsigned int, std::vector<unsigned int>> temp_set;
	for (unsigned int i = 0; i < dataset_pt.size(); ++i) temp_set[dataset_pt[i].label].push_back(i);

	for (unsigned int i = 0; i < cluster_centers.size(); ++i) {

		auto it = temp_set.find(dataset_pt[cluster_centers[i]].id);

		std::string f_name = "result/" + directory_comp() + "/Label/s-approx-label-" + std::to_string(it->first) + "_id(" + std::to_string(dataset_id) + ")_cutoff(" + std::to_string(cutoff) + ").txt";
		std::ofstream file;
		file.open(f_name.c_str(), std::ios::out | std::ios::app);

		if (file.fail()) {
			std::cerr << " cannot open the output file." << std::endl;
			file.clear();
			return;
		}

		for (unsigned int j = 0; j < it->second.size(); ++j) {

			for (unsigned int k = 0; k < dimensionality; ++k) file << dataset_pt[it->second[j]][k] << "\t";
			file << it->first << "\n";
		}

		file.close();
	}
}

// output computation time
void output_cpu_time() {

	std::string f_name = "result/" + directory_comp() + "/cpu_time_data-id(" + std::to_string(dataset_id) + ")_sampling_rate(" + std::to_string(sampling_rate) + ")_cutoff(" + std::to_string(cutoff) + ")_c-sampling_rate(" + std::to_string(sampling_rate_cell) + ")_core_no(" + std::to_string(core_no) + ")_s-approx.csv";
	std::ofstream file;
	file.open(f_name.c_str(), std::ios::out | std::ios::app);

	if (file.fail()) {
		std::cerr << " cannot open the output file." << std::endl;
		file.clear();
		return;
	}

	file << 
		"Grid mapping time [microsec]" << "," << 
		"Local density comp. time [microsec]" << "," << 
		"Dependensy comp. time [microsec]" << "," << 
		"Label propagation time [microsec]" << "," << 
		"Total time [microsec]" << "," << 
		"Rand index" << "," << 
		"Number of clusters" << "\n";
	file << 
		cpu_grid_mapping << "," << 
		cpu_grid_partition + cpu_local_density << "," << 
		cpu_dependency << "," << 
		cpu_label << "," << 
		cpu_grid_mapping + cpu_dependency + cpu_local_density + cpu_label << "," << 
		rand_index << "," << 
		cluster_centers.size() << "\n\n";

	file.close();
}

// rand index computation
void compute_rand_index() {

	// set sampling ratio
	const float ratio = 0.01;

	// prepare vector for samples
	std::vector<pt*> dataset_pt_sample;

	// random value variable
	std::mt19937 mt(1);
	std::uniform_real_distribution<> rnd(0, 1);

	// get samples
	for (unsigned int i = 0; i < dataset_pt.size(); ++i) {

		if (rnd(mt) <= ratio) dataset_pt_sample.push_back(&dataset_pt[i]);
	}

	double cnt = 0;
	double cardinality = (double)dataset_pt_sample.size();
	cardinality *= (double)(dataset_pt_sample.size() - 1.0);
	cardinality /= 2.0;

	// compute rand index
	#pragma omp parallel num_threads(40)
	{
		#pragma omp for schedule(dynamic) reduction(+:cnt)
		for (int i = 0; i < dataset_pt_sample.size(); ++i) {

			for (unsigned int j = i + 1; j < dataset_pt_sample.size(); ++j) {

				// get ground truth
				if (label_set[dataset_pt_sample[i]->id] == label_set[dataset_pt_sample[j]->id]) {

					if (dataset_pt_sample[i]->label == dataset_pt_sample[j]->label) ++cnt;
				}
				else {

					if (dataset_pt_sample[i]->label != dataset_pt_sample[j]->label) ++cnt;
				}
			}
		}
	}

	rand_index = cnt / cardinality;
	std::cout << " rand index: " << rand_index << "\n";
}

// output decision graph
void output_decision_graph() {

	std::string f_name = "result/" + directory_comp() + "/Decision-graph/data-id(" + std::to_string(dataset_id) + ")_cutoff(" + std::to_string(cutoff) + ").csv";
	std::ofstream file;
	file.open(f_name.c_str(), std::ios::out | std::ios::app);

	if (file.fail()) {
		std::cerr << " cannot open the output file." << std::endl;
		file.clear();
		return;
	}

	for (unsigned int i = 0; i < dataset_pt.size(); ++i) {
		if (dataset_pt[i].local_density >= local_density_min) file << dataset_pt[i].id << "," << dataset_pt[i].local_density << "," << dataset_pt[i].dependent_dist << "\n";
	}

	file.close();
}
