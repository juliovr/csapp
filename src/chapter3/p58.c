
long decode2(long x, long y, long z)
{
    long temp = y - z;

    return ((y << 63) >> 63) ^ (x * temp);
}
