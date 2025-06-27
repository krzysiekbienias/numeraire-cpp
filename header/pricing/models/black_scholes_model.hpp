# pragma once

class BlackScholesModel{
public :
    double priceVanilla(double S, double K, double r, double sigma, double tau, bool isCall) const;
    
    double priceDigital(double S, double K, double r, double sigma, double tau, double cashPayoff, bool isCall) const;
    double assetOrNothing(double S, double K, double r, double sigma, double tau, bool isCall) const;
};
