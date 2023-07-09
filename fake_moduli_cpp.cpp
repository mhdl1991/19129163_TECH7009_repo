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

#define MODULUS_BSIZE 1024


int main (int argc, char **argv) {
	// initialize the RNG and seed it
	gmp_randclass mt(gmp_randinit_mt);
	mt.seed( (unsigned int)time(NULL) );
	
	// GMP library variables
	mpz_class p, q, n;
	bool p_isprime = false, q_isprime = false;
	
	// generate a bunch of fake RSA moduli 
	// for testing the product tree algorithm with
	// write them to a file
	std::string filename = "input.mpz";
	std::vector<mpz_class> test_moduli_vect;
	int i = 0;
	while (test_moduli_vect.size() != 7) {
		std::cout << "creating Test moduli #" << i << std::endl;
		for (;;) {
			// create two random numbers
			// check if they are both probable primes.
			p = mt.get_z_bits(MODULUS_BSIZE);
			q = mt.get_z_bits(MODULUS_BSIZE);
			p_isprime = mpz_probab_prime_p(p.get_mpz_t(), 10); 	
			q_isprime = mpz_probab_prime_p(q.get_mpz_t(), 10);
			if (p_isprime && q_isprime) {break;} 	
		}
		// put the moduli in a file
		std::cout << "created Test moduli #" << i << std::endl;
		n = p * q;
		test_moduli_vect.push_back(n); 
		i++;
	}
	
	// create an additional modulus with a shared factor
	std::cout << "creating Test moduli #" << i << std::endl;
	for (;;) {
		q = mt.get_z_bits(MODULUS_BSIZE);
		q_isprime = mpz_probab_prime_p(q.get_mpz_t(), 10);
		if (q_isprime) {break;}
	}
	std::cout << "created Test moduli #" << i << std::endl;
	n = p * q;
	test_moduli_vect.push_back( n );
	
	std::cout << "test vector now has " << test_moduli_vect.size() << " moduli" << std::endl;
	std::cout << "writing Test moduli to " << filename << std::endl;
	
	// write test moduli to file as hex strings.
	FILE* f = fopen(filename.c_str(), "w+");
	for (mpz_class num : test_moduli_vect) {
		gmp_fprintf(f, "%Zx", num.get_mpz_t() );
		fprintf(f,"\n");
	}
	fclose(f);	
	return 0;
}
