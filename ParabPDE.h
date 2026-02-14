#ifndef PARABPDE_H
#define PARABPDE_H

#include <stdexcept>
#include <cmath>

namespace pde {

    /**
     * @brief Interface pour EDP paraboliques de type général.
     */
    class ParabPDE {
    protected:
		double T_, Smin_, Smax_;  ///< Domaine de l'EDP

    public:
        ParabPDE(double T, double Smin, double Smax)
            : T_(T), Smin_(Smin), Smax_(Smax)
        {
            if (T_ <= 0.0 || Smin_ >= Smax_ || Smin_ < 0)
                throw std::invalid_argument("Domaine EDP invalide");
        }

        /**
		 * @brief Coefficients de l'équation différentielle partielle.
         */
        virtual double a(double t, double S) const = 0;
        virtual double b(double t, double S) const = 0;
        virtual double c(double t, double S) const = 0;
        virtual double d(double t, double S) const = 0;

        /**
         * @brief Terminal Boundary Condition.
         * @return v(T,S).
         */
		virtual double Terminal(double S) const = 0;

        /**
         * @brief Lower Boundary Condition.
         * @return v(t,Smin).
         */
        virtual double Lower(double t) const = 0;

        /**
         * @brief Upper Boundary Condition.
         * @return v(t,Smax).
         */
        virtual double Upper(double t) const = 0;

		double T() const { return T_; }
		double Smin() const { return Smin_; }
		double Smax() const { return Smax_; }
    };

} // namespace pde

#endif // PARABPDE_H

