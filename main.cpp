#include "MCBondPricer.hpp"
#include <iostream>

int main() 
{
    // Note(vss): User inputs for bond pricing parameters.
    int paymentFrequency;
    double couponRate;
    double faceValue;
    int yearsRemaining;
    double initialInterestRate;
    double riskFreeRate;
    double volatility;

    // Note(vss): Defaults for optional simulation parameters.
    bool dirtyPrice = true;
    unsigned int initialSeed = 539;
    int trials = 10000;
    bool runParallel = true;
    

    std::cout << "Enter payment frequency (1=Annually, 2=Bi-Annually, 4=Quarterly, 12=Monthly): ";
    std::cin >> paymentFrequency;

    std::cout << "Enter coupon rate (as a decimal, e.g., 0.05 for 5%): ";
    std::cin >> couponRate;

    std::cout << "Enter face value: ";
    std::cin >> faceValue;

    std::cout << "Enter years remaining: ";
    std::cin >> yearsRemaining;

    std::cout << "Enter initial interest rate (Annual rate as a decimal): ";
    std::cin >> initialInterestRate;

    std::cout << "Enter risk-free rate (Annual rate as a decimal): ";
    std::cin >> riskFreeRate;

    std::cout << "Enter volatility (Annual rate as a decimal): ";
    std::cin >> volatility;

    // Note(vss): optional parameters
    std::cout << "Include dirty price in calculation? (1 for Yes, 0 for No): ";
    std::cin >> dirtyPrice;

    std::cout << "Enter seed for random number generator: ";
    std::cin >> initialSeed;

    std::cout << "Enter number of trials for simulation: ";
    std::cin >> trials;

    std::cout << "Run parallel? (1 for Yes, 0 for No): ";
    std::cin >> runParallel;

    MCBondPricer mcBondPricer(paymentFrequency, couponRate, faceValue, yearsRemaining,
        initialInterestRate, riskFreeRate, volatility, dirtyPrice, initialSeed, trials, runParallel);

    mcBondPricer.runSimulations();

    return 0;
}
