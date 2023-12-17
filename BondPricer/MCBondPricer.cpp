#include "MCBondPricer.hpp"
#include <chrono>
#include <ctime>
#include <thread>
#include <numeric>


MCBondPricer::MCBondPricer(int paymentFrequency, double couponRate, double faceValue, int yearsRemaining,
    double initialInterestRate, double riskFreeRate, double volatility,
    bool dirtyPrice, unsigned int initialSeed, int trials, bool runParallel)
    : paymentFrequency(paymentFrequency), couponRate(couponRate), faceValue(faceValue),
    yearsRemaining(yearsRemaining), initialInterestRate(initialInterestRate),
    riskFreeRate(riskFreeRate), volatility(volatility), dirtyPrice(dirtyPrice),
    initialSeed(initialSeed), trials(trials), runParallel(runParallel) {}

void MCBondPricer::runSimulations()
{
    std::vector<double> bondPrices(trials, 0.0);
    std::vector<std::thread> threads;

    YieldCurveGenerator yieldCurveGenerator(0.03, riskFreeRate, volatility, initialInterestRate);
    std::mt19937 gen(initialSeed);
    auto start = std::chrono::high_resolution_clock::now();
    double bondPrice = 0.0;

    if (runParallel) 
    {
        // Note(vss): Parallel execution
        for (int i = 0; i < trials; ++i) 
        {
            threads.emplace_back([this, &bondPrices, i]() {
                YieldCurveGenerator localYieldCurveGenerator(0.03, riskFreeRate, volatility, initialInterestRate);
                auto interestRatePath = localYieldCurveGenerator.generateInterestRatePath(yearsRemaining * 252, 1.0 / 252);
                auto yieldCurve = localYieldCurveGenerator.createYieldCurve(interestRatePath, 252);
                bondPrices[i] = calculateBondPrice(yieldCurve);
                });
        }
        for (auto& thread : threads) 
        {
            thread.join();
        }
    }
    else 
    {
        // Note(vss): single-threaded logic
        for (int i = 0; i < trials; ++i)
        {
            auto interestRatePath = yieldCurveGenerator.generateInterestRatePath(yearsRemaining * 252, 1.0 / 252);
            auto yieldCurve = yieldCurveGenerator.createYieldCurve(interestRatePath, 252);
            bondPrice += calculateBondPrice(yieldCurve);
        }
    }

    // Note(vss): Calculate average bond price after simulations
    if (runParallel)
    {
        double totalBondPrice = std::accumulate(bondPrices.begin(), bondPrices.end(), 0.0);
        double threadedAverageBondPrice = totalBondPrice / trials;
        std::cout << "[INFO] Bond Price: " << threadedAverageBondPrice << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        int elapsed_ms = static_cast<int>(elapsed.count());  // Convert to int to truncate decimals
        std::cout << "[INFO] Simulation completed in: " << elapsed_ms << " ms" << std::endl;

    }
    else
    {
        double averageBondPrice = bondPrice / trials;
        std::cout << "[INFO] Bond Price: " << averageBondPrice << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        int elapsed_ms = static_cast<int>(elapsed.count());  // Convert to int to truncate decimals
        std::cout << "[INFO] Simulation completed in: " << elapsed_ms << " ms" << std::endl;

    }
}

int MCBondPricer::calculateDaysSinceLastCoupon() 
{
    // Note(vss): Get the current date from the system
    std::time_t currentTime = std::time(nullptr);
    std::tm currentDate;
    localtime_s(&currentDate, &currentTime);


    // Note(vss): Calculate the month and year of the last coupon payment
    int monthsBetweenPayments = 12 / paymentFrequency;
    int monthOfLastPayment = (currentDate.tm_mon - monthsBetweenPayments + 12) % 12;
    int yearOfLastPayment = currentDate.tm_year + (currentDate.tm_mon < monthOfLastPayment ? -1 : 0);

    // Note(vss): Construct the last coupon date
    std::tm lastCouponDate = {};
    lastCouponDate.tm_year = yearOfLastPayment;
    lastCouponDate.tm_mon = monthOfLastPayment;
    lastCouponDate.tm_mday = 1; // Assuming payments are made on the first of the month
    lastCouponDate.tm_hour = 0;
    lastCouponDate.tm_min = 0;
    lastCouponDate.tm_sec = 0;

    std::time_t lastCouponTime = std::mktime(&lastCouponDate);

    // Note(vss): Calculate days since last coupon payment
    return static_cast<int>(std::difftime(currentTime, lastCouponTime) / (60 * 60 * 24));
}

double MCBondPricer::calculateBondPrice(const std::vector<double>& yieldCurve) 
{
    
    double bondPrice = 0.0;
    double couponPayment = faceValue * couponRate / paymentFrequency;
    int totalPayments = yearsRemaining * paymentFrequency;

    for (int i = 1; i <= totalPayments; ++i) 
    {
        int yearIndex = (i - 1) / paymentFrequency; // Determine which year's yield to use
        double yield = yieldCurve[yearIndex];
        double discountFactor = 1.0 / std::pow(1.0 + yield, i);
        bondPrice += couponPayment * discountFactor;
    }

    bondPrice += faceValue / std::pow(1.0 + yieldCurve.back(), totalPayments);

    if (dirtyPrice) 
    {
        bondPrice += calculateAccruedInterest(calculateDaysSinceLastCoupon());
    }

    return bondPrice;
}

double MCBondPricer::calculateAccruedInterest(int daysSinceLastCoupon) 
{
    double couponPerPeriod = faceValue * couponRate / paymentFrequency;
    int daysPerPeriod = 365;
    return couponPerPeriod * (daysSinceLastCoupon / static_cast<double>(daysPerPeriod));
}
