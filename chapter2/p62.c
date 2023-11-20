
#define INT_MIN 0x80000000

int int_shifts_are_arithmetic()
{
	int shift_val = (sizeof(int) << 3) - 1;
	int x = -1;
	int x_shifted = x >> shift_val;
	int result = x_shifted & 1;
	return result;
	
}

int main()
{
	printf("int_shifts_are_arithmetic = %d\n", int_shifts_are_arithmetic());
}
