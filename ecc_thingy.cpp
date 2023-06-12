#include <iostream>
#include <vector>
#include <tuple>
#include <climits>

uint64_t pow_mod(uint64_t a, uint64_t b, uint64_t p);
uint64_t add_mod(uint64_t a, uint64_t b, uint64_t p);
uint64_t mult_mod(uint64_t a, uint64_t b, uint64_t p);
uint64_t poly_mod(uint64_t a, uint64_t b, uint64_t p);


// all ECC operations happen with a pulus

// Tonelli-Shanks
std::tuple<uint64_t, uint64_t, bool> sqrt_mod(uint64_t a, uint64_t p) {
	uint64_t q = p - 1;
    uint64_t ss = 0;
    uint64_t z = 2;
    uint64_t c, r, t, m;
	
	if (pow_mod(a, (p - 1) / 2, p) != 1) {
        return std::tuple<uint64_t, uint64_t, bool> {0, 0, false};
    }
	
	while (!(q&1)) {
		ss++;
		q >>= 1;
	}
	
	while (ss == 1) {
		uint64_t r1 = pow_mod(a, (p + 1) / 4, p);
		return std::tuple<uint64_t, uint64_t, bool> {r1, p - r1, true};
	}
	
	while ( pow_mod(z, (p - 1) / 2, p) != p - 1) { z++; }
	c = pow_mod(z,q,p);
	r = pow_mod(a, (q + 1) / 2, p);
	t = pow_mod(a, q, p);

	
	
}

// Modular exponentiation
uint64_t pow_mod(uint64_t a, uint64_t b, uint64_t p) {
	int x = 1;
	if (1 & b) { x = a; }
	while(1) {
		if (!b) {break;}
		b >>= 1;
		a = mult_mod(a, a, p); //(a * a) % p;
		if (b & 1) { mult_mod(x, a, p); } //{ x = (x * a) % p; }
	}
	return x;
}

// Modular addition
uint64_t add_mod(uint64_t a, uint64_t b, uint64_t p) {
	a %= p;
	b %= p;
	uint64_t sum = a + b;
	if (sum >= p || sum < a) {
		sum -= p;
	}
	return sum;
}

// Modular multiplication
uint64_t mult_mod(uint64_t a, uint64_t b, uint64_t p){
	uint64_t res = 0;
	uint64_t temp_b = 0;
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

// Horner's Method
uint64_t poly_mod(uint64_t x, std::vector<uint64_t> coefficients, uint64_t p) {
	uint64_t res = 0;
	for (uint64_t n : coefficients) {
		res = add_mod( mult_mod(res, x), n);
	}
	return res;
}







int main(int argc, char **argv){

	return 0;
}