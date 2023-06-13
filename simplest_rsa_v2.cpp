#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include<gmp.h>
#include<gmpxx.h>



int main(int argc, char **argv) {		
	srand((unsigned int)time(NULL));
	std::string plaintext = "Hello world! Isn't this exciting?";
	
	mpz_t e, d, p, q;
	mpz_inits(e, 3);
		
	

	return 0;
}