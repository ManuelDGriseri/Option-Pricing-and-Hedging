#include "pch.h"

#ifndef CNMETHOD_H
#define CNMETHOD_H

#include "ImplicitScheme.h"

namespace pde {

    /**
     * @brief Méthode de Crank–Nicolson pour le schéma implicite aux différences finies.
     * @tparam TPDE Type d'EDP.
     */
	template<typename TPDE>
    class CNMethod : public ImplicitScheme<TPDE> {
    public:
        CNMethod(const TPDE& pde, int imax, int jmax)
            : ImplicitScheme<TPDE>(pde, imax, jmax) 
        {
        }

        double A(int i, int j) const override {
            return 0.5 * this->dt_ * (this->b(i - 0.5, j) / 2.0 - this->a(i - 0.5, j) / this->dS_) / this->dS_;
        }

        double B(int i, int j) const override {
            return 1.0 + 0.5 * this->dt_ * (2.0 * this->a(i - 0.5, j) / (this->dS_ * this->dS_) - this->c(i - 0.5, j));
        }

        double C(int i, int j) const override {
            return -0.5 * this->dt_ * (this->b(i - 0.5, j) / 2.0 + this->a(i - 0.5, j) / this->dS_) / this->dS_;
        }

        double D(int i, int j) const override {
            return -this->dt_ * this->d(i - 0.5, j);
        }

        double E(int i, int j) const override {
            return -A(i, j); 
        }

        double F(int i, int j) const override {
            return  2.0 - B(i, j); 
        }

        double G(int i, int j) const override {
            return -C(i, j); 
        }
    };

} // namespace pde

#endif // CNMETHOD_H
