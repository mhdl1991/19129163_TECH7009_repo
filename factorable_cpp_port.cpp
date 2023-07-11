// built this on Kali Linux and presumably Lubuntu using the command
// g++ -std=c++20 factorable_cpp_port.cpp -o -Wall fastgcd -lgmp -lgmpxx -pthread

// -- Efficient implementation of all-pairs GCD
// original program fastgcd.c by:
// NH,AH 2012/03

// C++ version by
// MK 2023


// Usage: 
//   fastgcd INPUT
//   (where INPUT is a file containing hex-encoded RSA moduli)
//

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include<cassert>


// for the file system library/header
#ifndef __has_include
	static_assert(false, "__has_include not supported");
#else
#if __has_include(<filesystem>)
#include <filesystem>
	namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
	namespace fs = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
	namespace fs = boost::filesystem;
#endif
#endif

#include<chrono>
#include<gmp.h>
#include<gmpxx.h>

// move all macros here
#define NTHREADS 4
#define INPUT_FN    "input.mpz"
#define OUTPUT_FN   "output.mpz"
#define NOW std::chrono::high_resolution_clock::now();
#define TIME_ELAPSED std::chrono::duration<double>(diff).count();
#define counter unsigned long


// test if file exists
bool file_exists(std::string filename) { return fs::exists(filename); }

// read hex strings from infile and write final count followed by gmp
// binary format values to output
void prep_hex_input(const std::string infile, const std::string outfile){
	std::ifstream in(infile);
	FILE* out;
	int count = 0;
	std::vector<mpz_class> temp_vec;
	std::string temp_string;
	
	auto start = NOW;
	std::cerr << std::fixed << std::setprecision(9) << std::left;
	std::cerr << "preprocessing input from " << infile << std::endl;
	
	// read strings from infile
	while( std::getline(in, temp_string) ) {
		temp_vec.push_back( mpz_class(temp_string, 16) );
		count++;
	}
	in.close();
	
	// write to outfile as binary values
	out = fopen (outfile.c_str(), "wb");
	fwrite(&count, sizeof(count), 1, out);
	for (mpz_class x: temp_vec) {
		__gmpz_out_raw(out, x.get_mpz_t());
	}
	fclose(out);
	
	auto end = NOW;
	auto diff = end - start;
	double dT = TIME_ELAPSED;
	std::cerr << "preprocessing " << count << " elements took " << dT << " s " << std::endl;
}




void prep_hex_input_OLD(const std::string infile, const std::string outfile) {
	FILE* in = fopen(infile.c_str(), "r");
	FILE* out = fopen (outfile.c_str(), "wb");
	int res, count = 0;
	mpz_class x;
	
	auto start = NOW;
	std::cerr << std::fixed << std::setprecision(9) << std::left;
	std::cerr << "preprocessing input from " << infile << std::endl;
	
	fwrite(&count, sizeof(count), 1, out); // ???
	for (;;) {
		res = gmp_fscanf(in, "%Zx", x.get_mpz_t());
		if (res == EOF) {break;}
		if (res != 1) {
			std::cerr << "invalid input" << std::endl;
			exit(1);
		}
		__gmpz_out_raw(out, x.get_mpz_t());
		count++;
	}
	fclose(in);
	rewind(out);
	fwrite(&count, sizeof(count), 1, out);
	fclose(out);
	
	auto end = NOW;
	auto diff = end - start;
	double dT = TIME_ELAPSED;
	std::cerr << "preprocessing " << count << " elements took " << dT << " s " << std::endl;
}

