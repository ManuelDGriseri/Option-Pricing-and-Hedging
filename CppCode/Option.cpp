#include "pch.h"
#include "Option.h"

namespace crr {

    Option::Option(double S0, double R, double sigma, double T, int N)
        : S0_(S0), R_(R), sigma_(sigma), T_(T), N_(N)
    {
        if (S0_ < 0.0)    throw std::invalid_argument("S0 doit être >= 0");
        if (sigma_ < 0.0) throw std::invalid_argument("Sigma doit être >= 0");
        if (T_ < 0.0)     throw std::invalid_argument("T doit être >= 0");
        if (N_ <= 0)      throw std::invalid_argument("N doit être > 0");

        double dt = T_ / N_;
        double sq = sigma_ * std::sqrt(dt);
        double rN = R_ * dt;
        u_ = rN + sq;
        d_ = rN - sq;
        discount_ = 1.0 / (1.0 + rN);
    }

    double Option::price() const {
        return treePrice()[0][0];
    }

    double Option::deltaZero() const {
        return hedgingStrategy().delta[0][0];
    }

} // namespace crr
