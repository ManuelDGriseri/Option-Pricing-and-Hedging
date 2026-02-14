#include "pch.h"

#ifndef DIFFUSION_H
#define DIFFUSION_H

#include "ParabPDE.h"
#include <cmath>

namespace pde {

    /**
     * @brief EDP d'un modèle de diffusion.
     * @tparam TPayoff Type de payoff.
     * @tparam TVol Type de volatilité.
     */
    template<typename TPayoff, typename TVol>
    class Diffusion : public ParabPDE {
    private:
        TPayoff payoff_;
        TVol vol_;
        double R_;

    public:
        Diffusion(double T, double Smin, double Smax, double R, const TPayoff& payoff, const TVol& vol)
			: ParabPDE(T, Smin, Smax), R_(R), payoff_(payoff), vol_(vol)
        {
        }

        double a(double t, double S) const override {
            return -0.5 * std::pow(vol_(t, S) * S, 2.0);
        }

        double b(double t, double S) const override {
            return -R_ * S;
        }

        double c(double t, double S) const override {
            return R_;
        }
        
        double d(double t, double S) const override {
            return 0;
        }

        double Terminal(double S) const override {
            return payoff_(S);
        }

        double Lower(double t) const override {
            return payoff_(Smin_) * std::exp(-R_ * (T_ - t));
        }

        double Upper(double t) const override {
            return payoff_(Smax_) * std::exp(-R_ * (T_ - t));
        }
    };

} // namespace pde

#endif // DIFFUSION_H
