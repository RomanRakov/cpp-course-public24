#include <cmath>
#include <functional>
#include <numeric>
#include <vector>
#include <thread>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <string>

class Integral {
private:
    int a, b, n, tn;

public:
    Integral(int& argc, char** argv) {
        a = std::stoi(argv[1]);
        b = std::stoi(argv[2]);
        n = std::stoi(argv[3]);
        tn = std::stoi(argv[4]);
    }

    static double integralFunction(double x) {
        return std::sqrt(1 + std::exp(x));
    }

    double trapezoidalIntegral(double start, double end, int steps, const std::function<double(double)>& f) {
        double h = (end - start) / steps;
        double sum = 0.5 * (f(start) + f(end));
        for (int i = 1; i < steps; ++i) {
            sum += f(start + i * h);
        }
        return sum * h;
    }

    double calculateIntegral() {
        std::vector<std::thread> threads;
        std::vector<double> partialResults(tn, 0.0);

        double h = static_cast<double>(b - a) / n;
        int stepsPerThread = n / tn;

        auto f = integralFunction;

        for (int i = 0; i < tn; ++i) {
            double start = a + i * stepsPerThread * h;
            double end = start + stepsPerThread * h;

            threads.emplace_back([&, i, start, end]() {
                partialResults[i] = trapezoidalIntegral(start, end, stepsPerThread, f);
                });
        }

        for (auto& t : threads) {
            t.join();
        }

        return std::accumulate(partialResults.begin(), partialResults.end(), 0.0);
    }
};

int main(int argc, char** argv)
{
    auto i = Integral(argc, argv);
    std::cout << std::fixed << std::setprecision(4);
    std::cout << i.calculateIntegral() << '\n';
    return 0;
}