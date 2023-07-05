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

// using the vec_t struct defined in the original instead of trying to use a C++ vector
// because even when using C++ you can't use GMP's mpz_t with vectors, you have to use classes
typedef struct vec_ {
  mpz_t *el;
  int count; 
} vec_t;
// init vector v to contain count mpzs
void init_vec(vec_t *v, int count) {
	assert(v);
	v->count = count;
	v->el = malloc(count * sizeof(mpz_t));
	assert(v->el);
	for (int i=0; i < v->count; i++) { mpz_init(v->el[i]); }
}
// free the vector v
void free_vec(vec_t *v) {  
	assert(v);
	for (int i=0; i < v->count; i++) { mpz_clear(v->el[i]);}
	free(v->el);
}

// test if file exists
bool file_exists(const std::string name)
{
	return std::filesystem::exists(name);
}

// read hex strings from infile and write final count followed by gmp
// binary format values to output
void prep_hex_input(const std::string infile, const std::string outfile)
{
  double start = now();
  //fprintf(stderr, "preprocessing input from %s\n", infile);
  std::cerr << "preprocessing input from " << infile << std::endl;

  //FILE *in = fopen(infile, "r");
  //assert(in);
  //FILE *out = fopen(outfile, "wb");
  //assert(out);
  
  std::ifstream in(infile);
  std::ofstream out(outfile);
  

  int count=0;
  fwrite(&count, sizeof(count), 1, out);
  mpz_t x;
  mpz_init(x);
  for (;;) {
    int res = gmp_fscanf(in, "%Zx", x);
    if (res == EOF)
      break;
    if (res != 1) {
      std::cerr << "Invalid input" << std::endl; 
      exit(1);
    }
    __mpz_out_raw(out, x);
    count++;
  }
  fclose(in);
  rewind(out);
  fwrite(&count, sizeof(count), 1, out);
  fclose(out);
}









void iter_threads(int start, int end, void (*func)(int n) ) {
	// execute func(n) over the range [start, end] using NTHREADS worker threads
	// need to make sure func() is thread-safe
	
	int n = start;
	std::mutex mtx;
	
	auto thread_body = [&](void*){
		for(;;){
			std::lock_guard<std::mutex> lock(mtx);
			int i = n++;
			if (i >= end) {break;}
			func(i);
		}
		return nullptr;
	}
	vec<std::thread> threads;
	for (int j = 0; j < NTHREADS; j++){
		threads.emplace_back(thread_body, nullptr);
	}
	for (std::thread& thread : threads){ thread.join(); }
}

int main (int argc, char** argv) {

}