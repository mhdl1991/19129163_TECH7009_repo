#include <iostream>
#include <iomanip>
#include<fstream>
#include <string>
#include <vector>
#include <gmp.h>
#include <gmpxx.h>
// test for implementing Hastad's attack in C++
// using ciphertexts that have been created using Textbook RSA

std::vector<mpz_class> read_from_file(std::string filename){
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
	mpz_t 	_C1, _C2, _C3, _C, 
			_N1, _N2, _N3, _N,
			n1, n2, n3,
			y1, y2, y3,
			x,
			temp1, temp2;
	
	mpz_inits(	_C1, _C2, _C3, _C, 
				_N1, _N2, _N3, _N, 
				n1, n2, n3,
				y1, y2, y3,
				x,
				temp1, temp2, 0);
	
	std::vector<mpz_class> c1 = read_from_file("c1.txt")
		,	c2 = read_from_file("c2.txt")
		,	c3 = read_from_file("c3.txt")
		,	n1 = read_from_file("n1.txt")
		,	n2 = read_from_file("n2.txt")
		,	n3 = read_from_file("n3.txt");

	mpz_set(_C1, c1[0].get_mpz_t());
	mpz_set(_C2, c2[0].get_mpz_t());
	mpz_set(_C3, c3[0].get_mpz_t());
	
	mpz_set(_N1, n1[0].get_mpz_t());
	mpz_set(_N2, n2[0].get_mpz_t());
	mpz_set(_N3, n3[0].get_mpz_t());
	
	mpz_mult(temp1, _N1, _N2); 
	mpz_mult(_N, temp1, _N3); // N = N1 * N2 * N3
	
	mpz_fdiv_q(n1, _N, _N1); // n1 = N / N1
	mpz_fdiv_q(n2, _N, _N2); // n2 = N / N2
	mpz_fdiv_q(n3, _N, _N3); // n3 = N / N3
	
	mpz_invert(y1, _N1, n1);  // n1 * y1 = 1 mod N1
	mpz_invert(y2, _N2, n2);  // n2 * y2 = 1 mod N2
	mpz_invert(y3, _N3, n3);  // n3 * y3 = 1 mod N3
	
	// x =  c1y1n1 + c2y2n2 + c3y3n3 (mod N)
	mpz_mult(temp2, c1, y1);
	mpz_mult(temp1, temp2, n1);
	mpz_add(x, temp1);
	
	mpz_mult(temp2, c2, y2);
	mpz_mult(temp1, temp2, n2);
	mpz_add(x, temp1);

	mpz_mult(temp2, c3, y3);
	mpz_mult(temp1, temp2, n3);
	mpz_add(x, temp1);
	
	std::cout << x << std::endl;
	

	mpz_clears(	_C1, _C2, _C3, _C, 
				_N1, _N2, _N3, _N, 
				n1, n2, n3,
				y1, y2, y3,
				x,
				temp1, temp2, 0);
	return 0;
}
