#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include <chrono>
#include <gmp.h>
#include <gmpxx.h>

// based partly on https://gist.github.com/akosma/865b887f993de462369a04f4e81596b8

// this program will be used to generate ciphertexts for use with the demonstration of Hastad's attack

void save_ciphertext_to_file(std::string dest_filename, std::vector<mpz_class> ciphertext){
	// save ciphertexts to file.
	std::ofstream f_out;
	f_out.open(dest_filename);
	for (mpz_class z : ciphertext) {
		//mpz_out_str(f_out, 16, z.get_mpz_t()); // Write the ciphertext as a hexadecimal 
		f_out << z << " ";
	}
	f_out.close();
	return;
}

std::vector<mpz_class> encrypt (std::string plaintext, const mpz_t e, const mpz_t mod) {
	std::vector<mpz_class> ciphertext;
	mpz_t temp1, temp2;
	mpz_inits(temp1, temp2, 0);
	// encrypt a plaintext using an encryption key e and a modulus mod
	for (char p: plaintext) {
		mpz_set_ui(temp2, (uint) p);
		mpz_powm(temp1, temp2, e, mod);
		ciphertext.push_back( mpz_class(temp1) );
	}
	mpz_clears(temp1, temp2, 0);
	return ciphertext;
}

std::vector<mpz_class> decrypt (std::vector<mpz_class> ciphertext, const mpz_t d, const mpz_t mod) {
	std::vector<mpz_class> plaintext;
	mpz_t temp1;
	mpz_init(temp1);
	// decrypt a plaintext using an encryption key d and a modulus mod
	for (mpz_class p: ciphertext) {
		//mpz_set(temp2, p);
		mpz_powm(temp1, p.get_mpz_t(), d, mod);
		plaintext.push_back( mpz_class(temp1) );
	}
	mpz_clear(temp1);
	return plaintext;
}

void random_p_q_selection(mpz_t p, mpz_t q, gmp_randstate_t s, const mpz_t e) {
	// select random primes for p and q
	mpz_t p1, q1, size, temp;
    bool p_isprime = false;
	mpz_inits(p1, q1, size, temp, 0);
	for(;;){
        mpz_urandomb(p, s, 1024);
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

void euler_totient_function(mpz_t phi_n, const mpz_t p, const mpz_t q) {
	mpz_t p_, q_;
	mpz_inits(p_, q_, 0);
	mpz_sub_ui(p_, p, 1);
	mpz_sub_ui(q_, q, 1);
	mpz_mul(phi_n, p_, q_);
	mpz_clears(p_, q_, 0);
	return;
}

int main(int argc, char **argv) {		
    // initialize the RNG and seed it
	gmp_randstate_t mt;
	gmp_randinit_mt(mt);
    gmp_randseed_ui(mt,(unsigned int)time(NULL) );
    // GMP library variables
	mpz_t _temp1, _temp2, e, d, p, q, n, phi_n;
	mpz_inits(_temp1, _temp2, e, d, p, q, n, phi_n, 0);
	std::vector<mpz_class> ciphertext, decrypted_plaintext;


	// timing
	auto start = std::chrono::high_resolution_clock::now();
	auto end = std::chrono::high_resolution_clock::now();

    // test plaintexts
	std::vector<std::string> plaintext = {"E", "F", "G"};
	
	// files for storing ciphertext
	std::vector<std::string> file_out = {"c1.txt", "c2.txt", "c3.txt"};
	
	// pick an encryption key;
	mpz_set_str(e, "3", 10);
	
	std::cout << "RSA program for testing Hastad's attack" << std::endl;
	std::cout << "Encryption exponent is e = " << e << std::endl;
	
	for (int i = 0; i < 3; i++) {
		start = std::chrono::high_resolution_clock::now();
		
		std::cout << "PLAINTEXT: " << plaintext[i] << std::endl;
		
		// pick two prime numbers p and q where (p-1) and (q-1) are both coprime with e;
		random_p_q_selection(p, q, mt, e);
		// get pq = n
		mpz_mul(n, p, q);
		// euler totient function
		euler_totient_function(phi_n, p, q);
		
		ciphertext = encrypt(plaintext[i], e, n);
		save_ciphertext_to_file(file_out[i], ciphertext);
		
		std::cout << "Saved ciphertext to " << file_out[i] << std::endl;
		
		end = std::chrono::high_resolution_clock::now();
		
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		std::cout << "Time taken: " << duration.count() << std::endl;
	}

    // clear GMP variables
	mpz_clears(_temp1, _temp2, e, d, p, q, n, phi_n, 0);
	gmp_randclear(mt);
	return 0;
}
