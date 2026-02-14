#ifndef IMPLICITSCHEME_H
#define IMPLICITSCHEME_H

#include "FDMethod.h"

namespace pde {

    /**
     * @brief Schéma implicite aux différences finies.
     * @tparam TPDE Type d'EDP.
     */
    template<typename TPDE>
    class ImplicitScheme : public FDMethod<TPDE> {
    public:
        ImplicitScheme(const TPDE& pde, int imax, int jmax);

        /**
         * @brief Coefficients de l’équation discrétisée.
         */
        virtual double A(int i, int j) const = 0;
        virtual double B(int i, int j) const = 0;
        virtual double C(int i, int j) const = 0;
        virtual double D(int i, int j) const = 0;
        virtual double E(int i, int j) const = 0;
        virtual double F(int i, int j) const = 0;
        virtual double G(int i, int j) const = 0;

        /**
         * @brief Résolution d’un système tridiagonal par l’algorithme de Thomas.
         * @return Vecteur solution du système.
         */
        std::vector<double> LUDecomposition(int i, std::vector<double> q) const;

        /**
         * @brief Calcul de la matrice de solution V.
         */
        void SolvePDE();

        std::vector<double> w(int i) const;
        std::vector<double> A(int i, std::vector<double> q) const;
    };

    template<typename TPDE>
    ImplicitScheme<TPDE>::ImplicitScheme(const TPDE& pde, int imax, int jmax)
        : FDMethod<TPDE>(pde, imax, jmax)
    {
    }

    template<typename TPDE>
    std::vector<double> ImplicitScheme<TPDE>::w(int i) const {
        std::vector<double> w(this->jmax_ + 1);
        w[1] = D(i, 1) + A(i, 1) * this->fl(i) - E(i, 1) * this->fl(i - 1);
        for (int j = 2; j < this->jmax_ - 1; j++)
            w[j] = D(i, j);
        w[this->jmax_ - 1] = D(i, this->jmax_ - 1) + C(i, this->jmax_ - 1) 
                            * this->fu(i) - G(i, this->jmax_ - 1) * this->fu(i - 1);
        return w;
    }

    template<typename TPDE>
    std::vector<double> ImplicitScheme<TPDE>::A(int i, std::vector<double> q) const {
        std::vector<double> p(this->jmax_ + 1);
        p[1] = B(i, 1) * q[1] + C(i, 1) * q[2];
        for (int j = 2; j < this->jmax_ - 1; j++)
            p[j] = A(i, j) * q[j - 1] + B(i, j) * q[j] + C(i, j) * q[j + 1];
        p[this->jmax_ - 1] = A(i, this->jmax_ - 1) * q[this->jmax_ - 2] + B(i, this->jmax_ - 1) * q[this->jmax_ - 1];
        return p;
    }

    template<typename TPDE>
    std::vector<double> ImplicitScheme<TPDE>::LUDecomposition(int i, std::vector<double> q) const {
        std::vector<double> p(this->jmax_ + 1), r(this->jmax_ + 1), y(this->jmax_ + 1);
        r[1] = F(i, 1);      
        y[1] = q[1];
        for (int j = 2; j < this->jmax_; j++) {
            r[j] = F(i, j) - E(i, j) * G(i, j - 1) / r[j - 1];
            y[j] = q[j] - E(i, j) * y[j - 1] / r[j - 1];
        }
        p[this->jmax_ - 1] = y[this->jmax_ - 1] / r[this->jmax_ - 1];

        for (int j = this->jmax_ - 2; j > 0; j--)
            p[j] = (y[j] - G(i, j) * p[j + 1]) / r[j];
        return p;
    }

    template<typename TPDE>
    void ImplicitScheme<TPDE>::SolvePDE()
    {
        for (int j = 0; j <= this->jmax_; j++)
            this->V[this->imax_][j] = this->f(j);

        for (int i = this->imax_; i > 0; i--)
        {
            auto pvec = this->A(i, this->V[i]);   
            auto wvec = this->w(i);               
            for (int j = 0; j <= this->jmax_; j++)
                pvec[j] += wvec[j];
            this->V[i - 1] = LUDecomposition(i, pvec);
            this->V[i - 1][0] = this->fl(i - 1);
            this->V[i - 1][this->jmax_] = this->fu(i - 1);
        }
    }

} // namespace pde

#endif // IMPLICITSCHEME_H