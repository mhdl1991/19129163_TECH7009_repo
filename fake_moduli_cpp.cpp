#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include <gmp.h>
#include <gmpxx.h>


void save_moduli_to_file(std::string filename, const mpz_t mod) {
	FILE *fp;
	fp = fopen(filename.c_str(),"w");
	if (fp) {	
		mpz_out_str(, 16, mod);
	} else {
		std::cout << "Uh oh, something went wrong." << std::endl
	}
	fclose(fp);
	return;
}

void generate_modulus(){
	return;
}

int main (int argc, char **argv) {
	// generate a bunch of fake RSA moduli for testing the product tree algorithm with
	std::vector<std::string> filenames = {"test0","test1","test2","test3","test4","test5","test6","test7"};
	


}