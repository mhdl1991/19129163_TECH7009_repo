#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <format>
#include <random>

// just to make typing shorter
#define u64 uint64_t
#define i64 int64_t
#define vec std::vector
#define str std::string

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


void write_file(vec<u64> nums, str filename){
	std::ofstream f;
	f.open(filename);
	for (u64 n : nums) {
		f << n << std::endl;
	}
	f.close();
	return;
}

vec<u64> read_file(str filename) {
	std::ifstream f;
	str line;
	f.open(filename);
	vec<u64> nums;
	u64 n;
	while (getline(f, line)) {
		n = stoull(line);
		nums.push_back(n);
	}
	f.close();
	return nums;
}



void remainder_tree() {
	
}

u64 product_tree(vec<u64> v) {
	
	vec<u64> v_new(v.size(), 0);
	u64 new_size;
	u64 level = 0;
	
	while (v.size() > 1) {
		new_size = (v.size() + 1) / 2;
		v_new.resize(new_size);
		for (int n = 0; n < v.size() / 2; n++) {
			v_new[n] = v[n * 2] * v[n * 2 + 1];
		}
		if (v.size() & 1) {
			v_new[v.size()/2] = v[v.size() - 1];
		}
		write_file(v_new, std::format("p{}",level) );
		v = v_new;
		level++;
	}
	return level;
}



int main (int argc, char **argv) {
	// test of the product tree algorithm
	// Simplified using small numbers, no multithreading.
	// convert this to use MPZ values from a file later and have multi threading
	vec<u64> test_nums = {1517, 1591, 2627, 3589}; // all of these have the factor 37 in common
	
	std::cout << "NUMBERS: " << std::endl; 
	for (u64 u : test_nums) {
		std::cout << u << ", ";
	}
	std::cout << std::endl;
	
	
	
	
	return 0;
}