#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <vector>
#include <chrono>
#include <thread>

#include <windows.h>

#include <GeographicLib/Geodesic.hpp>

#define PRINT_SEPARATOR printf("----------------------------\n")

using GeographicLib::Constants;


//DMS conversion struct
struct DMS
{
    double d;
    double m;
    double s;

    void toDMS(double in)
    {
        d = int(in);
        m = int((in - d) * 60.0);
        s = int((in - d - (m / 60.0)) * 3600);
        if(round(s) >= 60.0) {s = 0.0; m += 1.0;}
        if(round(m) >= 60.0) {m = 0.0; d += 1.0;}
    }

    double toFloat(){ return d + m / 60.0 + s / 3600.0; }
};


    //globals//


//geodesic calculations object
GeographicLib::Geodesic g_geodesic(Constants::WGS84_a(), Constants::WGS84_f());

//input and output variables
DMS g_laIn, g_loIn;
double g_radius;
int g_numPoints;
std::vector< std::pair<DMS, DMS> > g_coords;

//file name and handle
const char *g_outFileName = "coords.csv";
FILE *g_file;


    //functions//


void input()
{
    PRINT_SEPARATOR;
    printf("Enter the coordinates of the polygon center, its radius(km) and the number of points.\n");
    printf("format: xxD yyM zz.zz...S N xxD yyM zz.zz...S E rr.rr... R nn... N\n");
    printf("example: 27D 35M 17.178S N 56D 48M 12.134654S E 150.18 R 8 N\n\n");

    scanf
    (
        "%lfD %lfM %lfS N %lfD %lfM %lfS E %lf R %d N",
        &g_laIn.d, &g_laIn.m, &g_laIn.s,
        &g_loIn.d, &g_loIn.m, &g_loIn.s,
        &g_radius, &g_numPoints
    );
	
	g_laIn.toDMS(g_laIn.toFloat());
	g_loIn.toDMS(g_loIn.toFloat());

    fflush(stdin);

    PRINT_SEPARATOR;

    g_coords.resize(g_numPoints);
    g_radius *= 1000.0;
}

void process()
{
    double la1, lo1, la2, lo2, heading = 0.0;

    la1 = g_laIn.toFloat();
    lo1 = g_loIn.toFloat();

    printf("Processing...\n");

    for(int a = 0; a < g_numPoints; a++)
    {
        g_geodesic.Direct(la1, lo1, heading, g_radius, la2, lo2);
        g_coords[a].first.toDMS(la2);
        g_coords[a].second.toDMS(lo2);
        heading += floor(360.0 / (double)g_numPoints);
    }

    printf("Done processing!\n");
}

void output()
{
    g_file = fopen(g_outFileName, "w");

    while(g_file == nullptr)
    {
        printf("Error opening output csv file.\n");
        printf("Please check if the file is open in another program, close the program and try again.\n");
        system("pause");
        g_file = fopen(g_outFileName, "w");
    }

    printf("Saving results...\n");

    fprintf(g_file, "radius\n%g km\n", g_radius / 1000.0);
    fprintf(g_file, "center coordinates\n");
    fprintf(g_file, "d,m,s,,d,m,s\n");
    fprintf
    (
        g_file,
        "%.0lf,%.0lf,%.0lf,,%.0lf,%.0lf,%.0lf\n\n\n",
        g_laIn.d, g_laIn.m, g_laIn.s,
        g_loIn.d, g_loIn.m, g_loIn.s
    );

    fprintf(g_file, "vertex coordinates\n");
    fprintf(g_file, "d,m,s,,d,m,s\n");
    for(int a = 0; a < g_numPoints; a++)
    {
        fprintf
        (
            g_file,
            "%.0lf,%.0lf,%.0lf,,%.0lf,%.0lf,%.0lf\n",
            g_coords[a].first.d,
            g_coords[a].first.m,
            round(g_coords[a].first.s),
            g_coords[a].second.d,
            g_coords[a].second.m,
            round(g_coords[a].second.s)
        );
    }

    printf("Results successfully saved in file %s!\n", g_outFileName);
    printf("Done!\n");

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    PRINT_SEPARATOR;
    fclose(g_file);
}

int main()
{
    while(1)
    {
        input();
        process();
        output();
    }
}
