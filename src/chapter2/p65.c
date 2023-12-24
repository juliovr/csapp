
/* Return 1 when x contains an odd number of 1s; 0 otherwise.
   Assume w = 32 */
int odd_ones(unsigned x)
{
	x ^= (x >> 16);
	x ^= (x >> 8);
	x ^= (x >> 4);
	x ^= (x >> 2);
	x ^= (x >> 1);

	return x & 1;
}

int main()
{
	char *format = "result = %d, expected = %d\n";
	printf(format, odd_ones(0b1100), 0);
	printf(format, odd_ones(0b1110), 1);
	printf(format, odd_ones(0b11), 0);
	printf(format, odd_ones(0b100), 1);
	printf(format, odd_ones(0b1), 1);
	printf(format, odd_ones(0x80000000), 1);
	printf(format, odd_ones(0x80000080), 0);
}

