#include <iostream>
#include <iomanip>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <climits>

/*	RSA demo implementation version 2
*	this one will use some form of big integer class
*	and the prime numbers will be randomly selected
*	
*/
public namespace BigNumbers {

	public class HugeInt {
		protected:
			std::vector<uint32_t> value_; // integers stored as Base 2^32 numbers
			// each digit in the number has a value from 0 to ((2^32) - 1)
		 	uint32_t min_digit = 0;
			uint32_t max_digit = ULONG_MAX;
		
		public:
			HugeInt(){
				value_ = {0};
			}
			
			// addition
			HugeInt & HugeInt::operator += (HugeInt const& operand){
				uint32_t count, carry = 0, op0, op1, digits_result;
				for (count = 0; count < std::max(value_.size(), operand.value_.size());  count++){
					op0 = count < value_.size() ? value_.at(count) : 0;
					op1 = count < operand.value_.size() ? operand.value_.at(count) : 0;
					digits_result = op0 + op1 + carry;
					
				}
			}
	};
}