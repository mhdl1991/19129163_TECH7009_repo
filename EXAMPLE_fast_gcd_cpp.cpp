#include <iostream>
#include <fstream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
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



void remainder_tree(int level, vec<u64> orig) {
	str f_name = "p" + std::to_string(level);
	vec<u64> P, v;
	P = read_file(f_name);
	u64 sqr;
	
	std::cout << "REMAINDER TREE: " << std::endl;
	while (level > 0) {
		level--;
		f_name = "p" + std::to_string(level);
		
		v = read_file(f_name);
		
		for (int j = 0; j < v.size(); j++) {
			sqr = v[j] * v[j];
			v[j] = P[j / 2] % sqr;
		}
		
		std::cout << "LEVEL " << level << ": ";
		for(u64 n : v) {
			std::cout << n << ", ";
		}
		std::cout << std::endl;
		
		f_name = "r" + std::to_string(level);
		write_file(v, f_name);
		P = v;
	}
	
	vec<u64> w(orig.size(), 0);
	for (int k = 0; k < orig.size(); k++) {
		sqr = orig[k] * orig[k];
		w[k] = P[k / 2] % sqr;
		w[k] /= orig[k];
		w[k] = stein_gcd(w[k],orig[k]);
	}
	
	std::cout << "GCDs " << level << ": ";
	for(u64 n : w) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	
	
	write_file(w, "output");
	
	
}

int product_tree(vec<u64> v) {
	
	vec<u64> v_new(v.size(), 0);
	u64 new_size;
	int level = 0;
	str f_name = "p";
	
	std::cout << "PRODUCT TREE" << std::endl; 
	while (v.size() > 1) {
		new_size = (v.size() + 1) / 2;
		v_new.resize(new_size);
		for (int n = 0; n < v.size() / 2; n++) {
			v_new[n] = v[n * 2] * v[n * 2 + 1];
		}
		if (v.size() & 1) {
			v_new[v.size()/2] = v[v.size() - 1];
		}
		
		std::cout << "LEVEL " << level << ": ";
		for (u64 n : v_new) {
			std::cout << n << ", ";
		}
		std::cout << std::endl;
		
		
		f_name = "p" + std::to_string(level);
		write_file(v_new, f_name);
		v = v_new;
		level++;
	}
	return level;
}

void results(vec<u64> moduli){
	
	vec<u64> gcds = read_file("output");
	vec<u64> vulnerable;
	
	int size = 0;
	
	for (int i = 0; i < gcds.size(); i++) {
		if (gcds[i] != 1) {
			vulnerable.push_back(moduli[i]);
		}
	}
	
	std::cout << "VULNERABLE MODULI: " << std::endl;
	for (u64 n : vulnerable) {
		std::cout << n << ", ";
	}
	std::cout << std::endl;
	
}


int main (int argc, char **argv) {
	// test of the product tree algorithm
	// Simplified using small numbers, no multithreading.
	// convert this to use MPZ values from a file later and have multi threading
	vec<u64> test_nums = {1517, 1591, 2627, 3589, 95477, 497009, 82919, 7289}; // all of these have the factor 37 in common
	
	vec<u64> test_nums_copy = {1517, 1591, 2627, 3589, 95477, 497009, 82919, 7289}; // all of these have the factor 37 in common
	std::cout << "NUMBERS: " << std::endl; 
	for (u64 u : test_nums_copy) {
		std::cout << u << ", ";
	}
	std::cout << std::endl;
	int levels = product_tree(test_nums_copy);
	std::cout << "NUMBER OF LEVELS " << levels << std::endl;
	
	remainder_tree(levels - 1, test_nums);
	
	results(test_nums);
	
	return 0;
}