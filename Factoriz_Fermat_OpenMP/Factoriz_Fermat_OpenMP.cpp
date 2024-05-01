#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <ctime>
#include <omp.h>
using namespace std;

vector<int> fermat_factorization(int n)
{
    vector<int> factors;
    if (n % 2 == 0)
    {
        if (n / 2 != 1)
        {
            factors.push_back(2);
            vector<int> temp = fermat_factorization(n / 2);
            factors.insert(factors.end(), temp.begin(), temp.end());
        }
        else
            factors.push_back(2);
        return factors;
    }

    int a = ceil(sqrt(n));
    int b2 = a * a - n;
    while (sqrt(b2) != static_cast<int>(sqrt(b2)))
    {
        a++;
        b2 = a * a - n;
    }

    int factor1 = a + static_cast<int>(sqrt(b2));
    int factor2 = a - static_cast<int>(sqrt(b2));

    if (factor1 == 1)
        factors.push_back(factor2);
    else if (factor2 == 1)
        factors.push_back(factor1);
    else
    {
        vector<int> temp1 = fermat_factorization(factor1);
        vector<int> temp2 = fermat_factorization(factor2);
        factors.insert(factors.end(), temp1.begin(), temp1.end());
        factors.insert(factors.end(), temp2.begin(), temp2.end());
    }

    return factors;
}

vector<int> read_input(const string& FILE)
{
    vector<int> random_numbers;
    ifstream file(FILE);
    if (file.is_open())
    {
        int number;
        while (file >> number)
            random_numbers.push_back(number);
        file.close();
    }
    return random_numbers;
}

int main()
{
    const string FILE_INPUT = "random_numbers1.txt";
    vector<int> random_numbers = read_input(FILE_INPUT);

    int start_time = clock();

#pragma omp parallel for schedule(static) num_threads(4)
    for (int i = 0; i < random_numbers.size(); i++)
    {
        int number = random_numbers[i];
        vector<int> factors = fermat_factorization(number);

#pragma omp critical
        {
            cout << "Thread " << omp_get_thread_num() << ": " << number << ": ";
            for (int factor : factors)
                cout << factor << " ";
            cout << "\n";
        }
    }

    int end_time = clock();
    double run_time = double(end_time - start_time) / 1000;
    cout << "\nTotal run time: " << run_time << " seconds.\n";

    return 0;
}