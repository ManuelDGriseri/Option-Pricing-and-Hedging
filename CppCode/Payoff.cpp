#include "pch.h"
#include "Payoff.h"

namespace opt {

    PayoffCall::PayoffCall(double K)
        : K_(K)
    {
        if (K_ < 0.0)
            throw std::invalid_argument("Strike K doit être non-négatif");
    }

    double PayoffCall::operator()(double S) const {
        return std::max<double>(S - K_, 0.0);
    }

    PayoffPut::PayoffPut(double K)
        : K_(K)
    {
        if (K_ < 0.0)
            throw std::invalid_argument("Strike K doit être non-négatif");
    }

    double PayoffPut::operator()(double S) const {
        return std::max<double>(K_ - S, 0.0);
    }

    PayoffDigitCall::PayoffDigitCall(double K)
        : K_(K)
    {
        if (K_ < 0.0)
            throw std::invalid_argument("Strike K doit être non-négatif");
    }

    double PayoffDigitCall::operator()(double S) const {
        return (S > K_) ? 1.0 : 0.0;
    }

    PayoffDigitPut::PayoffDigitPut(double K)
        : K_(K)
    {
        if (K_ < 0.0)
            throw std::invalid_argument("Strike K doit être non-négatif");
    }

    double PayoffDigitPut::operator()(double S) const {
        return (S < K_) ? 1.0 : 0.0;
    }

    PayoffDoubleDigit::PayoffDoubleDigit(double K1, double K2)
		: K1_(K1), K2_(K2)
    {
        if (K1_ < 0.0 || K2_ < 0.0 || K1_ > K2_)
            throw std::invalid_argument("Strike incorrect");
    }

    double PayoffDoubleDigit::operator()(double S) const {
        return (S > K1_ && S < K2_) ? 1.0 : 0.0;
    }

    PayoffBull::PayoffBull(double K1, double K2)
        : K1_(K1), K2_(K2)
    {
        if (K1_ < 0.0 || K2_ < 0.0 || K1_ > K2_)
            throw std::invalid_argument("Strike incorrect");
    }

    double PayoffBull::operator()(double S) const {
        return (S < K1_) ? 0.0 : ((S > K2_) ? K2_ - K1_ : S - K1_);
    }

    PayoffBear::PayoffBear(double K1, double K2)
        : K1_(K1), K2_(K2)
    {
        if (K1_ < 0.0 || K2_ < 0.0 || K1_ > K2_)
            throw std::invalid_argument("Strike incorrect");
    }

    double PayoffBear::operator()(double S) const {
        return (S < K1_) ? K2_ - K1_ : ((S > K2_) ? 0 : K2_ - S);
    }

    PayoffStrangle::PayoffStrangle(double K1, double K2)
        : K1_(K1), K2_(K2)
    {
        if (K1_ < 0.0 || K2_ < 0.0 || K1_ > K2_)
            throw std::invalid_argument("Strike incorrect");
    }

    double PayoffStrangle::operator()(double S) const {
        return (S < K1_) ? K1_ - S : ((S > K2_) ? S - K2_ : 0);
    }

    PayoffButterfly::PayoffButterfly(double K1, double K2)
        : K1_(K1), K2_(K2)
    {
        if (K1_ < 0.0 || K2_ < 0.0 || K1_ > K2_)
            throw std::invalid_argument("Strike incorrect");
    }

    double PayoffButterfly::operator()(double S) const {
        return (S > K1_ && S <= 0.5 * (K1_ + K2_)) ? S - K1_ : ((S > 0.5 * (K1_ + K2_) && S < K2_) ? K2_ - S : 0);
    }

} // namespace opt
