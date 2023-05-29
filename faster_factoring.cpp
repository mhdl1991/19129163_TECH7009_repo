#include <iostream>
#include <vector>

std::vector<int> list_factors(int n) {
	std::vector<int> factors;
	for (int d = 2; d * d <= n; d++) { // no need to calculate the square root of n
		while ( (n % d) == 0 ) {
			factors.push_back(d);
			n /= d;
		}
	}
	if (n > 1) {
		factors.push_back(n);
	}
	return factors;
}

std::vector<int> list_factors_v2(int n) {
	std::vector<int> factors;
	for (int d : {2, 3, 5}) {
		while (n % d == 0) {
			factors.push_back(d);
			n /= d;
		}
	}
	static array<int, 8> increments = {4, 2, 4, 2, 4, 6, 2, 6};
	int i = 0;
	for (int d = 7; d * d <= n; d += increments[i++ % 8]) {
        while (n % d == 0) {
            factors.push_back(d);
            n /= d;
        }
    }
    if (n > 1) {factors.push_back(n);}
    return factors;
}





int main() {
	std::cout<<"Quick Factoring algorithm test"<<std::endl;
	
	int n = 360;
	std::vector<int> factors = list_factors_v2(n);
	for (int& x: factors) {
		std::cout << x << ", ";
	}
	std::cout<<""<<std::endl;
	
	return 0;
}