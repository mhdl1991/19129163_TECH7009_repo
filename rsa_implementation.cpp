//RSA ENCRYPTION IMPLEMENTATION?
#include <iostream>
#include <vector>
#define MILLER_RABIN 0


// return list of factors of a number
std::vector<int64_t> get_factors(int64_t n) {
	// return factors of integer n
	std::vector<int64_t> factors;
	for (int64_t i = 2; i < n; i++) {
		if (n % i == 0) {
				factors.insert(i);
		}
	}
	return factors;
}

// Stein's GCD algorithm
// non-negative integers only
int64_t gcd( int64_t a, int64_t b ) {
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
// modular exponentiation 
int64_t mod_exp (int64_t base, int8_t exp, int64_t mod) {
	int64_t result = 1;
	
	if (1 & exp) { result = base; }
	while(1) {
		if (!exp) {break;}
		exp >>= 1;
		base = (base * base) % mod;
		if (exp & 1) { result = (result * base) % mod; }
	}
	return result;
}
// primality test
bool is_prime(int64_t n) {
	//algorithm for testing primality of integer a
	
	if (MILLER_RABIN) {
		// use the Miller-Rabin test instead
		return miller_rabin_prime_test(n);
	}
	if (n == 2 || n == 3) {return true;} //2 and 3 are prime
	if (n < 2 || n & 1 == 0 || n % 3 == 0) {return false;} //0, 1, even numbers, multiples of 3 are not prime	
	for (int i = 5; i * i <= a; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {return false;}
	}
	return true;
}
// Miller-rabin prime test
bool miller_rabin_prime_test (int64_t n) {
	//Miller-Rabin primality test
	
}

// take two primes p and q
// calculate n = p * q
// calculate phi(n) = n + 1 - p - q
// find an intenger e between 1 and phi(n) that is co-prime with phi(n)
void rsa_key_thing(int64_t p, int64_t q) {
	if (!is_prime(p) || !is_prime(q)) { return; } // only works if p and q are both prime
	int64_t n = p * q;
	int64_t phi_n = n + 1 - p - q;
	
	for (int64_t e = 1; e < phi_n; ++e) {
		
	}
		
	
}



int main(int argc, char **argv[]) {
	
	std::cout<<""<<std::endl;
	return 0;
}