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



void generate_modulus(mpz_t p, mpz_t q, gmp_randstate_t s, const mpz_t e, bool shared_factor){
	mpz_t p1, q1, size, temp;
    bool p_isprime = false;
	mpz_inits(p1, q1, size, temp, 0);
		for(;;){
        mpz_urandomb(p, s, 2048);
        mpz_nextprime(q, p);
        mpz_sub_ui(p1, p, 1);
        mpz_sub_ui(q1, q, 1);
        p_isprime = mpz_probab_prime_p(p, 20);
        
        if (!p_isprime) {continue;} // p, q must be prime
        
        mpz_gcd(temp, p1, e);
        if (mpz_cmp_ui(temp, 1) != 0) {continue;}
        mpz_gcd(temp, q1, e);
        if (mpz_cmp_ui(temp, 1) != 0) {continue;}
		break;
	}
	mpz_clears(p1,q1, size, temp, 0);
	return;
}

int main (int argc, char **argv) {
													// initialize the RNG and seed it
	gmp_randstate_t mt;
	gmp_randinit_mt(mt);
    gmp_randseed_ui(mt,(unsigned int)time(NULL) );
													// GMP library variables
	mpz_t temp, p, q, n;
	mpz_inits(temp, p, q, n, 0);
	bool p_isprime = false, q_isprime = false;
													// generate a bunch of fake RSA moduli 
													// for testing the product tree algorithm with
													// write them to a file
	std::string filename = "input.mpz";
	std::vector<mpz_class> test_moduli_vect;
	
	int i = 0;
	while (test_moduli_vect.size() < 10) {
		std::cout << "creating Test moduli #" << i << std::endl;
		for (;;) {
			mpz_urandomb(p, mt, 1024);				// select a random number for p
			mpz_urandomb(q, mt, 1024);				// select a random number for q
			p_isprime = mpz_probab_prime_p(p, 10); 	// make sure p is prime
			q_isprime = mpz_probab_prime_p(q, 10);  // make sure q is prime
			if (p_isprime && q_isprime) {break;} 	// p, q must both be prime
		}
		std::cout << "created Test moduli #" << i << std::endl;
		mpz_mul(n, p, q);							// multiply p and q to make a moduli
		test_moduli_vect.push_back( mpz_class(n) ); // create a mpz_class instance of value n, put it in the vector
		i++;
	}
	
	std::cout << "writing Test moduli to " << filename << std::endl
	std::ofstream f;
	f.open(filename);
	for (mpz_class num : test_moduli_vect) {
		mpz_set(temp, num);
		gmp_fprintf(f, "%Zx", temp);
	}
	f.close();
	
													// clear GMP variables
	mpz_clears(temp, p, q, n, 0);
	gmp_randclear(mt);
	return 0;
}