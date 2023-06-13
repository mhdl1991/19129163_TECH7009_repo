#include<iostream>
#include<gmp.h>
#include<gmpxx.h>

int main (int argc, char **argv){
    // a simple test program to see how the GMP library works
    mpz_t a, b, c;
    mpz_inits(a, b, c, NULL);
    
    mpz_set_str(a, "1234567890", 10);
    mpz_set_str(b, "-3456789012", 10);
    
    mpz_add(c, a, b);
    
    std::cout << "Test program using GMP" << std::endl;
    
    std::cout << "A= ";
    mpz_out_str(stdout, 10, a);
    std::cout << std::endl;
    std::cout << "B= ";
    mpz_out_str(stdout, 10, b);
    std::cout << std::endl;

    std::cout << "A + B = ";
    mpz_out_str(stdout, 10, c);
    std::cout << std::endl;
    return 0;
}
