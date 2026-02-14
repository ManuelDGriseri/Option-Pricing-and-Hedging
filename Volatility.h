#include "pch.h"

#ifndef VOLATILITY_H
#define VOLATILITY_H

#include <stdexcept>

namespace pde {

    /**
     * @brief Classe abstraite représentant la volatilité du sous-jacent.
     */
    class Volatility {
    public:
        /**
         * @brief Calcule la valeur de la volatilité pour un prix S du sous-jacent à l'instant t.
         * @param t Temps.
         * @param S Prix du sous-jacent.
         * @return Valeur de la volatilité.
         */
        virtual double operator()(double t, double S) const = 0;
    };

    /**
     * @brief Volatilité dans le modèle de Black-Scholes.
     */
    class BSVol : public Volatility {
    private:
        double sigma_;

    public:
        /**
         * @param sigma Valeur constante de la volatilité.
         */
        BSVol(double sigma);

        double operator()(double t, double S) const override;
    };

    /**
     * @brief Volatilité dans un modèle à volatilité locale.
     */
    class LocalVol : public Volatility {
    private:
        double alfa_, beta_;

    public:
        /**
         * @param alfa Coefficient du temps.
         * @param beta Coefficient du sous-jacent.
         */
        LocalVol(double alfa, double beta);

        double operator()(double t, double S) const override;
    };

} // namespace pde

#endif // VOLATILITY_H
