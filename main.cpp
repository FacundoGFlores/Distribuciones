/**
 * Author: Flores, Facundo Gabriel
 * Description: Se muestra el histograma de la distribución exponencial 
 * 		según valores generados por el método del inverso
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>

#define PATH "histogram.txt"
#define MAX_HEIGHT 100 // Maximun frecuency for showing
#define ROLLS 100000 //Simulation
#define INTER 10 //Intervals
#define LAMBDA 3.0f

//Return a random number between 0 and 1
double __pref__rand()
{
    return (double)rand() / (double)RAND_MAX;
}

//Generate numbers exp. dist. using the inverse method
double dExp_Inverse(double lambda)
{
    double mu = __pref__rand();
    return -(log(mu) / lambda);
}

//Make a histogram base on pFunc
std::vector<int> Histogram(double(*pFunc)(double), const double param, const int nrolls, const int nintervals)
{
    int p[nintervals];
    memset(p, 0, nintervals * sizeof(int));
    for(int i = 0; i < nrolls; i++)
    {
	double number = pFunc(param);
	if(number < 1.0) ++p[int(nintervals * number)];
    }
    std::vector<int> v(p, p + sizeof(p) / sizeof(p[0]));
    return v;
}

//Write a histogram into a file
void writeHistogram(std::vector<int> &vec)
{
    std::ofstream fs(PATH);
    for(int i = 0; i < vec.size(); i++)
	fs << i << "\t" << vec[i] << std::endl;
    fs.close();
}

//Draw a histogram in a console
void drawHistogram(std::vector<int> &vec, const int nrolls, const int nintervals)
{
    std::cout << std::fixed; std::cout.precision(1);
    for(int i = 0; i < nintervals; ++i)
    {	
	std::cout << float(i)/nintervals << "-" << float(i+1)/nintervals << ": "; 
	std::cout << std::string(vec[i]*MAX_HEIGHT/nrolls,'*') << std::endl;
    }
    
}

int main(int argc, char **argv) {

    srand(std::time(NULL));
    
    std::vector<int> histo = Histogram(dExp_Inverse, LAMBDA, ROLLS, INTER);
    writeHistogram(histo);
    drawHistogram(histo, ROLLS, INTER);
    
    return 0;
}
