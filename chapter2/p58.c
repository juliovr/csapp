
int is_little_endian()
{
	int x = 1;
	void *pv = (void *)&x;
	int *pi = (int *)pv;

	return (*pi == 0x1);
}

int main(void)
{
	printf("is_little_endian = %d\n", is_little_endian());
}
