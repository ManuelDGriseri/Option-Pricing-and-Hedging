#ifndef EUROPEAN_H
#define EUROPEAN_H

#include "Option.h"

namespace crr {

    /**
     * @brief Option européenne CRR.
     * @tparam TPayoff Type de payoff.
     */
    template<typename TPayoff>
    class European : public Option {
    private:
        TPayoff payoff_;
        std::vector<std::vector<double>> stockTree_;  ///< Arbre binomiale recombinant des prix du sous-jacent
        void buildStockTree();                        ///< Génère l’arbre binomial recombinant des prix du sous-jacent

    public:
        European(double S0, double R, double sigma, double T, int N, const TPayoff& payoff);
        std::vector<std::vector<double>> treePrice() const override;
        HedgingStrategy hedgingStrategy() const override;
    };

    template<typename TPayoff>
    European<TPayoff>::European(double S0, double R, double sigma,
        double T, int N, const TPayoff& payoff)
        : Option(S0, R, sigma, T, N), payoff_(payoff)
    {
        buildStockTree();
    }

    template<typename TPayoff>
    void European<TPayoff>::buildStockTree() {
        stockTree_.assign(N_ + 1, {});
        for (int n = 0; n <= N_; ++n) {
            stockTree_[n].resize(n + 1);
            for (int i = 0; i <= n; ++i) {
                stockTree_[n][i] = S0_ * std::pow(1 + u_, i) * std::pow(1 + d_, n - i);
            }
        }
    }

    template<typename TPayoff>
    std::vector<std::vector<double>> European<TPayoff>::treePrice() const {
        std::vector<std::vector<double>> V(N_ + 1);
        V[N_].resize(N_ + 1);
        // Valeurs terminales
        for (int i = 0; i <= N_; ++i)
            V[N_][i] = payoff_(stockTree_[N_][i]);

        // Rétropropagation 
        for (int n = N_ - 1; n >= 0; --n) {
            V[n].resize(n + 1);
            for (int i = 0; i <= n; ++i) {
                V[n][i] = discount_ * 0.5 * (V[n + 1][i + 1] + V[n + 1][i]);
            }
        }
        return V;
    }

    template<typename TPayoff>
    Option::HedgingStrategy European<TPayoff>::hedgingStrategy() const {
        auto V = treePrice();
        HedgingStrategy H;
        H.delta.resize(N_);
        H.bond.resize(N_);
        for (int n = 0; n < N_; ++n) {
            H.delta[n].resize(n + 1);
            H.bond[n].resize(n + 1);
            for (int i = 0; i <= n; ++i) {
                double Vu = V[n + 1][i + 1];
                double Vd = V[n + 1][i];
                double Su = stockTree_[n + 1][i + 1];
                double Sd = stockTree_[n + 1][i];
                double dlt = (Vu - Vd) / (Su - Sd);
                H.delta[n][i] = dlt;
                H.bond[n][i] = V[n][i] - dlt * stockTree_[n][i];
            }
        }
        return H;
    }

} // namespace crr

#endif // EUROPEAN_H

