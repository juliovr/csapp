
int any_bit_equals_1(int x)
{
	int mask = -1;
	int result = x & mask;
	
	return !!result;
}

int any_bit_equals_0(int x)
{
	int mask = -1;
	int result = x ^ mask;
	
	return !!result;
}

int any_bit_in_lsb_equals_1(int x)
{
	int mask = 0xFF;
	int result = x & mask;
	
	return !!result;
}

int any_bit_in_msb_equals_0(int x)
{
	int shift_val = (sizeof(int) - 1) << 3;
	int x_right = x >> shift_val;
	int result = x_right & 0xFF;
	
	return !result;
}

int main()
{
	char *format = "result = %d, expected = %d\n";

	printf("any_bit_equals_1:\n");
	printf(format, any_bit_equals_1(0b0001), 1);
	printf(format, any_bit_equals_1(0b000), 0);
	printf(format, any_bit_equals_1(0b1), 1);
	printf(format, any_bit_equals_1(0b1111), 1);
	printf(format, any_bit_equals_1(0b0100), 1);
	printf("\n");

	printf("any_bit_equals_0:\n");
	printf(format, any_bit_equals_0(0b0001), 1);
	printf(format, any_bit_equals_0(0b000), 1);
	printf(format, any_bit_equals_0(0b1), 1);
	printf(format, any_bit_equals_0(0xFFFFFFFF), 0);
	printf(format, any_bit_equals_0(0b0100), 1);
	printf("\n");
	
	printf("any_bit_in_lsb_equals_1:\n");
	printf(format, any_bit_in_lsb_equals_1(0b0001), 1);
	printf(format, any_bit_in_lsb_equals_1(0b000), 0);
	printf(format, any_bit_in_lsb_equals_1(0b1), 1);
	printf(format, any_bit_in_lsb_equals_1(0xFF), 1);
	printf(format, any_bit_in_lsb_equals_1(0xFFFFFF00), 0);
	printf("\n");
	
	printf("any_bit_in_msb_equals_0:\n");
	printf(format, any_bit_in_msb_equals_0(0b0001), 1);
	printf(format, any_bit_in_msb_equals_0(0b000), 1);
	printf(format, any_bit_in_msb_equals_0(0b1), 1);
	printf(format, any_bit_in_msb_equals_0(0xFF), 1);
	printf(format, any_bit_in_msb_equals_0(0xFFFFFF00), 0);
	printf(format, any_bit_in_msb_equals_0(0xFF000000), 0);
	printf("\n");
	
}

