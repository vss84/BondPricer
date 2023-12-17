# Monte Carlo Bond Pricer

This Monte Carlo simulation, recognizing the uncertainty of the yield curve, calculates a bond's price by producing tens of thousands of individual yield curves. Each bond is priced using a distinct yield curve, and the resultant prices are then averaged to determine a "fair" value. The simulation utilizes the concurrency capabilities of C++ to efficiently generate these yield curves, with options for parallel or sequential processing.

# Example - Pricing Apple 16/36

Utilizing the Monte Carlo simulation to calculate the price of Apple's 16/36 bond issuance (US037833BW97) results in a value that closely aligns with its existing market price. This outcome suggests, based solely on the Monte Carlo simulation, that the bond seems appropriately priced in the current market.


**Simulation Results** Based on 10,000 yield curves.
![alt text](https://github.com/vss84/BondPricer/images/blob/master/simulationprice.png?raw=true)

**Note:** The image provided below does not directly stem from the simulation. Rather, it is a representation sourced from the web, depicting the market price of Apple's 16/36 bond issuance. This is to facilitate a comparison between the simulated results, as previously described, and the actual trading price of Apple's bond on that specific day.

![alt text](https://github.com/vss84/BondPricer/images/blob/master/applebondprice.png?raw=true)


# Simulation Parameters
Simulation parameters such as volatility, interest rates, dirty price enabler, can be changed when building the bond pricer (MCBondPricer).

```
MCBondPricer:
    Mandatory Parameters:
        Payment frequency,      // Annually, Bi-Annually, Quarterly, Monthly
        couponRate,             // Annual rate
        faceValue,              // 1'000
        yearsRemaining,
        initialInterestRate,    // Annual rate
        riskFreeRate,           // Annual rate, for stochastic process
        volatility              // Annual rate, for stochastic process
    Optional (Default) Parameters: dirtyPrice (1), initialSeed (97),
             trials (10,000), runParallel (0)
```