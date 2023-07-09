// build this on Kali Linux and presumably Lubuntu using the command
// g++ -std=c++20 factorable_cpp_port.cpp -o factorable_cpp_port.o -lgmp -lgmpxx -pthread
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

#define NTHREADS 4

#define INPUT_FN    "input.mpz"
#define OUTPUT_FN   "output.mpz"


// test if file exists
bool file_exists(std::string filename) { return fs::exists(filename); }

// read hex strings from infile and write final count followed by gmp
// binary format values to output
void prep_hex_input(const std::string infile, const std::string outfile) {
	FILE* in = fopen(infile.c_str(), "r");
	FILE* out = fopen (outfile.c_str(), "wb");
	int res, count = 0;
	mpz_class x;
	
	auto start = std::chrono::high_resolution_clock::now();
	
	std::cout << std::fixed << std::setprecision(9) << std::left;
	std::cout << "preprocessing input from " << infile << std::endl;
	
	fwrite(&count, sizeof(count), 1, out); // ???
	for (;;) {
		int res = gmp_fscanf(in, "%Zx", x.get_mpz_t());
		if (res == EOF) {break;}
		if (res != 1) {
			std::cout << "invalid input" << std::endl;
			exit(1);
		}
		__gmpz_out_raw(out, x.get_mpz_t());
		count++;
	}
	fclose(in);
	rewind(out);
	fwrite(&count, sizeof(count), 1, out);
	fclose(out);
	
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	double dT = std::chrono::duration<double>(diff).count();


	std::cout << "preprocessing " << count << " elements took " << dT << " s " << std::endl;}

// initializes v and fills it with contents of named binary format file
std::vector<mpz_class> input_bin_array(const std::string filename) {
	auto start = std::chrono::high_resolution_clock::now();
	
	std::cout << std::fixed << std::setprecision(9) << std::left;
	std::cout << "reading " << filename << " ..." << std::endl;
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
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	double dT = std::chrono::duration<double>(diff).count();
	
	std::cout << v.size() << " elements, " << bytes << "bytes in " << dT << " s" << std::endl;
	
	fclose(in);
	return v;
}

// writes v to the named file in binary format
void output_bin_array(std::vector<mpz_class> v, const std::string filename) {
	auto start = std::chrono::high_resolution_clock::now();
	std::cout << std::fixed << std::setprecision(9) << std::left;
	
	std::cout << "writing to " << filename << std::endl;
	
	FILE* out = fopen(filename.c_str(), "wb");
	assert(out);
	
	int count = v.size();
	fwrite(&count, sizeof(count), 1, out);
	size_t bytes = 0;
	for (mpz_class num : v) {
		bytes += __gmpz_out_raw(out, num.get_mpz_t());
	}	
	fclose(out);
	
	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - start;
	double dT = std::chrono::duration<double>(diff).count();
	std::cout << v.size() << " elements, " << bytes << " bytes in " << dT << " s" << std::endl;
}

// writes v to the named file as lines of hex strings
void output_hex_array(std::vector<mpz_class> v, const std::string filename) {
	std::cout << "writing " << filename << "... " << std::endl;
	FILE* out = fopen(filename.c_str(), "w");
	assert(out);
	for (mpz_class num : v) {
		gmp_fprintf(out, "%Zx\n", num.get_mpz_t());
	}
	fclose(out);
	std::cout << "ok!" << std::endl;
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
	int size = 0, level = 0;
	double dT;
	auto tstart = std::chrono::high_resolution_clock::now();

	std::cout << std::fixed << std::setprecision(9) << std::left;
	std::cout << "multiplying numbers... "  << std::endl;	
	while (v.size() > 1) {
		auto start = std::chrono::high_resolution_clock::now();
		std::cout << "level " << level << std::endl;
		size = (v.size() + 1) / 2;
		auto mul_job = [&](int k){
			w[k] = v[2*k] * v[(2*k)+1];		
		}
		iter_threads(0, v.size()/2, mul_job);
		if (v.size() & 1) { w[v.size()/2] = v[v.size()-1]; }
		
		filename = "p" + std::to_string(level) + ".mpz";
		output_bin_array(w, filename);
		v = w;
		level++;

		auto end = std::chrono::high_resolution_clock::now();
		auto diff = end - start;
		dT = std::chrono::duration<double>(diff).count();
		std::cout << dT << " s" << std::endl;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto diff = end - tstart;
	dT = std::chrono::duration<double>(diff).count();

	std::cout << "the product tree took " << dT << " seconds" << std::endl;
	return level;
}




int main (int argc, char** argv) {
	// test...
	if (file_exists(INPUT_FN)) {
		prep_hex_input(INPUT_FN, "input.tmp");
		std::vector<mpz_class> v = input_bin_array("input.tmp");
	}
	
	return 0;
}
