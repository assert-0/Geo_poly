#include <cstdio>
#include <GeographicLib/Geodesic.hpp>

using GeographicLib::Constants;

GeographicLib::Geodesic g_geodesic(Constants::WGS84_a(), Constants::WGS84_f());

int main()
{
    double la, lo;
    printf("%lf\n", g_geodesic.Direct(45.2372387, 16.6, 0.0, 10000000, la, lo));
}