// initializes v and fills it with contents of named binary format file
std::vector<mpz_class> input_bin_array(const std::string filename) {
	auto start = NOW;
	
	std::cerr << std::fixed << std::setprecision(9) << std::left;
	std::cerr << "reading " << filename << " ..." << std::endl;
	FILE *in = fopen(filename.c_str(), "rb");
	assert(in);
	int count;
	int ret = fread(&count, sizeof(count), 1, in);
	
	assert(ret == 1);
	assert(count >= 0);
	
	std::vector<mpz_class> v;
	mpz_t temp;
	mpz_init(temp);
	
	size_t bytes;
	for (int i = 0; i < count; i++){
		bytes += __gmpz_inp_raw(temp, in);
		v.push_back( mpz_class(temp) );
	}

	mpz_clear(temp);
	auto end = NOW;
	auto diff = end - start;
	double dT = TIME_ELAPSED;
	
	std::cerr << v.size() << " elements, " << bytes << "bytes in " << dT << " s" << std::endl;
	
	fclose(in);
	return v;
}

// writes v to the named file in binary format
void output_bin_array(std::vector<mpz_class> v, const std::string filename) {
	auto start = NOW;
	std::cerr << std::fixed << std::setprecision(9) << std::left;
	
	std::cerr << "writing to " << filename << std::endl;
	
	FILE* out = fopen(filename.c_str(), "wb");
	assert(out);
	
	int count = v.size();
	fwrite(&count, sizeof(count), 1, out);
	size_t bytes = 0;
	for (mpz_class num : v) {
		bytes += __gmpz_out_raw(out, num.get_mpz_t());
	}	
	fclose(out);
	
	auto end = NOW;
	auto diff = end - start;
	double dT = TIME_ELAPSED;
	std::cerr << v.size() << " elements, " << bytes << " bytes in " << dT << " s" << std::endl;
}

// writes v to the named file as lines of hex strings
void output_hex_array(std::vector<mpz_class> v, const std::string filename) {
	std::cerr << "writing " << filename << "... " << std::endl;
	FILE* out = fopen(filename.c_str(), "w");
	assert(out);
	for (mpz_class num : v) {
		gmp_fprintf(out, "%Zx\n", num.get_mpz_t());
	}
	fclose(out);
	std::cerr << "ok!" << std::endl;
}


// sort and uniquify vector
void uniq (std::vector<mpz_class> v){
	std::sort(
		v.begin(), 
		v.end(), 
		[](const mpz_class a, const mpz_class b) { return mpz_cmp( a.get_mpz_t(), b.get_mpz_t() );}
	);
		
}

// executes func(n) over the range [start, end) using NTHREADS worker threads
// make sure func(n) is thread-safe
void iter_threads(int start, int end, std::function<void(int)> func) {
	int n = start;
	std::mutex m;
	auto thread_body = [&](void*) {
		for (;;) {
			std::lock_guard<std::mutex> lock(m);
			int i = n++;
			if (i >= end) {break;}
			func(i);
		}
	};
	std::vector<std::thread> threads;
	for (int j = 0; j < NTHREADS; j++) {threads.emplace_back(thread_body, nullptr);}
	for (std::thread &t : threads) {t.join();}
}

int product_tree() {
	std::vector<mpz_class> v = input_bin_array(INPUT_FN), w;
	std::string filename;
	int size, level = 0;
	double dT;
	auto tstart = std::chrono::high_resolution_clock::now();

	std::cerr << std::fixed << std::setprecision(9) << std::left;
	std::cerr << "multiplying numbers... "  << std::endl;	
	while (v.size() > 1) {
		auto start = NOW;
		std::cerr << "level " << level << std::endl;
		size = (v.size() + 1) / 2;
		w.resize(size);
		auto mul_job = [&](int k){
			w[k] = v[2*k] * v[(2*k)+1];		
		};
		iter_threads(0, v.size()/2, mul_job);
		if (v.size() & 1) { w[v.size()/2] = v[v.size()-1]; }
		
		filename = "p" + std::to_string(level) + ".mpz";
		output_bin_array(w, filename);
		v = w;
		level++;

		auto end = NOW;
		auto diff = end - start;
		dT = TIME_ELAPSED;
		std::cerr << dT << " s" << std::endl;
	}

	auto end = NOW;
	auto diff = end - tstart;
	dT = TIME_ELAPSED;

	std::cerr << "the product tree took " << dT << " seconds" << std::endl;
	return level;
}

