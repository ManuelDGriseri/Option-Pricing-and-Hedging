#ifndef OPTION_H
#define OPTION_H

#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

namespace crr {

    /**
     * @brief Classe de base Option : calcul des paramètres du modèle.
     */
    class Option {
    protected:
        double S0_, R_, sigma_, T_;  ///< Paramètres initiaux
        int    N_;                   ///< Nombre de pas
        double u_, d_, discount_;    ///< Paramètres par pas

    public:
        /**
         * @brief Constructeur : initialise les paramètres de l’option pour le modèle CRR.
         * @param S0    Prix initial du sous-jacent.
         * @param R     Taux d’intérêt sans risque continu.
         * @param sigma Volatilité annuelle du sous-jacent.
         * @param T     Durée jusqu’à l’échéance en années.
         * @param N     Nombre de pas de l’arbre binomial.
         */
        Option(double S0, double R, double sigma, double T, int N);

        /**
         * @brief Arbre des prix de l’option selon le modèle CRR.
         * @return Matrice des valeurs aux nœuds.
         */
        virtual std::vector<std::vector<double>> treePrice() const = 0;

        /**
         * @brief Prix initial de l’option.
         * @return Valeur de l’option à n = 0.
         */
        double price() const;

        /**
         * @brief Stratégie de couverture : delta et obligation.
         */
        struct HedgingStrategy {
            std::vector<std::vector<double>> delta;  ///< Positions en sous-jacent.
            std::vector<std::vector<double>> bond;   ///< Positions en actif sans risque.
        };

        /**
         * @brief Calcule la couverture optimale (CRR).
         * @return HedgingStrategy contenant delta et bond par nœud.
         */
        virtual HedgingStrategy hedgingStrategy() const = 0;

        /**
         * @brief Delta initial.
         * @return Valeur du delta à n = 0.
         */
        double deltaZero() const;
    };

} // namespace crr

#endif // OPTION_H


