#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>
#define i64 int64_t

// just to make typing shorter lol
#define u64 uint64_t
#define vec std::vector
#define str std::string


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
	if (a== b) {return a;}
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
	//itd::pair<int, int> gcd_lcm = gcd_and_lcm(p - 1, q - 1);
	//return gcd_lcm.second; // n = pq, phi(n) = lcm(p-1, q-1)
	return ((p - 1) * (q - 1));
}

u64 exp_mod (u64 base, u64 exp, u64 mod) {
	int result = 1;
	
	if (1 & exp) { result = base; }
	while(1) {
		if (!exp) {break;}
		exp >>= 1;
		base = mult_mod(base, base, mod); //(base * base) % mod; //
		if (exp & 1) {result = mult_mod(result, base, mod); }//{ result = (result * base) % mod; }//
	}
	return result;
}

bool is_coprime_test (i64 p, u64 q) {
	return (stein_gcd(p, q) == 1);
}

vec<u64> list_of_e (u64 phi_n) {
	vec<u64> list;
	for (u64 i = 2; i < phi_n; i++){
		if (is_coprime_test(i, phi_n)) {list.push_back(i);}
	}
	return list;
}

u64 get_inverse(u64 a, u64 n) {
	u64 n0 = n, y = 0, x = 1, q, t;
	if  (n == 1) {return 0;}
	while (a > 1) { 
		q =  a / n;
		t =  n;
		n = a % n;
		a = t;
		t = y;
		y = x - mult_mod(q, y, n); //x - q * y;
		x = t;
	}
	if(x < 0) {x += n0;}
	return x;
}

vec<u64> encrypt(str plaintext, u64 e, u64 n){
	// very naive RSA encryption implementation
	vec<u64> ciphertext;
	u64 ip, ic;
	
	for (char p: plaintext){
		ip = p;
		ic = exp_mod(ip, e, n);
		ciphertext.push_back(ic);
	}
	return ciphertext;
}

str decrypt(vec<u64> ciphertext, u64 d, u64 n) {
	str plaintext = "";
	u64 ip;
	for (u64 c: ciphertext){
		ip = exp_mod(c, d, n);
		plaintext += (char) ip;
	}
	return plaintext;
}

void print_hex(u64 c) {
	std::cout << std::hex << c;
	std::cout << " ";	
}


int main(int argc, char **argv) {	
	
	srand((unsigned int)time(NULL));
	//u64 p1 = 6491, p2 = 6689;
	u64 p1 = 17, p2 = 41;
	u64 n = p1 * p2;
	u64 phi_n = phi(p1, p2);
	vec<u64> list_e = list_of_e(phi_n);
	u64 e = list_e[rand() % list_e.size()];
	u64 d = get_inverse(e, phi_n);
	
	str plaintext = "Hello world! Isn't this exciting?";
	vec<u64> ciphertext = encrypt(plaintext, e, n);
	
	std::cout << "A very simple RSA implementation demo. Not for serious use" << std::endl;
	std::cout << "We'll be using prime numbers p = " << p1 << " and q = " << p2 << std::endl;
	std::cout << "Their product is " << n << std::endl;
	std::cout << "The value of phi(n) is " << phi_n << std::endl;
	
	// prints possible encryption keys
	// std::cout << "We can use the following values as encryption keys: ";
	//  for (int _e: list_e) { std::cout << _e << ", ";}
	// std::cout << std::endl;
	
	// randomly select an encryption key and calculate it's decryption key
	std::cout << "Let's take an example encryption key of " << e << std::endl;
	std::cout << "This will have a corresponding decryption key of " << d << std::endl;  	
	
	std::cout << "We shall run a test" << std::endl;
	std::cout << "\nENCRYPTION" << std::endl;
	std::cout << "plaintext: " << plaintext << std::endl;
	std::cout << "ciphertext: ";
	for (u64 c: ciphertext){ print_hex(c); }
	std::cout << std::endl;
	
	std::cout << "\nDECRYPTION" << std::endl;
		std::cout << "ciphertext: ";
	for (u64 c: ciphertext){ print_hex(c); }
	std::cout << std::endl;
	std::cout << "plaintext: " << decrypt(ciphertext, d, n) << std::endl;
	
	return 0;
}