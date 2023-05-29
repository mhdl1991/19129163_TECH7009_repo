#include <iostream>
using namespace std;
int main() {
	cout<<"THEORY: MODULO BASE 2^N IS IDENTICAL TO BITWISE AND WITH (2^N - 1)"<<endl;
	cout<<"TEST:\n"<<endl;
	
	int base = (2 << 8);
	int test_start = base - 10;
	int test_end = base + 10;
	
	cout<<"NUMBERS: \n";
	for (int i = test_start; i != test_end; i++) { cout << i << ", ";}
	cout<<endl;
	
	cout<<"MODULO BASE "<<base<<":"<<endl;
	for (int i = test_start; i != test_end; i++) { cout << (i % base) << ", ";}
	cout<<endl;
	
	cout<<"AND WITH "<<(base-1)<<":"<<endl;
	for (int i = test_start; i != test_end; i++) { cout << (i & (base - 1)) << ", ";}
	cout<<endl;
	
	return 0;
}