void remainder_tree(int level){
	std::vector<mpz_class> P, v = input_bin_array(INPUT_FN), w;
	//int size = 0;
	double dT;
	auto tstart = std::chrono::high_resolution_clock::now();
	std::string filename = "p" + std::to_string(level) + ".mpz";
	mpz_class s;

	std::cerr << std::fixed << std::setprecision(9) << std::left;
	std::cerr << "computing remainder tree... "  << std::endl;	
	P = input_bin_array(filename);

	while (level > 0) {
		auto start = NOW;
		std::cerr << "level " << level << std::endl;
		level--;
		filename = "p" + std::to_string(level) + ".mpz";
		v = input_bin_array(filename);
		
		auto mul_job =[&](int k){ 
			s = v[k] * v[k];
			v[k] = P[k/2] % s;
		};
		iter_threads(0, v.size(), mul_job);
		#ifdef OUTPUT_REMAINDER_LEVELS
		filename = "r" + std::to_string(level) + ".mpz";
		output_bin_array(v, name);
		#endif
		P = v;
		auto end = NOW;
		auto diff = end - start;
		dT = TIME_ELAPSED;
		std::cerr << dT << " s" << std::endl;
	}
	// final round
	auto start = NOW;
	std::cerr << "output" << std::endl;
	v = input_bin_array(INPUT_FN);
	
	auto muldiv_job = [&](int k) {
		s = v[k] * v[k];
		w[k] = P[k / 2] % s;
		w[k] /= v[k];
		mpz_gcd(w[k].get_mpz_t(), w[k].get_mpz_t(), v[k].get_mpz_t());
	};
	iter_threads(0, v.size(), muldiv_job);
	output_bin_array(w, OUTPUT_FN);
	auto end = NOW;
	auto diff = end - start;
	dT = std::chrono::duration<double>(diff).count();
	std::cerr << dT << " s" << std::endl;

	end = std::chrono::high_resolution_clock::now();
	diff = end - tstart;
	dT = TIME_ELAPSED;
	std::cerr << "Remainder tree took " << dT << " s" << std::endl;
}

void emit_results(){
	std::vector<mpz_class> moduli = input_bin_array(INPUT_FN);
	std::vector<mpz_class> gcds = input_bin_array(OUTPUT_FN);
	double dT;
	auto start = NOW;

	std::cerr << "emitting results" << std::endl;


	int size = 0;
	for (counter i = 0; i < gcds.size(); i++) {
		if (gcds[i] == 1) {
			moduli[i] = moduli[size];
			gcds[size] = gcds[i];
			size++;
		}	
	}	
	for (counter i = size; i < gcds.size(); i++) {
		mpz_clear( moduli[i].get_mpz_t() );
		mpz_clear( gcds[i].get_mpz_t() );
	}
	output_hex_array(moduli, "weak_moduli");
	output_hex_array(gcds, "gcds");

	auto end = NOW;
	auto diff = end - start;
	dT = TIME_ELAPSED;
	std::cerr << "emitting " << size << " results took " << dT << " seconds" << std::endl;
		
}




int main (int argc, char** argv) {
	double dT;
	auto start = NOW;

	if (argc != 2) {
		std::cerr << "usage: " << argv[0] << " INPUT | --resume" << std::endl;
		exit(1);
	}

	if (strcmp(argv[1], "--resume")) {
		std::string input(argv[1]);
		remove(INPUT_FN);
		prep_hex_input(input, "input.tmp");
		if (rename("input.tmp", INPUT_FN)) {
			std::cerr << "can't rename input temp file" << std::endl;
			exit(1);
		}
	} else if (!file_exists(INPUT_FN)) {
		std::cerr << "unable to resume" << std::endl;
		exit(1);
	}
	int level = product_tree();
	remainder_tree(level-1);
	emit_results();
	auto end = NOW;
	auto diff = end - start;
	dT = TIME_ELAPSED;
	std::cerr << "run took " << dT << " s" << std::endl;
	return 0;
}
