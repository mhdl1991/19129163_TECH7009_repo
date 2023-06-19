#include <iostream>
#include <iomanip>
#include<fstream>
#include <string>
#include <vector>
#include <gmp.h>
#include <gmpxx.h>
// test for implementing Hastad's attack in C++
// using ciphertexts that have been created using Textbook RSA

std::vector<mpz_class> read_ciphertext_from_file(std::string filename){
    std::vector<mpz_class> ciphertext;
    std::ifstream f_in;
    std::string word;
    mpz_t i;
    mpz_init(i);
    f_in.open(filename);
    // ciphertext has numbers as space separated strings.
    while(f_in >> word) {
            // std::cout << word << std::endl;
            mpz_set_str(i, word.c_str(), 10);
            ciphertext.push_back( mpz_class(i) );
    }
    f_in.close();
    mpz_clear(i);
    return ciphertext;
}

int main (int argc, char **argv){
	std::vector<std::string> file_names = {"c1.txt", "c2.txt", "c3.txt"};
	
	mpz_t _C1, _C2, _C3, _C, temp;
	mpz_inits(_C1, _C2, _C3, _C, temp, 0);
	
	std::vector<mpz_class> c1 = read_ciphertext_from_file("c1.txt")
		,	c2 = read_ciphertext_from_file("c2.txt")
		,	c3 = read_ciphertext_from_file("c3.txt");

	mpz_set(_C1, c1[0].get_mpz_t());
	mpz_set(_C2, c2[0].get_mpz_t());
	mpz_set(_C3, c3[0].get_mpz_t());
	
	mpz_mul(temp, _C1, _C2);
	mpz_mul(_C, temp, _C3);
	

	mpz_clears(_C1, _C2, _C3, _C, temp, 0);
	return 0;
}
