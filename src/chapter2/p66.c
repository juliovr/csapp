
int leftmost_one(unsigned x)
{
	// Transform the binary into [0...01...1] form
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
	x |= (x >> 16);

	// The shifted and not make the shifted slot the rightmost 1
	return (x & ~(x >> 1));
}

int main()
{
	char *format = "result = 0x%.2x, expected = 0x%.2x\n";
	printf(format, leftmost_one(0xFF), 0x80);
	printf(format, leftmost_one(0x66), 0x40);
	printf(format, leftmost_one(0x4), 0x4);
	printf(format, leftmost_one(0xF), 0x8);
}
