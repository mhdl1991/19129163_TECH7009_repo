#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>

// Stein's GCD algorithm
// non-negative integers only
int64_t stein_gcd( int64_t a, int64_t b ) {
	if (a == b) {return a;}
	if (!a && !b) {return 0;}
	if (!a || !b) {return (a | b);} // if either a or b is 0, return the other one;
	
	// find k, highest power of 2 that divides a and b
	int64_t k;
	for (k = 0; ((a | b) & 1) == 0; ++k) {
		a >>= 1;
		b >>= 1;
	}
	
	// divide a until it is odd
	while ( (a & 1) == 0 ) { a >>= 1; }
	
	// from now on a is always odd
	do {
		while ((b & 1) == 0) { b >>= 1; }
		
		// a and b are now both odd
		// a should be less than b. Swap em
		if (a > b) { std::swap(a, b); }
		b = (b - a); // lots of online implementations of this use this as opposed to say, b -= a;
		
	} while (b != 0);
	
	return a << k; // restore common factors of 2
}

// for Reasons, we will be making a pair to get gcd and lcm
std::pair<int64_t, int64_t> gcd_and_lcm(int64_t a, int64_t b) {
	int64_t c = stein_gcd(a, b);
	return { c, (a * b)/c  };
}

int64_t phi(int64_t p, int64_t q) {
	//std::pair<int, int> gcd_lcm = gcd_and_lcm(p - 1, q - 1);
	//return gcd_lcm.second; // n = pq, phi(n) = lcm(p-1, q-1)
	return ((p - 1) * (q - 1));
}

int64_t mod_exp_v0 (int64_t base, int64_t exp, int64_t mod) {
	int result = 1;
	
	if (1 & exp) { result = base; }
	while(1) {
		if (!exp) {break;}
		exp >>= 1;
		base = (base * base) % mod;
		if (exp & 1) { result = (result * base) % mod; }
	}
	return result;
}

bool is_coprime_test (int64_t p, int64_t q) {
	return (stein_gcd(p, q) == 1);
}

std::vector<int64_t> list_of_e (int64_t phi_n) {
	std::vector<int64_t> list;
	for (int64_t i = 2; i < phi_n; i++){
		if (is_coprime_test(i, phi_n)){list.push_back(i);}
	}
	return list;
}

int64_t get_inverse(int64_t a, int64_t n) {
	int64_t n0 = n, y = 0, x = 1, q,  t;
	if  (n == 1) {return 0;}
	while (a > 1) { 
		q =  a / n;
		t =  n;
		n = a % n;
		a = t;
		t = y;
		y = x - q * y;
		x = t;
	}
	if (x < 0) {x += n0;}
	return x;

}

std::vector<int64_t> encrypt(std::string plaintext, int64_t e, int64_t n){
	// very naive RSA encryption implementation
	std::vector<int64_t> ciphertext;
	int64_t ip, ic;
	
	for (char p: plaintext){
		ip = p;
		ic = mod_exp_v0(ip, e, n);
		ciphertext.push_back(ic);
	}
	return ciphertext;
}

std::string decrypt(std::vector<int64_t> ciphertext, int64_t d, int64_t n) {
	std::string plaintext = "";
	int64_t ip;
	for (int64_t c: ciphertext){
		ip = mod_exp_v0(c, d, n);
		plaintext += (char) ip;
	}
	return plaintext;
	
}

void print_hex(int64_t c) {
	std::cout << std::hex << c;
	std::cout << " ";	
}


int main(int argc, char **argv) {		
	srand((unsigned int)time(NULL));

	std::string plaintext = "Hello world! Isn't this exciting?";
	
	
	
	

	return 0;
}