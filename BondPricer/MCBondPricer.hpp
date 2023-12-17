#ifndef MCBONDPRICER_HPP
#define MCBONDPRICER_HPP

#include "YieldCurveGenerator.hpp"
#include <iostream>
#include <vector>

class MCBondPricer {
public:
    MCBondPricer(int paymentFrequency, double couponRate, double faceValue, int yearsRemaining,
        double initialInterestRate, double riskFreeRate, double volatility,
        bool dirtyPrice = true, unsigned int initialSeed = 44, int trials = 10000, bool runParallel = true);

    void runSimulations();

private:
    int paymentFrequency;
    double couponRate;
    double faceValue;
    int yearsRemaining;
    bool dirtyPrice;

    double initialInterestRate;
    double riskFreeRate;
    double volatility;

    unsigned int initialSeed;
    int trials;
    bool runParallel;

    int calculateDaysSinceLastCoupon();
    double calculateBondPrice(const std::vector<double>& yieldCurve);
    // Note(vss): Accrued interest for dirty price calculation
    double calculateAccruedInterest(int daysSinceLastCoupon);
};

#endif // MCBONDPRICER_HPP
