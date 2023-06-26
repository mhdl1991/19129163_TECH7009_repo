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
	mpz_t 	C1, C2, C3, C, 
			N1, N2, N3, N,
            n1, n2, n3,
			y1, y2, y3,
			x,
			temp1, temp2, temp3;
	
	mpz_inits(	C1, C2, C3, C, 
				N1, N2, N3, N, 
                n1, n2, n3,
				y1, y2, y3,
				x,
				temp1, temp2, temp3, 0);
	
	std::vector<mpz_class> read_c1 = read_from_file("c1.txt")
		,	read_c2 = read_from_file("c2.txt")
		,	read_c3 = read_from_file("c3.txt")
		,	read_n1 = read_from_file("n1.txt")
		,	read_n2 = read_from_file("n2.txt")
		,	read_n3 = read_from_file("n3.txt");

	mpz_set(C1, read_c1[0].get_mpz_t());
	mpz_set(C2, read_c2[0].get_mpz_t());
	mpz_set(C3, read_c3[0].get_mpz_t());
	
	mpz_set(N1, read_n1[0].get_mpz_t());
	mpz_set(N2, read_n2[0].get_mpz_t());
	mpz_set(N3, read_n3[0].get_mpz_t());
	
	mpz_mul(temp1, N1, N2); 
	mpz_mul(N, temp1, N3); // N = N1 * N2 * N3
	
	mpz_fdiv_q(n1, N, N1); // n1 = N / N1
	mpz_fdiv_q(n2, N, N2); // n2 = N / N2
	mpz_fdiv_q(n3, N, N3); // n3 = N / N3
	
	mpz_invert(y1, n1, N1);  // n1 * y1 = 1 mod N1
	mpz_invert(y2, n2, N2);  // n2 * y2 = 1 mod N2
	mpz_invert(y3, n3, N3);  // n3 * y3 = 1 mod N3
	
	// x =  c1y1n1 + c2y2n2 + c3y3n3 (mod N)
    mpz_set_ui(x, 0);
    mpz_mul(temp1, C1, y1); 
    mpz_mul(temp2, temp1,  n1);
    mpz_add(x, x, temp2);
    
    mpz_mul(temp1, C2, y2);
    mpz_mul(temp2, temp1,  n2);
    mpz_add(x, x, temp2);
    
    mpz_mul(temp1, C3, y3);
    mpz_mul(temp2, temp1,  n3);
    mpz_add(x, x,temp2);
    
    mpz_mod(x, x, N);
    
    mpz_mod(temp1, x, N1);
    mpz_mod(temp2, x, N2);
    mpz_mod(temp3, x, N3);
	
	std::cout << "x = " << x << std::endl;
    
    std::cout << "x (mod N1) = "  << temp1 << std::endl;
    std::cout << "x (mod N2) = "  << temp2 << std::endl;
    std::cout << "x (mod N3) = "  << temp3 << std::endl;
    
    mpz_root(x, x, 3);
    std::cout << "cube root of x = " << x << std::endl;
	

	mpz_clears(	C1, C2, C3, C, 
				N1, N2, N3, N, 
                n1, n2, n3,
				y1, y2, y3,
				x,
				temp1, temp2, temp3, 0);
	return 0;
}
