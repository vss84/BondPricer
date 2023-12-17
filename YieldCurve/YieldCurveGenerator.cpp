#include "YieldCurveGenerator.hpp"

YieldCurveGenerator::YieldCurveGenerator(double kappa, double theta, double sigma, double initialRate)
    : kappa(kappa), theta(theta), sigma(sigma), r0(initialRate) {}

// Note(vss): Generates a path of interest rates using the specified model
std::vector<double> YieldCurveGenerator::generateInterestRatePath(int steps, double dt) 
{
    // Random number generation setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> d(0, sqrt(dt));

    std::vector<double> path(steps);
    for (auto& rate : path) 
    {
        double dw = d(gen);
        rate = getNextRate(dt, dw);
    }

    return path;
}

// Note(vss): Creates a yield curve based on the generated interest rate path
std::vector<double> YieldCurveGenerator::createYieldCurve(const std::vector<double>& interestRatePath, int stepsPerYear) 
{
    int numYears = interestRatePath.size() / stepsPerYear;
    std::vector<double> yieldCurve(numYears);

    for (int year = 0; year < numYears; ++year) 
    {
        double sumRates = 0.0;
        for (int day = 0; day < stepsPerYear; ++day) 
        {
            sumRates += interestRatePath[year * stepsPerYear + day];
        }
        yieldCurve[year] = sumRates / stepsPerYear; // Average rate for the year
    }

    return yieldCurve;
}

// Note(vss): Calculates the next interest rate based on the current state and input parameters
double YieldCurveGenerator::getNextRate(double dt, double dw)
{
    double dr = kappa * (theta - r0) * dt + sigma * sqrt(r0) * dw;
    r0 += dr;

    if (r0 < 0)
    {
        r0 = 0;
    }

    return r0;
}
