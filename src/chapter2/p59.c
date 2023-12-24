
int main()
{
	int mask = 0xFF;
	
	int x = 0x89ABCDEF;
	int y = 0x76543210;
	int result = ((y & ~mask) | (x & mask));

	printf("0x%.2X\n", result);
}

