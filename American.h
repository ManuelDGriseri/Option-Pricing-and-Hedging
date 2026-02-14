#ifndef AMERICAN_H
#define AMERICAN_H

#include "Option.h"

namespace crr {

    /**
     * @brief Option américaine CRR avec couverture via décomposition de Doob.
     * @tparam TPayoff Type de payoff.
     */
    template<typename TPayoff>
    class American : public Option {
    private:
        TPayoff payoff_;                                
        std::vector<std::vector<double>> stockTree_;  ///< Arbre recombiné du sous-jacent
        void buildStockTree();

    public:
        American(double S0, double R, double sigma, double T, int N, const TPayoff& payoff);
        std::vector<std::vector<double>> treePrice() const override;
        HedgingStrategy hedgingStrategy() const override;

        /**
         * @brief Prix asymptotique de l’option (extrapolation répétée de Richardson).
         * @return Valeur de l’option à n = 0.
         */
        double priceRR() const;

        /**
         * @brief Delta asymptotique (bump-and-reprice).
         * @return Valeur du delta à n = 0.
         */
        double deltaRR() const;
    };

    template<typename TPayoff>
    American<TPayoff>::American(double S0, double R, double sigma,
        double T, int N, const TPayoff& payoff)
        : Option(S0, R, sigma, T, N), payoff_(payoff)
    {
        buildStockTree();
    }

    template<typename TPayoff>
    void American<TPayoff>::buildStockTree() {
        stockTree_.assign(N_ + 1, {});
        for (int n = 0; n <= N_; ++n) {
            stockTree_[n].resize(n + 1);
            for (int i = 0; i <= n; ++i) {
                stockTree_[n][i] = S0_ * std::pow(1 + u_, i) * std::pow(1 + d_, n - i);
            }
        }
    }

    template<typename TPayoff>
    std::vector<std::vector<double>> American<TPayoff>::treePrice() const {
        std::vector<std::vector<double>> V(N_ + 1);
        V[N_].resize(N_ + 1);
        for (int i = 0; i <= N_; ++i)
            V[N_][i] = payoff_(stockTree_[N_][i]);

        for (int n = N_ - 1; n >= 0; --n) {
            V[n].resize(n + 1);
            for (int i = 0; i <= n; ++i) {
                double cont = discount_ * 0.5 * (V[n + 1][i + 1] + V[n + 1][i]);
                double exer = payoff_(stockTree_[n][i]);
                V[n][i] = std::max<double>(exer, cont);
            }
        }
        return V;
    }

    template<typename TPayoff>
    Option::HedgingStrategy American<TPayoff>::hedgingStrategy() const {
        // 0) Construction de l'arbre non recombinant
        std::vector<std::vector<double>> stockTreeNR;
        int N = N_;
        stockTreeNR.assign(N + 1, {});
        stockTreeNR[0] = { S0_ };
        for (int n = 1; n <= N; ++n) {
            int sz = 1 << n;
            stockTreeNR[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                bool up = (j & 1);
                double prev = stockTreeNR[n - 1][j >> 1];
                stockTreeNR[n][j] = prev * (1 + (up ? u_ : d_));
            }
        }

        // 1) Calcul de VNR sur arbre non recombinant
        std::vector<std::vector<double>> VNR(N + 1);
        VNR[N].resize(1 << N);
        for (int j = 0; j < (1 << N); ++j)
            VNR[N][j] = payoff_(stockTreeNR[N][j]);

        for (int n = N - 1; n >= 0; --n) {
            int sz = 1 << n;
            VNR[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                double cont = discount_ * 0.5 * (VNR[n + 1][2 * j] + VNR[n + 1][2 * j + 1]);
                VNR[n][j] = std::max<double>(payoff_(stockTreeNR[n][j]), cont);
            }
        }

        // 2) Calcul des incréments de compensateur 
        std::vector<std::vector<double>> incr(N);
        for (int n = 0; n < N; ++n) {
            int sz = 1 << n;
            incr[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                double cont = discount_ * 0.5 * (VNR[n + 1][2 * j] + VNR[n + 1][2 * j + 1]);
                incr[n][j] = VNR[n][j] - cont;
            }
        }

        // 3) Propagation forward pour A et calcul de M
        std::vector<std::vector<double>> A(N + 1), M(N + 1);
        A[0].resize(1);
        M[0].resize(1);
        A[0][0] = 0;
        M[0][0] = VNR[0][0];
        for (int n = 1; n <= N; ++n) {
            int sz = 1 << n;
            A[n].resize(sz);
            M[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                double prevA = A[n - 1][j >> 1];  // j/2 pour la Fn-1 mesurabilité de An
                double inc = incr[n - 1][j >> 1];
                A[n][j] = (prevA + inc) / discount_;  // faut confronter avec Vn, valeurs de An-1 e incr dans n-1
                M[n][j] = VNR[n][j] + A[n][j];
            }
        }

        // 4) Couverture sur la martingale M
        HedgingStrategy H;
        H.delta.resize(N);
        H.bond.resize(N);
        for (int n = 0; n < N; ++n) {
            int sz = 1 << n;
            H.delta[n].resize(sz);
            H.bond[n].resize(sz);
            for (int j = 0; j < sz; ++j) {
                double Mu = M[n + 1][2 * j + 1];
                double Md = M[n + 1][2 * j];
                double Su = stockTreeNR[n + 1][2 * j + 1];
                double Sd = stockTreeNR[n + 1][2 * j];
                double dlt = (Mu - Md) / (Su - Sd);
                H.delta[n][j] = dlt;
                H.bond[n][j] = M[n][j] - dlt * stockTreeNR[n][j];
            }
        }
        return H;
    }

    template<typename TPayoff>
    double American<TPayoff>::priceRR() const {
        std::vector<int> Ns = { 100, 200, 400 };
        int n = Ns.size(), m = 2;
        std::vector<std::vector<double>> A(n, std::vector<double>(m + 1));
        for (int i = 0; i < n; ++i) {
            American<TPayoff> opt(S0_, R_, sigma_, T_, Ns[i], payoff_);
            A[i][0] = opt.price();
        }

        for (int k = 1; k <= m; ++k) {
            for (int i = 0; i + k < n; ++i) {
                double ratio = Ns[i + k] / Ns[i];
                A[i][k] = A[i + 1][k - 1] + (A[i + 1][k - 1] - A[i][k - 1]) / (ratio - 1.0);
            }
        }
        return A[0][m];
    }

    template<typename TPayoff>
    double American<TPayoff>::deltaRR() const {
        double eps = 1e-4 * S0_;
        American<TPayoff> up(S0_ + eps, R_, sigma_, T_, N_, payoff_);
        American<TPayoff> dn(S0_ - eps, R_, sigma_, T_, N_, payoff_);
        double priceUp = up.priceRR();
        double priceDn = dn.priceRR();
        return (priceUp - priceDn) / (2 * eps);
    }

} // namespace crr

#endif // AMERICAN_H

