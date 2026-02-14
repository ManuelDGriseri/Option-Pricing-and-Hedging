#include "pch.h"
#include "Volatility.h"

namespace pde {

    BSVol::BSVol(double sigma)
        : sigma_(sigma)
    {
        if (sigma_ < 0.0)
            throw std::invalid_argument("Sigma doit être >= 0");
    }

    double BSVol::operator()(double t, double S) const {
        return sigma_;
    }

    LocalVol::LocalVol(double alfa, double beta)
        : alfa_(alfa), beta_(beta)
    {
        if (alfa_ < 0.0 || beta_ < 0.0)
            throw std::invalid_argument("Coefficient doit être >= 0");
    }

    double LocalVol::operator()(double t, double S) const {
        return (alfa_ / (t + 1)) + (beta_ / (S + 1));
    }

} // namespace pde