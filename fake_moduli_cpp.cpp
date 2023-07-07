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


void write_moduli_to_file(const std::string target_file, std::vector<mpz_class> moduli) {
	std::ofstream f_out;
	f_out.open(target_file);
	for (mpz_class z : moduli) {
		f_out << z;
	}
	f_out.close();
	return;
}

void generate_modulus(){
	mpz_t p1, q1, size, temp;
    bool p_isprime = false;
	mpz_inits(p1, q1, size, temp, 0);
	
	
	
	mpz_clears(p1,q1, size, temp, 0);
	return;
}

int main (int argc, char **argv) {
	// generate a bunch of fake RSA moduli for testing the product tree algorithm with
	// write them to a file
	std::string filename = "input.mpz";
	std::vector<mpz_class> test_moduli_vect;
	


}