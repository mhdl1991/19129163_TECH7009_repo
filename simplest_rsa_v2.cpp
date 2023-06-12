#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>

// I kept forgetting where the _ is supposed to go
#define i64 int64_t
#define u64 uint64_t


// Modular multiplication
u64 mult_mod(u64 a, u64 b, u64 p){
	u64 res = 0;
	u64 temp_b = 0;
	if (b >= p) {	
		if (p > UINT64_MAX / 2u) 
		{ b -= p; } 
		else { b %= p; }
	}
	
	while (a) {
		if (a & 1) {
			if (b >= p - res) { res -= p; }
			res += b;
		}
		a >>= 1;
		
		temp_b = b;
		if (b >= p - b) { temp_b -= p; }
		b += temp_b;
	}
	return res;
}

// Stein's GCD algorithm
// non-negative integers only
u64 stein_gcd( u64 a, u64 b ) {
	if (a == b) {return a;}
	if (!a && !b) {return 0;}
	if (!a || !b) {return (a | b);} // if either a or b is 0, return the other one;
	
	// find k, highest power of 2 that divides a and b
	u64 k;
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
std::pair<u64, u64> gcd_and_lcm(u64 a, u64 b) {
	u64 c = stein_gcd(a, b);
	return { c, (a * b)/c  };
}

u64 phi(u64 p, u64 q) {
	//std::pair<int, int> gcd_lcm = gcd_and_lcm(p - 1, q - 1);
	//return gcd_lcm.second; // n = pq, phi(n) = lcm(p-1, q-1)
	return ((p - 1) * (q - 1));
}

u64 mod_exp_v0 (u64 base, u64 exp, u64 mod) {
	u64 result = 1;
	
	if (1 & exp) { result = base; }
	while(1) {
		if (!exp) {break;}
		exp >>= 1;
		base = mult_mod(base, base, mod); //(base * base) % mod;
		if (exp & 1) {result = mult_mod(result, base, mod); }//{ result = (result * base) % mod; }
	}
	return result;
}

bool is_coprime_test (u64 p, u64 q) {
	return (stein_gcd(p, q) == 1);
}

std::vector<u64> list_of_e (u64 phi_n) {
	std::vector<u64> list;
	for (u64 i = 2; i < phi_n; i++){
		if (is_coprime_test(i, phi_n)){list.push_back(i);}
	}
	return list;
}

u64 get_inverse(u64 a, u64 n) {
	u64 n0 = n, y = 0, x = 1, q,  t;
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

std::vector<u64> encrypt(std::string plaintext, u64 e, u64 n){
	// very naive RSA encryption implementation (textbook RSA?)
	std::vector<u64> ciphertext;
	u64 ip, ic;
	
	for (char p: plaintext){
		ip = p;
		ic = mod_exp_v0(ip, e, n);
		ciphertext.push_back(ic);
	}
	return ciphertext;
}

std::string decrypt(std::vector<u64> ciphertext, u64 d, u64 n) {
	std::string plaintext = "";
	u64 ip;
	for (u64 c: ciphertext){
		ip = mod_exp_v0(c, d, n);
		plaintext += (char) ip;
	}
	return plaintext;
	
}

void print_hex(u64 c) {
	std::cout << std::hex << c;
	std::cout << " ";	
}

u64 rand_between(u64 a, u64 b) {

}


bool miller_rabin_prime_test(u64 n, int k) {

}




int main(int argc, char **argv) {		
	srand((unsigned int)time(NULL));

	std::string plaintext = "Hello world! Isn't this exciting?";
	
	u64 e = 3; // choose an exponent, and base selection of p and q based on this choice
	
	
	

	return 0;
}