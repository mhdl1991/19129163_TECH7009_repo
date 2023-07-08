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
	//gmp_randstate_t mt;
	//gmp_randinit_mt(mt);
    //gmp_randseed_ui(mt,(unsigned int)time(NULL) );
	gmp_randclass mt(gmp_randinit_mt);
	mt.seed( (unsigned int)time(NULL) );
	
													// GMP library variables
	mpz_t temp, p, q, n;
	mpz_inits(temp, p, q, n, 0);
	
	mpz_class p_, q_, n_;
	
	
	bool p_isprime = false, q_isprime = false;
													// generate a bunch of fake RSA moduli 
													// for testing the product tree algorithm with
													// write them to a file
	std::string filename = "input.mpz";
	std::vector<mpz_class> test_moduli_vect;
	
	int i = 0;
	while (test_moduli_vect.size() < 7) {
		std::cout << "creating Test moduli #" << i << std::endl;
		for (;;) {
			//mpz_urandomb(p, mt, MODULUS_BSIZE);		// select a random number for p
			//mpz_urandomb(q, mt, MODULUS_BSIZE);		// select a random number for q
			p_ = mt.get_z_bits(MODULUS_BSIZE);
			q_ = mt.get_z_bits(MODULUS_BSIZE);
			p_isprime = mpz_probab_prime_p(p_.get_mpz_t(), 10); 	// make sure p is prime
			q_isprime = mpz_probab_prime_p(q_.get_mpz_t(), 10);  // make sure q is prime
			if (p_isprime && q_isprime) {break;} 	// p, q must both be prime
		}
		std::cout << "created Test moduli #" << i << std::endl;
		//mpz_mul(n, p, q);							// multiply p and q to make a moduli
		n_ = p_ * q_;
		test_moduli_vect.push_back(n_); // create a mpz_class instance of value n, put it in the vector
		i++;
	}
													// create an additional modulus with a shared factor
	std::cout << "creating Test moduli #" << i << std::endl;
	for (;;) {
		//mpz_urandomb(q, mt, MODULUS_BSIZE);
		q_ = mt.get_z_bits(MODULUS_BSIZE);
		q_isprime = mpz_probab_prime_p(q_.get_mpz_t(), 10);
		if (q_isprime) {break;}
	}
	std::cout << "created Test moduli #" << i << std::endl;
	//mpz_mul(n, p, q);								// multiply p and q to make a moduli
	n_ = p_ * q_;
	test_moduli_vect.push_back( n_ ); 	// create a mpz_class instance of value n, put it in the vector
	i++;
	
	std::cout << "test vector now has " << test_moduli_vect.size() << " moduli" << std::endl;
	
	std::cout << "writing Test moduli to " << filename << std::endl;
	
	FILE* f = fopen(filename);
	for (mpz_class num : test_moduli_vect) {
		gmp_fprintf(f, "%Zx", num.get_mpz_t() );
		// f << num << std::endl;
		// mpz_set(temp, num);
		// gmp_fprintf(f, "%Zx", temp);
	}
	fclose(f);
													// clear GMP variables
	mpz_clears(temp, p, q, n, 0);
	//gmp_randclear(mt);
	return 0;
}