#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#include <gmp.h>
#include <gmpxx.h>


// based partly on https://gist.github.com/akosma/865b887f993de462369a04f4e81596b8

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
	srand((unsigned int)time(NULL));
	
	std::string plaintext = "Textbook RSA encryption program made using the GMP library";
	std::vector<mpz_class> ciphertext, decrypted_plaintext;


	mpz_t _temp1, _temp2, e, d, p, q, n, phi_n;
	mpz_inits(_temp1, _temp2, e, d, p, q, n, phi_n, 0);

	// pick an encryption key;
	mpz_set_str(e, "3", 10);

	// pick two prime numbers p and q where (p-1) and (q-1) are both coprime with e;
	mpz_set_str(p, "6491", 10);
	mpz_set_str(q, "6689", 10);

	// calculate n = pq and phi(n) = (p-1)(q-1)
	mpz_mul(n, p, q);
	euler_totient_function(phi_n, p, q);

	// get the decryption key (inverse of e in modulo phi(n) )
	mpz_invert(d, e, phi_n);

	mpz_mul(_temp1, e, d);
	mpz_mod(_temp2, _temp1, phi_n);
	if (mpz_cmp_ui(_temp2, 1) != 0) {
		std::cout << "Something has gone wrong here" << std::endl;
	}



	std::cout << "Textbook RSA C++ program using the GMP library." << std::endl;
	std::cout << "Encryption exponent is e = " << e << std::endl;
	std::cout << "Primes chosen are p = " << p << " and q = " << q << std::endl;
	std::cout << "n = pq = " << n << std::endl;
	std::cout << "phi(n) = (p-1)(q-1) = " << phi_n << std::endl;
	std::cout << "decryption key is d = " << d << std::endl;

	std::cout << "ENCRYPTION" << std::endl;
	std::cout << "PLAINTEXT:\n\t" << plaintext << std::endl;

	std::cout << "PLAINTEXT AS INTS:" << std::endl;
	for (char c : plaintext) {
		std::cout << (uint) c << " ";
	}
	std::cout << std::endl;

	ciphertext = encrypt(plaintext, e, n);
	decrypted_plaintext = decrypt(ciphertext, d, n);

	std::cout << "CIPHERTEXT:\n\t";
	for (mpz_class m : ciphertext) {
		std::cout << m << " ";
	}
	std::cout << std::endl;
	

	std::cout << "DECRYPTION" << std::endl;
	std::cout << "DECRYPTED CIPHERTEXT:\n\t";
	for (mpz_class m : decrypted_plaintext) {
		std::cout << m << " ";
	}
	std::cout << std::endl;
	std::cout << "CONVERTING BACK TO STRING\n\t";
	for (mpz_class m : decrypted_plaintext) {
		std::cout << (char) mpz_get_ui(m.get_mpz_t());
	}
	std::cout << std::endl;

	mpz_clears(_temp1, _temp2, e, d, p, q, n, phi_n, 0);
	return 0;
}