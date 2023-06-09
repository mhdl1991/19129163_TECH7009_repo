#include<iostream>
#include<iomanip>

int64_t mod_exp (int64_t base, int64_t exp, int64_t mod) {
	int64_t result = 1;
	if (1 & exp) { result = base; }
	while(1) {
		std::cout << "exp: " << exp << " base: " << base << " result: " << result << std::endl;
		
		if (!exp) {break;}
		exp >>= 1;
		std::cout << "new exp: exp / 2 = " << exp << std::endl;
		base = (base * base) % mod;
		std::cout << "new base: base * base mod " << mod << " = " << base << std::endl;
		
		if (exp & 1) { 
			result = (result * base) % mod; 
			
			std::cout << "new result: result * " << base << " mod " << mod << " = " << result << std::endl;
		}
	}
	std::cout<< " result: " << result << std::endl;
	return result;
}

int main(int argc, char **argv) {
	int64_t base = 303;
	int64_t exp = 103;
	int64_t modulus = 403;
	mod_exp(base, exp, modulus);
	
	return 0;
}