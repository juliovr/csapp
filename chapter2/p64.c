
/* Return 1 when any odd bit of x equals 1; 0 otherwise.
   Assume w = 32 */
int any_odd_one(unsigned x)
{
	int mask = 0x55555555;
	int result = x & mask;
	return !!result;
}

int main()
{
	char *format = "result = %d, expected = %d\n";
	printf("any_odd_one:\n");
	printf(format, any_odd_one(0b01010101), 1);
	printf(format, any_odd_one(0b00000000), 0);
	printf(format, any_odd_one(0b10101010), 0);
	printf(format, any_odd_one(0b00000001), 1);
	printf(format, any_odd_one(0xFFFFFFFF), 1);
	printf(format, any_odd_one(0x55000000), 1);
}
