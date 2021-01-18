#include "matplotlibcpp.h"
#include <cmath>
#include <algorithm>
namespace plt = matplotlibcpp;


double get_max_elem(std::vector<double> v) 
{
    double result = v[0];
    for (double f: v)
        result = f > result ? f : result;
    return result;
}

double get_min_elem(std::vector<double> v) 
{
    double result = v[0];
    for (double f: v)
        result = f < result ? f : result;
    return result;
}



void draw_cusum_chart(std::vector<double> input , double UCL, double LCL)
{
    // Prepare data.
    int n = input.size();
    std::vector<double> x(n), y(n), z(n,UCL), w(n,LCL), cl(n, (UCL+LCL) / 2.0);
    for(int i=0; i<n; ++i) {
        x.at(i) = i;
    }
    // Set the size of output image to 1200x780 pixels
    plt::figure_size(1200, 780);
    // Plot line from given x and y data. Color is selected automatically.
    plt::plot(x, input, "bo");
    plt::plot(x, input, "b");
    // Plot a red dashed line from given x and y data.
    plt::named_plot("UCL", x, w,"r--");
    plt::named_plot("LCL", x, z,"r--");
    plt::named_plot("CL", x, cl,"g--");
    plt::xlim(0,n);
    plt::ylim(UCL + 1.0, LCL - 1.0);
    // Add graph title
    plt::title("CUSUM Chart");
    // Enable legend.
    plt::legend();
    // Save the image (file format is determined by the extension)
    plt::save("./basic.png");
}
//Testing 
/*
int main() 
{
    std::srand(time(NULL));
    std::vector<double> input = {2.03,-0.33,0.78,-0.07,-3.12,3.55,-0.97,-1.66,-3.99,2.42,-0.01,3.48,-1.41,2.16,1.49,-1.6,3.19,1.9,-2.31,0.47};
    draw_cusum_chart(input, 4.0, -4.0);
    return 0;
}
*/
