#ifndef AGGREGATOR_H
#define AGGREGATOR_H

#include <cmath>
#include <stdexcept>

namespace crr {

	/**
	 * @brief Interface abstraite pour l'agrégation des prix path-dépendants.
	 */
	class Aggregator {
	public:
		/**
		 * @brief Calcule la nouvelle valeur agrégée.
		 * @param agg   Valeur agrégée jusqu'à l'étape précédente.
		 * @param price Prix courant du sous-jacent.
		 * @param step  Numéro de l'étape.
		 * @return Valeur agrégée mise à jour.
		 */
		virtual double operator()(double agg, double price, double step) const = 0;
	};

	/**
	 * @brief Agrégateur pour la moyenne arithmétique.
	 */
	class Arithmetic : public Aggregator {
	public:
		double operator()(double agg, double price, double step) const override {
			return (agg * step + price) / (step + 1);
		}
	};

	/**
	 * @brief Agrégateur pour la moyenne géométrique.
	 */
	class Geometric : public Aggregator {
	public:
		double operator()(double agg, double price, double step) const override {
			return std::pow(std::pow(agg, step) * price, 1.0 / (step + 1));
		}
	};

	/**
     * @brief Agrégateur pour le maximum.
	 */
	class LookMax : public Aggregator {
	public:
		double operator()(double agg, double price, double step) const override {
			return std::max<double>(agg, price);
		}
	};

	/**
	 * @brief Agrégateur pour le minimum.
	 */
	class LookMin : public Aggregator {
	public:
		double operator()(double agg, double price, double step) const override {
			return std::min<double>(agg, price);
		}
	};

} // namespace crr 

#endif // AGGREGATOR_H
