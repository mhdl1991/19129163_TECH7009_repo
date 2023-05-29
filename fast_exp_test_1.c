#include <stdio.h>
#include <time.h>


uint64_t mod_exp_v0 (uint64_t base, int8_t exp, uint64_t mod) {
	uint64_t result = 1;
	
	if (1 & exp) { result = base; }
	while(1) {
		if (!exp) {break;}
		exp >>= 1;
		base = (base * base) % mod;
		if (exp & 1) { result = (result * base) % mod; }
	}
	return result;
}

uint64_t ipow_v0(uint64_t x, int8_t exp) {
	if (exp == 0) {return x;}
	if (x == 1) {return 1;}
	
	uint64_t result = 1;
	int8_t	_x = x;
	while (_x != 0) {
		result *= _x;
		_x--;
	}
	return result;
}

uint64_t ipow_v1 (uint64_t x, int8_t exp) {
	if (exp == 0) {return x;}
	if (x == 1) {return 1;}
	
	uint64_t _x = x, result = 1;
	while(1) {
		if (!exp) {break;}
		
		if (exp & 1) {
			result *= _x;
		}
		exp >>= 1;
		_x *= _x;
	}
	return result;
}

int main() {
	
	clock_t start = clock(), diff;
	printf("Testing some integer exponentiation algorithms\n");
	printf("The cubes of the numbers from 0 to 35:\n");
	
	
	start = clock();
	printf("using function ipow_v0():\n");
	for (uint64_t i = 0; i < 36; i++) {
		printf("%d\t", ipow_v0(i, 3) );
		if (i % 5 == 0) {printf("\n");} // five per line
	}
	diff = clock() - start;
	printf("Time taken: %d.\n", diff);
	
	
	start = clock();
	printf("using function ipow_v1():\n");
	for (uint64_t i = 0; i < 36; i++) {
		printf("%d\t", ipow_v1(i, 3) );
		if (i % 5 == 0) {printf("\n");} // five per line
	}
	diff = clock() - start;
	printf("Time taken: %d.\n", diff);
	
	start = clock();
	printf("using function mod_exp_v0() (GIVES MODULAR EXPONENTIATION):\n");
	for (uint64_t i = 0; i < 36; i++) {
		printf("%d\t", mod_exp_v0(i, 3, 255) );
		if (i % 5 == 0) {printf("\n");} // five per line
	}
	diff = clock() - start;
	printf("Time taken: %d.\n", diff);
	
	return 0;
}