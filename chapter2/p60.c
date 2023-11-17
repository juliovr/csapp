
unsigned replace_byte(unsigned x, int i, unsigned char b)
{
#if 0
	int bytes_to_shift = (i*8);
	int mask = (0xFF << bytes_to_shift);
	unsigned result = (x & ~mask) | (b << bytes_to_shift);
	return result;
#endif

	unsigned char *p = (unsigned char *)&x;
	p[i] = b;

	return x;
}

int main()
{
	unsigned a = replace_byte(0x12345678, 2, 0xAB);
	printf("result = 0x%.2X, expected = 0x%.2X\n", a, 0x12AB5678);

	unsigned b = replace_byte(0x12345678, 0, 0xAB);
	printf("result = 0x%.2X, expected = 0x%.2X\n", b, 0x123456AB);
}
