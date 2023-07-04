#include <iostream>
#include <utility>
#include <vector>

// Stein's GCD algorithm
// non-negative integers only
uint64_t fast_gcd( uint64_t a, uint64_t b ) {
	if (a == b) {return a;}
	if (!a && !b) {return 0;}
	if (!a || !b) {return (a | b);} // if either a or b is 0, return the other one;
	
	// find k, highest power of 2 that divides a and b
	uint64_t k;
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
std::pair<uint64_t, uint64_t> gcd_and_lcm(uint64_t a, uint64_t b) {
	uint64_t c = fast_gcd(a, b);
	return { c, (a * b)/c  };
}

bool coprime_test(uint64_t a, uint64_t b) { return (fast_gcd(a, b) == 1); }


int main(int argc, char **argv) {
	
	std::vector< std::pair<uint64_t, uint64_t> > tests = {
		{1, 16},
		{31, 24},
		{333, 777},
		{2, 32},
		{7542, 1203},
		{111111, 3},
		{93923, 12012},
		{1391992, 120010230},
	};
	
	std::cout << "Test program for getting GCD and LCD of non-negative integers." << std::endl;
	std::pair<uint64_t,uint64_t> x;
	
	for (std::pair<uint64_t, uint64_t> row : tests) { //for each pair in tests
		x = gcd_and_lcm(row.first, row.second);
		std::cout << "The GCD of " << row.first << " and " << row.second << " is " << x.first << ". ";
		if (x.first == 1) {std::cout << "These numbers are coprime." << std::endl;} else {std::cout << std::endl;}
		std::cout << "The LCM of " << row.first << " and " << row.second << " is " << x.second << ".\n" << std::endl;
	}
	return 0;
}