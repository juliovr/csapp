
unsigned srl(unsigned x, int k)
{
	/* Perform shift arithmetically */
	unsigned xsra = (int) x >> k;

	int mask = -1 << ((sizeof(int) << 3) - k);

	int result = ~mask & xsra;
}

int sra(int x, int k)
{
	/* Perform shift logically */
	int xsrl = (unsigned) x >> k;

	int mask = -1 << ((sizeof(int) << 3) - k);

	return xsrl | mask;
}

int main()
{
	char *format = "result = 0x%.2X, expected = 0x%.2X\n";
	printf("srl:\n");
	printf(format, srl(-1, 8), 0x00FFFFFF);
	printf("\n");

	printf("sra:\n");
	printf(format, sra(0xFF000000, 8), 0xFFFF0000);
}

