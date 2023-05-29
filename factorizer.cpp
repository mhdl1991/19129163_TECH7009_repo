#include <iostream>

// Stein's GCD algorithm for non-negative integers
static int gcd(int a, int b) {
	if (a == b) {return a;}
	if (!a && !b) {return 0;}
	
	if (!a || !b) {return (a | b);} // if either a or b is 0, return the other one;
		
	// find k, highest power of 2 that divides a and b
	int k;
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

int main (int argc, char **argv){
	// factorization program
	
	int x, a, b;
	std::cout << "Please enter a number, I will try and get two factors for it:" << std::endl;
	while (1) {
		std::cin >> x;
		if (!std::cin.fail()) break;
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << "invalid entry. Try again";
	}
	
	// default  answer
	a = 1;
	b = x;
	
	// x is even
	if ( !(x&1) ) {
		a = 2;
		b = x>>1;
	} else {
		// x is odd
		
		
	}




	std::cout << "The factors of " << x << " are " << a << " and " << b << std::endl;	
	
	return 0;
}