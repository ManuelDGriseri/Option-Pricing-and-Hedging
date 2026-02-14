#ifndef ASIAN_H
#define ASIAN_H

#include "Option.h"

namespace crr {

    /**
     * @brief Options path-dépendante CRR.
     * @tparam TPayoff Type de payoff.
     * @tparam TAggregator Type d'agrégateur.
     */
	template<typename TPayoff, typename TAggregator>
    class Asian : public Option {
    private:
		TPayoff payoff_;
		TAggregator aggregator_;  
        std::vector<std::vector<double>> stockTreeNR_;  ///< Arbre binomiale non recombinant des prix du sous-jacent
        void buildStockTreeNR();                        ///< Génère l’arbre binomial non recombinant des prix du sous-jacent

    public:
        Asian(double S0, double R, double sigma, double T, int N, const TPayoff& payoff, const TAggregator& aggregator);
        std::vector<std::vector<double>> treePrice() const override;
        HedgingStrategy hedgingStrategy() const override;

        /**
         * @brief Valeurs terminales de l’option path-dépendante.
         * @return Vecteur des payoffs à l’échéance pour chaque trajectoire du sous-jacent.
         */
        std::vector<double> terminalValues() const; 

        /**
         * @brief Prix asymptotique de l’option (méthode Monte Carlo).
         * @return Valeur de l’option à n = 0.
         */
        double priceMC() const;

        /**
         * @brief Delta asymptotique (méthode bump-and-reprice).
         * @return Valeur du delta à n = 0.
         */
        double deltaMC() const;
    };

    template<typename TPayoff, typename TAggregator>
    Asian<TPayoff, TAggregator>::Asian(double S0, double R, double sigma, double T, 
        int N, const TPayoff& payoff, const TAggregator& aggregator)
		: Option(S0, R, sigma, T, N), payoff_(payoff), aggregator_(aggregator)
    {
        buildStockTreeNR();
    }

    template<typename TPayoff, typename TAggregator>
    void Asian<TPayoff, TAggregator>::buildStockTreeNR() {
        stockTreeNR_.assign(N_ + 1, {});
        stockTreeNR_[0] = { S0_ };
        for (int n = 1; n <= N_; ++n) {
            int sz = 1 << n;  // 2^n
            stockTreeNR_[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                bool up = (j & 1);  // up = 1 si j impair, 0 sinon
                double prev = stockTreeNR_[n - 1][j >> 1];  // j/2
                stockTreeNR_[n][j] = prev * (1 + (up ? u_ : d_));
            }
        }
    }

    template<typename TPayoff, typename TAggregator>
    std::vector<double> Asian<TPayoff, TAggregator>::terminalValues() const {
        int leafSz = 1 << N_;
        std::vector<double> vals(leafSz);
        for (int j = 0; j < leafSz; ++j) {
            double agg = S0_;
            for (int n = 1; n <= N_; ++n) {
                int idx = j >> (N_ - n);
                agg = aggregator_(agg, stockTreeNR_[n][idx], n);
            }
            vals[j] = payoff_(agg);
        }
        return vals;
    }

    template<typename TPayoff, typename TAggregator>
    std::vector<std::vector<double>> Asian<TPayoff, TAggregator>::treePrice() const {
        auto terminal = terminalValues();
        std::vector<std::vector<double>> V(N_ + 1);
        int leafSz = 1 << N_;
        V[N_].resize(leafSz);
        for (int j = 0; j < leafSz; ++j)
            V[N_][j] = terminal[j];

        for (int n = N_ - 1; n >= 0; --n) {
            int sz = 1 << n;
            V[n].resize(sz);
            for (int j = 0; j < sz; ++j)
                V[n][j] = discount_ * 0.5 * (V[n + 1][2 * j + 1] + V[n + 1][2 * j]);
        }
        return V;
    }

    template<typename TPayoff, typename TAggregator>
    Option::HedgingStrategy Asian<TPayoff, TAggregator>::hedgingStrategy() const {
        auto V = treePrice();
        HedgingStrategy H;
        H.delta.resize(N_);
        H.bond.resize(N_);
        for (int n = 0; n < N_; ++n) {
            int sz = 1 << n;
            H.delta[n].resize(sz);
            H.bond[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                double Vu = V[n + 1][2 * j + 1];
                double Vd = V[n + 1][2 * j];
                double Su = stockTreeNR_[n + 1][2 * j + 1];
                double Sd = stockTreeNR_[n + 1][2 * j];
                double dlt = (Vu - Vd) / (Su - Sd);
                H.delta[n][j] = dlt;
                H.bond[n][j] = V[n][j] - dlt * stockTreeNR_[n][j];
            }
        }
        return H;
    }

    template<typename TPayoff, typename TAggregator>
    double Asian<TPayoff, TAggregator>::priceMC() const {
        int steps = 100;
        int paths = 10000;
        std::mt19937_64 rng(42);
        std::normal_distribution<double> nd(0.0, 1.0);
        double dt = T_ / steps;
        double discount = std::exp(-R_ * T_);
        double sumPayoff = 0.0;
        for (int i = 0; i < paths; ++i) {
            double St = S0_;
            double agg = S0_;
            for (int j = 0; j < steps; ++j) {
                double Z = nd(rng);
                St *= std::exp((R_ - 0.5 * sigma_ * sigma_) * dt + sigma_ * std::sqrt(dt) * Z);
                agg = aggregator_(agg, St, j + 1);
            }
            sumPayoff += payoff_(agg);
        }
        return discount * sumPayoff / paths;
    }

    template<typename TPayoff, typename TAggregator>
    double Asian<TPayoff, TAggregator>::deltaMC() const {
        double eps = 1e-4 * S0_;
        Asian<TPayoff, TAggregator> up(S0_ + eps, R_, sigma_, T_, N_, payoff_, aggregator_);
        Asian<TPayoff, TAggregator> dn(S0_ - eps, R_, sigma_, T_, N_, payoff_, aggregator_);
        double priceUp = up.priceMC();
        double priceDn = dn.priceMC();
        return (priceUp - priceDn) / (2 * eps);
    }

} // namespace crr

#endif // ASIAN_H


