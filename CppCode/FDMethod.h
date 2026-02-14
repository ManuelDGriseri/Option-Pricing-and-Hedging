#ifndef FDMETHOD_H
#define FDMETHOD_H

#include <vector>
#include <stdexcept>

namespace pde{

    /**
     * @brief Méthodes aux différences finies pour la résolution numérique d'EDP.
     * @tparam TPDE Type d'EDP.
     */
    template<typename TPDE>
    class FDMethod {
    protected:
        TPDE pde_;
		int imax_, jmax_;                    ///< Nombre de pas en temps et en prix
        double dt_, dS_;                     ///< Pas en temps et en prix
        std::vector<std::vector<double>> V;  ///< Matrice de solution

    public:
        FDMethod(const TPDE& pde, int imax, int jmax);

        double t(double i) const { return dt_ * i; }
        double S(int j) const { return pde_.Smin() + dS_ * j; }

        double a(double i, int j) const { return pde_.a(t(i), S(j)); }
        double b(double i, int j) const { return pde_.b(t(i), S(j)); }
        double c(double i, int j) const { return pde_.c(t(i), S(j)); }
        double d(double i, int j) const { return pde_.d(t(i), S(j)); }

        double f(int j) const { return pde_.Terminal(S(j)); }
        double fu(int i) const { return pde_.Upper(t(i)); }
        double fl(int i) const { return pde_.Lower(t(i)); }

        /**
         * @brief Prix de l’option par interpolation bilinéaire.
         * @return v(t,S).
         */
        double v(double t, double S) const;

        /**
         * @brief Delta de l’option par schéma aux différences centrales.
         * @return delta(t,S).
         */
        double delta(double t, double S) const;

        /**
         * @brief Grille : prix et delta.
         */
        struct Grid {
            std::vector<std::vector<double>> val;  ///< Prix.
            std::vector<std::vector<double>> del;  ///< Delta.
        };

        /**
         * @brief Génère un maillage fixe de 11×11 points pour (t,S).
         * @return Matrices 11×11 contenant v(t_i,S_j) et delta(t_i,S_j).
         */
        Grid grid() const;
    };

    template<typename TPDE>
    FDMethod<TPDE>::FDMethod(const TPDE& pde, int imax, int jmax)
		: pde_(pde), imax_(imax), jmax_(jmax)
    {
        if (imax_ <= 0) throw std::invalid_argument("Nt doit être >= 1");
        if (jmax_ <= 1) throw std::invalid_argument("NS doit être >= 2");
        
        dS_ = (pde_.Smax() - pde_.Smin()) / jmax_;
        dt_ = pde_.T() / imax_;
        V.resize(imax_ + 1);
        for (int i = 0; i <= imax_; i++)
            V[i].resize(jmax_ + 1);
    }

    template<typename TPDE>
    double FDMethod<TPDE>::v(double t, double S) const {
        if (t < 0 || t > pde_.T())
            throw std::out_of_range("t hors du domaine");
        if (S < pde_.Smin() || S > pde_.Smax())
            throw std::invalid_argument("S hors du domaine");
        if (S == pde_.Smax())
            return pde_.Upper(t);
        if (t == pde_.T())
            return pde_.Terminal(S);

        int i = (int)(t / dt_);
        int j = (int)((S - pde_.Smin()) / dS_);
        double l1 = (t - FDMethod<TPDE>::t(i)) / dt_, l0 = 1.0 - l1;
        double w1 = (S - FDMethod<TPDE>::S(j)) / dS_, w0 = 1.0 - w1;
        return  l1 * w1 * V[i + 1][j + 1] + l1 * w0 * V[i + 1][j]
                + l0 * w1 * V[i][j + 1] + l0 * w0 * V[i][j];
    }

    template<typename TPDE>
    double FDMethod<TPDE>::delta(double t, double S) const {
        if (t < 0 || t > pde_.T()) 
            throw std::out_of_range("t hors du domaine");
        if (S < pde_.Smin() || S > pde_.Smax()) 
            throw std::invalid_argument("S hors du domaine");

        double dlt;
        int i = (int)(t / dt_);
        int j = (int)((S - pde_.Smin()) / dS_);
        if (j == 0)
            dlt = (V[i][1] - V[i][0]) / dS_;
        else if (j == jmax_)
            dlt = (V[i][jmax_] - V[i][jmax_ - 1]) / dS_;
        else
            dlt = (V[i][j + 1] - V[i][j - 1]) / (2.0 * dS_);
        return std::min<double>(1.0, std::max<double>(-1.0, dlt));
    }

    template<typename TPDE>
    typename FDMethod<TPDE>::Grid FDMethod<TPDE>::grid() const {
        int N = 10;
        std::vector<double> tvals(N + 1), Svals(N + 1);
        for (int i = 0; i <= N; ++i) 
            tvals[i] = pde_.T() * i / double(N);
        for (int j = 0; j <= N; ++j) 
            Svals[j] = pde_.Smin() + (pde_.Smax() - pde_.Smin()) * j / double(N);

        Grid M;
        M.val.assign(N + 1, std::vector<double>(N + 1));
        M.del.assign(N + 1, std::vector<double>(N + 1));
        for (int i = 0; i <= N; ++i) {
            for (int j = 0; j <= N; ++j) {
                M.val[i][j] = v(tvals[i], Svals[j]);
                M.del[i][j] = delta(tvals[i], Svals[j]);
            }
        }
        return M;
    }

} // namespace pde

#endif // FDMETHOD_H


