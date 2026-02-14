#ifndef EXPORTS_H
#define EXPORTS_H

#include "Payoff.h"
#include "Option.h"
#include "European.h"
#include "Aggregator.h"
#include "Asian.h"
#include "American.h"
#include "ParabPDE.h"
#include "Volatility.h"
#include "Diffusion.h"
#include "FDMethod.h"
#include "ImplicitScheme.h"
#include "CNMethod.h"
#include <windows.h>  // MessageBoxA
#include <comdef.h>   // VARIANT
#include <OleAuto.h>  // SAFEARRAY

extern "C" {

    //=============================================================================
    // Vanilla Call
    //=============================================================================

    /**
     * @brief Calcule le prix d'un call européen.
     */
    __declspec(dllexport) double __stdcall PriceEuCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un call européen.
     */
    __declspec(dllexport) double __stdcall DeltaEuCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice 2D V[n][i] du call vanille en VARIANT SAFEARRAY.
     */
    __declspec(dllexport) VARIANT __stdcall TreeEuCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du call vanille en VARIANT SAFEARRAY.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaEuCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du call vanille en VARIANT SAFEARRAY.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondEuCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Vanilla Put
    //=============================================================================

    /**
     * @brief Calcule le prix d'un put européen.
     */
    __declspec(dllexport) double __stdcall PriceEuPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un put européen.
     */
    __declspec(dllexport) double __stdcall DeltaEuPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall TreeEuPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaEuPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondEuPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Digital Call
    //=============================================================================

    /**
     * @brief Calcule le prix d'un call digital.
     */
    __declspec(dllexport) double __stdcall PriceDigitCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un call digital.
     */
    __declspec(dllexport) double __stdcall DeltaDigitCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du call digital.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDigitCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du call digital.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaDigitCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du call digital.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondDigitCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Digital Put
    //=============================================================================

    /**
     * @brief Calcule le prix d'un put digital.
     */
    __declspec(dllexport) double __stdcall PriceDigitPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un put digital.
     */
    __declspec(dllexport) double __stdcall DeltaDigitPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du put digital.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDigitPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du put digital.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaDigitPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du put digital.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondDigitPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Double-Digital
    //=============================================================================

    /**
     * @brief Calcule le prix d'une option double-digital.
     */
    __declspec(dllexport) double __stdcall PriceDD(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Calcule le delta d'une option double-digital.
     */
    __declspec(dllexport) double __stdcall DeltaDD(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice de valorisation du double-digit.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDD(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des deltas du double-digit.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaDD(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des bonds du double-digit.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondDD(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    //=============================================================================
    // Bull Spread
    //=============================================================================

    /**
     * @brief Calcule le prix d'une option bull spread.
     */
    __declspec(dllexport) double __stdcall PriceBull(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Calcule le delta d'une option bull spread.
     */
    __declspec(dllexport) double __stdcall DeltaBull(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice de valorisation du bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBull(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des deltas du bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaBull(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des bonds du bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondBull(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    //=============================================================================
	// Bear Spread
    //=============================================================================

    /**
     * @brief Calcule le prix d'une option bear spread.
     */
    __declspec(dllexport) double __stdcall PriceBear(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Calcule le delta d'une option bear spread.
     */
    __declspec(dllexport) double __stdcall DeltaBear(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice de valorisation du bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBear(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des deltas du bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaBear(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des bonds du bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondBear(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    //=============================================================================
    // Strangle
    //=============================================================================

    /**
     * @brief Calcule le prix d'une option strangle.
     */
    __declspec(dllexport) double __stdcall PriceStrangle(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Calcule le delta d'une option strangle.
     */
    __declspec(dllexport) double __stdcall DeltaStrangle(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice de valorisation du strangle.
     */
    __declspec(dllexport) VARIANT __stdcall TreeStrangle(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des deltas du strangle.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaStrangle(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des bonds du strangle.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondStrangle(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    //=============================================================================
    // Butterfly
    //=============================================================================

    /**
     * @brief Calcule le prix d'une option butterfly.
     */
    __declspec(dllexport) double __stdcall PriceButterfly(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Calcule le delta d'une option butterfly.
     */
    __declspec(dllexport) double __stdcall DeltaButterfly(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice de valorisation du butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall TreeButterfly(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des deltas du butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaButterfly(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    /**
     * @brief Renvoie la matrice des bonds du butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondButterfly(
        double S0, double R, double sigma, double T, int N, double K1, double K2
    );

    //=============================================================================
    // Arithmetic Call
    //=============================================================================

    /**
     * @brief Calcule le prix d'un call sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall PriceAritCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un call sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall DeltaAritCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du call sur moyenne arithmétique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeAritCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du call sur moyenne arithmétique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaAritCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du call sur moyenne arithmétique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondAritCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix MC d'un call sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall PriceAritCallMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta MC d'un call sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall DeltaAritCallMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Arithmetic Put
    //=============================================================================

    /**
     * @brief Calcule le prix d'un put sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall PriceAritPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un put sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall DeltaAritPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du put sur moyenne arithmétique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeAritPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du put sur moyenne arithmétique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaAritPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du put sur moyenne arithmétique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondAritPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix MC d'un put sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall PriceAritPutMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta MC d'un put sur moyenne arithmétique.
     */
    __declspec(dllexport) double __stdcall DeltaAritPutMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Geometric Call
    //=============================================================================

    /**
     * @brief Calcule le prix d'un call sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall PriceGeomCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un call sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall DeltaGeomCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du call sur moyenne géométrique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeGeomCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du call sur moyenne géométrique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaGeomCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du call sur moyenne géométrique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondGeomCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix MC d'un call sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall PriceGeomCallMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta MC d'un call sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall DeltaGeomCallMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Geometric Put
    //=============================================================================

    /**
     * @brief Calcule le prix d'un put sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall PriceGeomPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un put sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall DeltaGeomPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du put sur moyenne géométrique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeGeomPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du put sur moyenne géométrique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaGeomPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du put sur moyenne géométrique.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondGeomPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix MC d'un put sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall PriceGeomPutMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta MC d'un put sur moyenne géométrique.
     */
    __declspec(dllexport) double __stdcall DeltaGeomPutMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Lookback Call
    //=============================================================================

    /**
     * @brief Calcule le prix d'un call sur max.
     */
    __declspec(dllexport) double __stdcall PriceMaxCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un call sur max.
     */
    __declspec(dllexport) double __stdcall DeltaMaxCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du call sur max.
     */
    __declspec(dllexport) VARIANT __stdcall TreeMaxCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du call sur max.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaMaxCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du call sur max.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondMaxCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix MC d'un call sur max.
     */
    __declspec(dllexport) double __stdcall PriceMaxCallMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta MC d'un call sur max.
     */
    __declspec(dllexport) double __stdcall DeltaMaxCallMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Lookback Put
    //=============================================================================

    /**
     * @brief Calcule le prix d'un put sur min.
     */
    __declspec(dllexport) double __stdcall PriceMinPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un put sur min.
     */
    __declspec(dllexport) double __stdcall DeltaMinPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du put sur min.
     */
    __declspec(dllexport) VARIANT __stdcall TreeMinPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du put sur min.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaMinPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du put sur min.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondMinPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix MC d'un put sur min.
     */
    __declspec(dllexport) double __stdcall PriceMinPutMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta MC d'un put sur min.
     */
    __declspec(dllexport) double __stdcall DeltaMinPutMC(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // American Call
    //=============================================================================

    /**
     * @brief Calcule le prix d'un call américain.
     */
    __declspec(dllexport) double __stdcall PriceAmCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un call américain.
     */
    __declspec(dllexport) double __stdcall DeltaAmCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du call américain.
     */
    __declspec(dllexport) VARIANT __stdcall TreeAmCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du call américain.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaAmCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du call américain.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondAmCall(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // American Put
    //=============================================================================

    /**
     * @brief Calcule le prix d'un put américain.
     */
    __declspec(dllexport) double __stdcall PriceAmPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta d'un put américain.
     */
    __declspec(dllexport) double __stdcall DeltaAmPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice de valorisation du put américain.
     */
    __declspec(dllexport) VARIANT __stdcall TreeAmPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des deltas du put américain.
     */
    __declspec(dllexport) VARIANT __stdcall TreeDeltaAmPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Renvoie la matrice des bonds du put américain.
     */
    __declspec(dllexport) VARIANT __stdcall TreeBondAmPut(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le prix Repeated Richardson d'un put américain.
     */
    __declspec(dllexport) double __stdcall PriceAmPutRR(
        double S0, double R, double sigma, double T, int N, double K
    );

    /**
     * @brief Calcule le delta Repeated Richardson d'un put américain.
     */
    __declspec(dllexport) double __stdcall DeltaAmPutRR(
        double S0, double R, double sigma, double T, int N, double K
    );

    //=============================================================================
    // Black-Scholes
    //=============================================================================

    /**
     * @brief Calcule le prix BS d'un call vanille.
     */
    __declspec(dllexport) double __stdcall PriceEuCallBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un call vanille.
     */
    __declspec(dllexport) double __stdcall DeltaEuCallBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un call vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceEuCallBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un call vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaEuCallBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un put vanille.
     */
    __declspec(dllexport) double __stdcall PriceEuPutBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un put vanille.
     */
    __declspec(dllexport) double __stdcall DeltaEuPutBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceEuPutBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaEuPutBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un call digital.
     */
    __declspec(dllexport) double __stdcall PriceDigitCallBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un call digital.
     */
    __declspec(dllexport) double __stdcall DeltaDigitCallBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un call digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceDigitCallBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un call digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaDigitCallBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un put digital.
     */
    __declspec(dllexport) double __stdcall PriceDigitPutBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un put digital.
     */
    __declspec(dllexport) double __stdcall DeltaDigitPutBS(
        double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un put digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceDigitPutBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un put digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaDigitPutBS(
        double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un double-digital.
     */
    __declspec(dllexport) double __stdcall PriceDDBS(
        double t, double S, double sigma, double T, double R, double K1, 
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un double-digital.
     */
    __declspec(dllexport) double __stdcall DeltaDDBS(
        double t, double S, double sigma, double T, double R, double K1, 
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un double-digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceDDBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un double-digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaDDBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un bull spread.
     */
    __declspec(dllexport) double __stdcall PriceBullBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un bull spread.
     */
    __declspec(dllexport) double __stdcall DeltaBullBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceBullBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaBullBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un bear spread.
     */
    __declspec(dllexport) double __stdcall PriceBearBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un bear spread.
     */
    __declspec(dllexport) double __stdcall DeltaBearBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceBearBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaBearBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un strangle.
     */
    __declspec(dllexport) double __stdcall PriceStrangleBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un strangle.
     */
    __declspec(dllexport) double __stdcall DeltaStrangleBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un strangle.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceStrangleBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un strangle.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaStrangleBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix BS d'un butterfly.
     */
    __declspec(dllexport) double __stdcall PriceButterflyBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta BS d'un butterfly.
     */
    __declspec(dllexport) double __stdcall DeltaButterflyBS(
        double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix BS d'un butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceButterflyBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta BS d'un butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaButterflyBS(
        double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    //=============================================================================
    // Volatilité Locale
    //=============================================================================

    /**
     * @brief Calcule la volatilité locale.
     */
    __declspec(dllexport) double __stdcall Vol(
        double t, double S, double alfa, double beta
    );

    /**
     * @brief Calcule le prix VL d'un call vanille.
     */
    __declspec(dllexport) double __stdcall PriceEuCallVL(
        double t, double S, double alfa, double beta, double T, double R, 
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un call vanille.
     */
    __declspec(dllexport) double __stdcall DeltaEuCallVL(
        double t, double S, double alfa, double beta, double T, double R, 
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un call vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceEuCallVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un call vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaEuCallVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un put vanille.
     */
    __declspec(dllexport) double __stdcall PriceEuPutVL(
        double t, double S, double alfa, double beta, double T, double R, 
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un put vanille.
     */
    __declspec(dllexport) double __stdcall DeltaEuPutVL(
        double t, double S, double alfa, double beta, double T, double R, 
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceEuPutVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un put vanille.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaEuPutVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un call digital.
     */
    __declspec(dllexport) double __stdcall PriceDigitCallVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un call digital.
     */
    __declspec(dllexport) double __stdcall DeltaDigitCallVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un call digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceDigitCallVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un call digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaDigitCallVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un put digital.
     */
    __declspec(dllexport) double __stdcall PriceDigitPutVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un put digital.
     */
    __declspec(dllexport) double __stdcall DeltaDigitPutVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un put digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceDigitPutVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un put digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaDigitPutVL(
        double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un double-digital.
     */
    __declspec(dllexport) double __stdcall PriceDDVL(
        double t, double S, double alfa, double beta, double T, double R, 
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un double-digital.
     */
    __declspec(dllexport) double __stdcall DeltaDDVL(
        double t, double S, double alfa, double beta, double T, double R, 
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un double-digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceDDVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un double-digital.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaDDVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un bull spread.
     */
    __declspec(dllexport) double __stdcall PriceBullVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un bull spread.
     */
    __declspec(dllexport) double __stdcall DeltaBullVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceBullVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un bull spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaBullVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un bear spread.
     */
    __declspec(dllexport) double __stdcall PriceBearVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un bear spread.
     */
    __declspec(dllexport) double __stdcall DeltaBearVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceBearVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un bear spread.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaBearVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un strangle.
     */
    __declspec(dllexport) double __stdcall PriceStrangleVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un strangle.
     */
    __declspec(dllexport) double __stdcall DeltaStrangleVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un strangle.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceStrangleVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un strangle.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaStrangleVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le prix VL d'un butterfly.
     */
    __declspec(dllexport) double __stdcall PriceButterflyVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule le delta VL d'un butterfly.
     */
    __declspec(dllexport) double __stdcall DeltaButterflyVL(
        double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du prix VL d'un butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall GridPriceButterflyVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

    /**
     * @brief Calcule la grille du delta VL d'un butterfly.
     */
    __declspec(dllexport) VARIANT __stdcall GridDeltaButterflyVL(
        double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax
    );

} // extern "C"

#endif // EXPORTS_H



