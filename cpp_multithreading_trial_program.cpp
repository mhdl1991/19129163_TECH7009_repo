#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <string>

#define NTHREADS 4
#define U64 uint64_t
#define VEC std::vector
#define STR std::string
// C++ multithreading test thingy
// the product tree/remainder tree program but with multithreading implemented.

void print_vec(VEC<U64> V);
VEC<U64> read_file(STR filename);
void write_file(VEC<U64> V, STR filename);
int product_tree(VEC<U64> V);
void iter_threads(int start, int end, std::function<void(int)> func);
U64 stein_gcd( U64 a, U64 b );
void results(VEC<U64> moduli);

// Stein's GCD algorithm
// non-negative integers only
U64 stein_gcd( U64 a, U64 b ) {
	if (a== b) {return a;}
	if (!a && !b) {return 0;}
	if (!a || !b) {return (a | b);} // if either a or b is 0, return the other one;
	// find k, highest power of 2 that divides a and b
	U64 k;
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

// print the contents of a vector
void print_vec(VEC<U64> V) {
	for (U64 n : V) { std::cout << n << ", "; } 
	std::cout << std::endl;
}

VEC<U64> read_file(STR filename){
	std::ifstream f;
	STR line;
	f.open(filename);
	VEC<U64> V;
	U64 n;
	while (getline(f, line)) {
		n = stoull(line);
		V.push_back(n);
	}
	f.close();
	return V;
}

void write_file(VEC<U64> V, STR filename){
	std::ofstream f;
	f.open(filename);
	for (U64 n : V) { f << n << std::endl; }
	f.close();
	return;
}

int product_tree(VEC<U64> V){
	VEC<U64> V_new;
	int level = 0;
	STR f = "p";
	
	std::cout << "PRODUCT TREE" << std::endl;
	while (V.size() > 1) {
		V_new.resize( (V.size() + 1) / 2 );
		
		auto mult = [&](int k){
			V_new[k] = V[2 * k] * V[(2 * k) + 1];
		};
		iter_threads(0, V.size() / 2, mult);
		
		if (V.size() & 1) { V_new[V.size()/2] = V[V.size()-1]; }
		
		std::cout << "LEVEL " << level << ": ";
		print_vec(V);
		
		f = "p" + std::to_string(level);
		write_file(V_new, f);
		V = V_new;
		level++;
	}
	return level;
}

void remainder_tree(int level, VEC<U64> orig) {
	VEC<U64> U, V;
	U64 S;
	STR f = "p" + std::to_string(level);
	
	U = read_file(f);
	
	std::cout << "REMAINDER TREE: " << std::endl;
	std::cout << "LEVEL " << level << ": ";
	print_vec(U);
	
	while (level > 0) {
		level--;
		f = "p" + std::to_string(level);
		V = read_file(f);
		auto mult = [&](int i){
			S = V[i] * V[i];
			V[i] = U[i / 2] % S;
		};
		iter_threads(0, V.size(), mult);
		std::cout << "LEVEL " << level << ": ";
		print_vec(V);
		
		f = "r" + std::to_string(level);
		write_file(V, f);
		U = V;
	}
	VEC<U64> W(orig.size(), 0);
	auto multdiv = [&](int i) {
		S = V[i] * V[i];
		W[i] = U[i/2] % S;
		W[i] /= orig[i];
		W[i] = stein_gcd(W[i], orig[i]);
	};
	iter_threads(0, orig.size(), multdiv);
	
	std::cout << "GCDs: ";
	print_vec(W);
	
	write_file(W, "output");
}

void results(VEC<U64> moduli){
	VEC<U64> gcds = read_file("output");
	VEC<U64> vulnerable;
	
	for (int i = 0; i < gcds.size(); i++){
		if (gcds[i] != 1) {
			vulnerable.push_back(moduli[i]);
		}
	}
	std::cout << "VULNERABLE MODULI: ";
	print_vec(vulnerable);
	std::cout << std::endl;
}

void iter_threads(int start, int end, std::function<void(int)> func) {
	int n = start;
	std::mutex m;
	auto thread_body = [&](void*) {
		for (;;) {
			std::lock_guard<std::mutex> lock(m);
			int i = n++;
			if (i >= end) {break;}
			func(i);
		}
	};
	VEC<std::thread> threads;
	for (int j = 0; j < NTHREADS; j++) {threads.emplace_back(thread_body, nullptr);}
	for (std::thread &t : threads) {t.join();}
}

int main (int argc, char **argv) {
	// test of the product tree algorithm
	// Simplified using small numbers, no multithreading.
	// convert this to use MPZ values from a file later and have multi threading
	VEC<U64> test_nums = {21, 15, 33, 221, 437, 899, 111, 123}; // factor 3 in common with the  first three
	
	VEC<U64> test_nums_copy = {21, 15, 33, 221, 437, 899, 111, 123};
	std::cout << "NUMBERS: "; 
	print_vec(test_nums_copy);
	
	int levels = product_tree(test_nums_copy);
	std::cout << "NUMBER OF LEVELS " << levels << std::endl;
	
	remainder_tree(levels - 1, test_nums);
	results(test_nums);
	return 0;
}