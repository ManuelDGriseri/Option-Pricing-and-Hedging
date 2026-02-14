#ifndef PAYOFF_H
#define PAYOFF_H

#include <stdexcept>

namespace opt {

    /**
     * @brief Classe abstraite représentant le payoff d’une option.
     */
    class Payoff {
    public:
        /**
         * @brief Calcule la valeur du payoff pour un prix S du sous-jacent.
         * @param S Prix du sous-jacent à maturité.
         * @return Valeur du payoff.
         */
        virtual double operator()(double S) const = 0;
    };

    /**
     * @brief Payoff d’une option d’achat européenne (call).
     */
    class PayoffCall : public Payoff {
    private:
        double K_; 

    public:
        /**
         * @param K Prix d’exercice de l’option.
         */
        PayoffCall(double K);

        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’une option de vente européenne (put).
     */
    class PayoffPut : public Payoff {
    private:
        double K_; 

    public:
        PayoffPut(double K);
        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’un call digital.
     */
    class PayoffDigitCall : public Payoff {
    private:
        double K_;

    public:
        PayoffDigitCall(double K);
        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’un put digital.
     */
    class PayoffDigitPut : public Payoff {
    private:
        double K_;

    public:
        PayoffDigitPut(double K);
        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’une option double-digital.
     */
    class PayoffDoubleDigit : public Payoff {
    private:
        double K1_;
		double K2_;

    public:
        /**
         * @param K1 Prix d’exercice inférieur.
         * @param K2 Prix d’exercice supérieur.
         */
        PayoffDoubleDigit(double K1, double K2);

        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’une option bull spread.
     */
    class PayoffBull : public Payoff {
    private:
        double K1_;
        double K2_;

    public:
        PayoffBull(double K1, double K2);
        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’une option bear spread.
     */
    class PayoffBear : public Payoff {
    private:
        double K1_;
        double K2_;

    public:
        PayoffBear(double K1, double K2);
        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’une option strangle.
     */
    class PayoffStrangle : public Payoff {
    private:
        double K1_;
        double K2_;

    public:
        PayoffStrangle(double K1, double K2);
        double operator()(double S) const override;
    };

    /**
     * @brief Payoff d’une option butterfly.
     */
    class PayoffButterfly : public Payoff {
    private:
        double K1_;
        double K2_;

    public:
        PayoffButterfly(double K1, double K2);
        double operator()(double S) const override;
    };

} // namespace opt

#endif // PAYOFF_H

