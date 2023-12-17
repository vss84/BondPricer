#ifndef YIELDCURVEGENERATOR_HPP
#define YIELDCURVEGENERATOR_HPP

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

// Class for generating yield curves using Monte Carlo simulations.
// This class uses a stochastic process to generate paths of interest rates and creates yield curves based on these paths.
class YieldCurveGenerator {
public:
    // Constructor initializes the model parameters for the yield curve generation.
    // kappa: Mean reversion speed.
    // theta: Long-term mean rate.
    // sigma: Volatility of interest rate changes.
    // initialRate: Initial short-term interest rate.
    YieldCurveGenerator(double kappa, double theta, double sigma, double initialRate);

    // Generates a path of interest rates using a stochastic process.
    // steps: Number of steps in the simulation.
    // dt: Time increment for each step.
    std::vector<double> generateInterestRatePath(int steps, double dt);

    // Creates a yield curve based on the generated path of interest rates.
    // interestRatePath: Vector of interest rates generated by generateInterestRatePath.
    // stepsPerYear: Number of steps per year, used for annualizing the rates.
    std::vector<double> createYieldCurve(const std::vector<double>& interestRatePath, int stepsPerYear);

private:
    double kappa;    // Mean reversion speed
    double theta;    // Long-term mean rate
    double sigma;    // Volatility
    double r0;       // Initial rate

    // Computes the next interest rate in the path.
    // dt: Time increment for the step.
    // dw: Random increment from the Wiener process.
    double getNextRate(double dt, double dw);
};

#endif // YIELDCURVEGENERATOR_HPP