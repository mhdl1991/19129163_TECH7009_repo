#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include<assert>
#include<filesystem>

#include<gmp.h>
#include<gmpxx.h>

#define NTHREADS 4
#define vec std::vector

#define INPUT_FN    "input.mpz"
#define OUTPUT_FN   "output.mpz"

// test if file exists
bool file_exists(std::string filename)
{
	return std::filesystem::exists(filename);
}

// read hex strings from infile and write final count followed by gmp
// binary format values to output
void prep_hex_input(std::string infile, std::string outfile) {
	std::ifstream in(infile, std::ifstream::in);
	std::ofstream out(outfile, std::ofstream::out);
	int res, count = 0;
	
	out << count;

	mpz_t x;
	mpz_init(x);
	while(in.good()){
		res = gmp_fscanf(in, "%Zx", x);
		if (res == EOF) { break; }
		if (res != 1) {
			fprintf(stderr, "invalid input\n");
			exit(1);
		}
		__mpz_out_raw(out, x);
		count++;
	}
	mpz_clear(x);
	
	out.rewind();
	out << count;
	
	in.close();
	out.close();
}

std::vector<mpz_class> input_bin_array(const std::string filename) {
	std::ifstream in(filename, std::ifstream::in);
	
}


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
	VEC<std::thread> threads;
	for (int j = 0; j < NTHREADS; j++) {threads.emplace_back(thread_body, nullptr);}
	for (std::thread &t : threads) {t.join();}
}

int main (int argc, char** argv) {

}