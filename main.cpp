/**
 * Author: Flores, Facundo Gabriel
 * Description: Se muestra distintas gráficas de frecuencias
 * 		según valores generados por las diferentes distribuciones
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

#define LAMBDA 2

/***************** normal defs *******************/
#define MU 0
#define THAO 1
#define TCL_MAX 12
/*************************************************/

/****************** erlang defs ******************/
#define K 1
#define BETHA 2
/*************************************************/

/****************** weibull defs *****************/
#define ALPHA_W 1
#define BETHA_W 0.5
/*************************************************/

//Return a random number between 0 and 1
double __pref__rand()
{
    return (double)rand() / (double)RAND_MAX;
}

//Generate numbers exp. dist. using the inverse method
double dExp_Inverse(const double lambda)
{
    double mu = __pref__rand();
    return -(log(mu) / lambda);
}

//Generate numbers exp. dist. usign the Vonn Neumman method
double dExp_VN(const double lambda)
{
    int k = 0;
    int ind = 2;
    bool exit = false;
    std::pair<double, double> pPair = std::make_pair(__pref__rand(), __pref__rand());
    double p = pPair.first;
    do{
	
	while(pPair.first > pPair.second)
	{
	    pPair.first = pPair.second; //u = v
	    pPair.second = __pref__rand();
	    ind++;
	}
	
	if(ind % 2 == 0) exit = true;
	else{
	    k++;
	    pPair = std::make_pair(__pref__rand(), __pref__rand());
	    p = pPair.first;
	    ind = 2;
	}
    }while(!exit);
    return (k + p) / lambda;
}

//Generate numbers normal dist. using the TCL method
double dNormal_TCL(const double mu, const double thao, const long n)
{
    double u = .0f;
    for(long i = 0; i < n; i++)
	u += __pref__rand();
    double t = u - n / 2;
    double normal = t * thao + mu;
    return normal;
}

//Generate numbers normal dist. using the Polar method
double dNormal_Polar(const double mu, const double thao)
{
    //(r,theta)
    std::pair<double, double> pPair = std::make_pair(sqrt(dExp_Inverse(.5f)), (double)(__pref__rand() * 2 * M_PI));
    return pPair.first * cos(pPair.second);
}

//Generate numbers erlang dist. 
double dErlang(double k, double betha)
{
    double p = 1;
    for(long i = 0; i < (long)(k); i++)
	p *= __pref__rand();
    return -log(p) / betha;    
}

//Generate numbers weibull dist.
double dWeibull(double alpha, double betha)
{
    return betha * pow(-log(__pref__rand()), 1.0f / alpha);
}


//Make a histogram base on pFunc
std::vector<int> Histogram(double(*pFunc)(double, double), const double param1, const double param2, const int nrolls, const int nintervals)
{
    int p[nintervals];
    memset(p, 0, nintervals * sizeof(int));
    for(int i = 0; i < nrolls; i++)
    {
	double number = pFunc(param1, param2);
	if(number < 1.0) ++p[int(nintervals * number)];
    }
    std::vector<int> v(p, p + sizeof(p) / sizeof(p[0]));
    return v;
}

//Make a histogram base on pFunc
std::vector<int> Histogram(double(*pFunc)(double, double, long), const double param1, const double param2, const long param3, const int nrolls, const int nintervals)
{
    int p[nintervals];
    memset(p, 0, nintervals * sizeof(int));
    for(int i = 0; i < nrolls; i++)
    {
	double number = pFunc(param1, param2, param3);
	if(number < 1.0) ++p[int(nintervals * number)];
    }
    std::vector<int> v(p, p + sizeof(p) / sizeof(p[0]));
    return v;
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
    std::vector<int> histo;
    
    std::cout << "==========EXP_INVERSE(2)==========" << std::endl;
    histo = Histogram(dExp_Inverse, LAMBDA, ROLLS, INTER);
    drawHistogram(histo, ROLLS, INTER);
    histo.clear();
    std::cout << "==========EXP_VN(2)==========" << std::endl;
    histo = Histogram(dExp_VN, LAMBDA, ROLLS, INTER);
    drawHistogram(histo, ROLLS, INTER);
    histo.clear();
    std::cout << "==========NORMAL_POLAR(0, 1)==========" << std::endl;
    histo = Histogram(dNormal_Polar, MU, THAO, ROLLS, INTER);
    drawHistogram(histo, ROLLS, INTER);
    histo.clear();
    std::cout << "==========ERLANG(1,2)==========" << std::endl;
    histo = Histogram(dErlang, K, BETHA, ROLLS, INTER);
    drawHistogram(histo, ROLLS, INTER);
    histo.clear();
    std::cout << "==========WEIBULL(1,0.5)==========" << std::endl;
    histo = Histogram(dWeibull, ALPHA_W, BETHA_W, ROLLS, INTER);
    drawHistogram(histo, ROLLS, INTER);
    histo.clear();    
    return 0;
}
