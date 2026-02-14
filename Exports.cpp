#include "pch.h"
#include "Exports.h"

//=============================================================================
// Factorisation du code 
//=============================================================================

/**
 * @brief Déclare une fonction extern "C" __stdcall retournant un double protégée par try/catch.
 * @param name Identifiant de la fonction exportée.
 * @param args Signature (entre parenthèses) de la fonction.
 * @param body Code à exécuter (doit inclure un return).
 */
#define SAFE_DOUBLE(name, args, body)               \
extern "C" double __stdcall name args {             \
    try { body; }                                   \
    catch(const std::exception& e) {                \
        return reportError(e.what(), #name);        \
    }                                               \
    catch(...) {                                    \
        return reportError("Erreur inconnue dans " #name, #name); \
    }                                               \
}

 /**
  * @brief Déclare une fonction extern "C" __stdcall retournant un VARIANT SAFEARRAY protégée par try/catch.
  * @param name Identifiant de la fonction exportée.
  * @param args Signature (entre parenthèses) de la fonction.
  * @param body  Bloc `{ … }` remplissant et retournant un VARIANT.
  */
#define SAFE_VARIANT(name, args, body)                   \
extern "C" VARIANT __stdcall name args {                 \
    try { body; }                                        \
    catch(const std::exception& e) {                     \
        reportError(e.what(), #name);                    \
        return makeVariantFromArray(nullptr);            \
    }                                                    \
    catch(...) {                                         \
        reportError("Erreur inconnue dans " #name, #name); \
        return makeVariantFromArray(nullptr);            \
    }                                                    \
}

static bool g_errorDisplayed = false;

extern "C" __declspec(dllexport) void __stdcall ResetErrorFlag()
{
    g_errorDisplayed = false;
}

/**
 * @brief Affiche une boîte d'erreur une seule fois.
 * @param msg Le message d'erreur à afficher.
 * @param title Le titre de la boîte de dialogue.
 * @return NaN pour signaler l'erreur au code appelant.
 */
double reportError(const char* msg, const char* title) {
    if (!g_errorDisplayed) {
        wchar_t wmsg[256], wtitle[256];
        MultiByteToWideChar(CP_UTF8, 0, msg, -1, wmsg, 256);
        MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, 256);
        MessageBoxW(nullptr, wmsg, wtitle, MB_OK | MB_ICONERROR);
        g_errorDisplayed = true;
    }
    return std::nan("");
}

/**
 * @brief Emballe un SAFEARRAY* de doubles dans un VARIANT.
 * @param psa Le pointeur vers le SAFEARRAY à encapsuler.
 * @return Un VARIANT prêt à être renvoyé à VBA.
 */
VARIANT makeVariantFromArray(SAFEARRAY* psa) {
    VARIANT var;
    VariantInit(&var);
    var.vt = VT_ARRAY | VT_R8;
    var.parray = psa;
    return var;
}

/**
 * @brief Convertit un conteneur matriciel en VARIANT COM pour l’interopérabilité.
 * @tparam Mtx Type du conteneur à deux dimensions.
 * @param matrix Référence constante à la matrice de valeurs.
 * @return VARIANT contenant un SAFEARRAY bidimensionnel de doubles initialisé avec les éléments de la matrice.
 */
template<typename Mtx>
VARIANT toVariant(const Mtx& matrix) {
    int levels = int(matrix.size());
    int maxWidth = 0;
    for (int n = 0; n < levels; ++n)
        maxWidth = std::max<int>(maxWidth, int(matrix[n].size()));

    bool isExpo = maxWidth > levels;
    int rows = isExpo ? maxWidth : levels;
    int cols = levels;

    SAFEARRAYBOUND sab[2];
    sab[0].lLbound = 0; sab[0].cElements = rows;
    sab[1].lLbound = 0; sab[1].cElements = cols;
    SAFEARRAY* psa = SafeArrayCreate(VT_R8, 2, sab);

    double* data = nullptr;
    SafeArrayAccessData(psa, (void**)&data);
    for (int n = 0; n < cols; ++n) {
        int width = int(matrix[n].size());
        for (int i = 0; i < width; ++i) {
            data[i + n * rows] = matrix[n][i];
        }
    }
    SafeArrayUnaccessData(psa);
    return makeVariantFromArray(psa);
}

//=============================================================================
// Vanilla Call
//=============================================================================

SAFE_DOUBLE(PriceEuCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).price();
)

SAFE_DOUBLE(DeltaEuCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).deltaZero();
)

SAFE_VARIANT(TreeEuCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = crr::European<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).treePrice();
        return toVariant(V);                  
    }
)

SAFE_VARIANT(TreeDeltaEuCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).hedgingStrategy();
        return toVariant(H.delta);            
    }
)

SAFE_VARIANT(TreeBondEuCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).hedgingStrategy();
        return toVariant(H.bond);             
    }
)

//=============================================================================
// Vanilla Put
//=============================================================================

SAFE_DOUBLE(PriceEuPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).price();
)

SAFE_DOUBLE(DeltaEuPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).deltaZero();
)

SAFE_VARIANT(TreeEuPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = crr::European<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaEuPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondEuPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Digital Call
//=============================================================================

SAFE_DOUBLE(PriceDigitCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffDigitCall>(S0, R, sigma, T, N, opt::PayoffDigitCall(K)).price();
)

SAFE_DOUBLE(DeltaDigitCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffDigitCall>(S0, R, sigma, T, N, opt::PayoffDigitCall(K)).deltaZero();
)

SAFE_VARIANT(TreeDigitCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = crr::European<opt::PayoffDigitCall>(S0, R, sigma, T, N, opt::PayoffDigitCall(K)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaDigitCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffDigitCall>(S0, R, sigma, T, N, opt::PayoffDigitCall(K)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondDigitCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffDigitCall>(S0, R, sigma, T, N, opt::PayoffDigitCall(K)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Digital Put
//=============================================================================

SAFE_DOUBLE(PriceDigitPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffDigitPut>(S0, R, sigma, T, N, opt::PayoffDigitPut(K)).price();
)

SAFE_DOUBLE(DeltaDigitPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::European<opt::PayoffDigitPut>(S0, R, sigma, T, N, opt::PayoffDigitPut(K)).deltaZero();
)

SAFE_VARIANT(TreeDigitPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = crr::European<opt::PayoffDigitPut>(S0, R, sigma, T, N, opt::PayoffDigitPut(K)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaDigitPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffDigitPut>(S0, R, sigma, T, N, opt::PayoffDigitPut(K)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondDigitPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::European<opt::PayoffDigitPut>(S0, R, sigma, T, N, opt::PayoffDigitPut(K)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Double-Digital
//=============================================================================

SAFE_DOUBLE(PriceDD,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffDoubleDigit>(S0, R, sigma, T, N, opt::PayoffDoubleDigit(K1, K2)).price();
)

SAFE_DOUBLE(DeltaDD,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffDoubleDigit>(S0, R, sigma, T, N, opt::PayoffDoubleDigit(K1, K2)).deltaZero();
)

SAFE_VARIANT(TreeDD,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto V = crr::European<opt::PayoffDoubleDigit>(S0, R, sigma, T, N, opt::PayoffDoubleDigit(K1, K2)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaDD,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffDoubleDigit>(S0, R, sigma, T, N, opt::PayoffDoubleDigit(K1, K2)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondDD,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffDoubleDigit>(S0, R, sigma, T, N, opt::PayoffDoubleDigit(K1, K2)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Bull Spread
//=============================================================================

SAFE_DOUBLE(PriceBull,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffBull>(S0, R, sigma, T, N, opt::PayoffBull(K1, K2)).price();
)

SAFE_DOUBLE(DeltaBull,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffBull>(S0, R, sigma, T, N, opt::PayoffBull(K1, K2)).deltaZero();
)

SAFE_VARIANT(TreeBull,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto V = crr::European<opt::PayoffBull>(S0, R, sigma, T, N, opt::PayoffBull(K1, K2)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaBull,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffBull>(S0, R, sigma, T, N, opt::PayoffBull(K1, K2)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondBull,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffBull>(S0, R, sigma, T, N, opt::PayoffBull(K1, K2)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Bear Spread
//=============================================================================

SAFE_DOUBLE(PriceBear,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffBear>(S0, R, sigma, T, N, opt::PayoffBear(K1, K2)).price();
)

SAFE_DOUBLE(DeltaBear,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffBear>(S0, R, sigma, T, N, opt::PayoffBear(K1, K2)).deltaZero();
)

SAFE_VARIANT(TreeBear,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto V = crr::European<opt::PayoffBear>(S0, R, sigma, T, N, opt::PayoffBear(K1, K2)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaBear,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffBear>(S0, R, sigma, T, N, opt::PayoffBear(K1, K2)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondBear,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffBear>(S0, R, sigma, T, N, opt::PayoffBear(K1, K2)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Strangle
//=============================================================================

SAFE_DOUBLE(PriceStrangle,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffStrangle>(S0, R, sigma, T, N, opt::PayoffStrangle(K1, K2)).price();
)

SAFE_DOUBLE(DeltaStrangle,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffStrangle>(S0, R, sigma, T, N, opt::PayoffStrangle(K1, K2)).deltaZero();
)

SAFE_VARIANT(TreeStrangle,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto V = crr::European<opt::PayoffStrangle>(S0, R, sigma, T, N, opt::PayoffStrangle(K1, K2)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaStrangle,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffStrangle>(S0, R, sigma, T, N, opt::PayoffStrangle(K1, K2)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondStrangle,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffStrangle>(S0, R, sigma, T, N, opt::PayoffStrangle(K1, K2)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Butterfly
//=============================================================================

SAFE_DOUBLE(PriceButterfly,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffButterfly>(S0, R, sigma, T, N, opt::PayoffButterfly(K1, K2)).price();
)

SAFE_DOUBLE(DeltaButterfly,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    return crr::European<opt::PayoffButterfly>(S0, R, sigma, T, N, opt::PayoffButterfly(K1, K2)).deltaZero();
)

SAFE_VARIANT(TreeButterfly,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto V = crr::European<opt::PayoffButterfly>(S0, R, sigma, T, N, opt::PayoffButterfly(K1, K2)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaButterfly,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffButterfly>(S0, R, sigma, T, N, opt::PayoffButterfly(K1, K2)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondButterfly,
    (double S0, double R, double sigma, double T, int N, double K1, double K2),
    {
        auto H = crr::European<opt::PayoffButterfly>(S0, R, sigma, T, N, opt::PayoffButterfly(K1, K2)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// Arithmetic Call
//=============================================================================

using AsianCallArithmetic = crr::Asian<opt::PayoffCall, crr::Arithmetic>;  // la virgule cause problèmes à la macro

SAFE_DOUBLE(PriceAritCall,  
    (double S0, double R, double sigma, double T, int N, double K),  
    return AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).price();  
)

SAFE_DOUBLE(DeltaAritCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).deltaZero(); 
)

SAFE_VARIANT(TreeAritCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaAritCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondAritCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceAritCallMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).priceMC();
)

SAFE_DOUBLE(DeltaAritCallMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallArithmetic(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Arithmetic()).deltaMC();
)

//=============================================================================
// Arithmetic Put
//=============================================================================

using AsianPutArithmetic = crr::Asian<opt::PayoffPut, crr::Arithmetic>;

SAFE_DOUBLE(PriceAritPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).price();
)

SAFE_DOUBLE(DeltaAritPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).deltaZero();
)

SAFE_VARIANT(TreeAritPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaAritPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondAritPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceAritPutMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).priceMC();
)

SAFE_DOUBLE(DeltaAritPutMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutArithmetic(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Arithmetic()).deltaMC();
)

//=============================================================================
// Geometric Call
//=============================================================================

using AsianCallGeometric = crr::Asian<opt::PayoffCall, crr::Geometric>;

SAFE_DOUBLE(PriceGeomCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).price();
)

SAFE_DOUBLE(DeltaGeomCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).deltaZero();
)

SAFE_VARIANT(TreeGeomCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaGeomCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondGeomCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceGeomCallMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).priceMC();
)

SAFE_DOUBLE(DeltaGeomCallMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallGeometric(S0, R, sigma, T, N, opt::PayoffCall(K), crr::Geometric()).deltaMC();
)

//=============================================================================
// Geometric Put
//=============================================================================

using AsianPutGeometric = crr::Asian<opt::PayoffPut, crr::Geometric>;

SAFE_DOUBLE(PriceGeomPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).price();
)

SAFE_DOUBLE(DeltaGeomPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).deltaZero();
)

SAFE_VARIANT(TreeGeomPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaGeomPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondGeomPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceGeomPutMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).priceMC();
)

SAFE_DOUBLE(DeltaGeomPutMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutGeometric(S0, R, sigma, T, N, opt::PayoffPut(K), crr::Geometric()).deltaMC();
)

//=============================================================================
// Lookback Call
//=============================================================================

using AsianCallLookMax = crr::Asian<opt::PayoffCall, crr::LookMax>;

SAFE_DOUBLE(PriceMaxCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).price();
)

SAFE_DOUBLE(DeltaMaxCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).deltaZero();
)

SAFE_VARIANT(TreeMaxCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaMaxCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondMaxCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceMaxCallMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).priceMC();
)

SAFE_DOUBLE(DeltaMaxCallMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianCallLookMax(S0, R, sigma, T, N, opt::PayoffCall(K), crr::LookMax()).deltaMC();
)

//=============================================================================
// Lookback Put
//=============================================================================

using AsianPutLookMin = crr::Asian<opt::PayoffPut, crr::LookMin>;

SAFE_DOUBLE(PriceMinPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).price();
)

SAFE_DOUBLE(DeltaMinPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).deltaZero();
)

SAFE_VARIANT(TreeMinPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaMinPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondMinPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceMinPutMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).priceMC();
)

SAFE_DOUBLE(DeltaMinPutMC,
    (double S0, double R, double sigma, double T, int N, double K),
    return AsianPutLookMin(S0, R, sigma, T, N, opt::PayoffPut(K), crr::LookMin()).deltaMC();
)

//=============================================================================
// American Call
//=============================================================================

SAFE_DOUBLE(PriceAmCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::American<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).price();
)

SAFE_DOUBLE(DeltaAmCall,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::American<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).deltaZero();
)

SAFE_VARIANT(TreeAmCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = crr::American<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaAmCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::American<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondAmCall,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::American<opt::PayoffCall>(S0, R, sigma, T, N, opt::PayoffCall(K)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

//=============================================================================
// American Put
//=============================================================================

SAFE_DOUBLE(PriceAmPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).price();
)

SAFE_DOUBLE(DeltaAmPut,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).deltaZero();
)

SAFE_VARIANT(TreeAmPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto V = crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).treePrice();
        return toVariant(V);
    }
)

SAFE_VARIANT(TreeDeltaAmPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).hedgingStrategy();
        return toVariant(H.delta);
    }
)

SAFE_VARIANT(TreeBondAmPut,
    (double S0, double R, double sigma, double T, int N, double K),
    {
        auto H = crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).hedgingStrategy();
        return toVariant(H.bond);
    }
)

SAFE_DOUBLE(PriceAmPutRR,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).priceRR();
)

SAFE_DOUBLE(DeltaAmPutRR,
    (double S0, double R, double sigma, double T, int N, double K),
    return crr::American<opt::PayoffPut>(S0, R, sigma, T, N, opt::PayoffPut(K)).deltaRR();
)

//=============================================================================
// Black-Scholes
//=============================================================================

using DiffusionCallBS = pde::Diffusion<opt::PayoffCall, pde::BSVol>;

SAFE_DOUBLE(PriceEuCallBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallBS eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaEuCallBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallBS eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceEuCallBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallBS eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
		auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaEuCallBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallBS eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionPutBS = pde::Diffusion<opt::PayoffPut, pde::BSVol>;

SAFE_DOUBLE(PriceEuPutBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutBS eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaEuPutBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutBS eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceEuPutBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutBS eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaEuPutBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutBS eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionDigitCallBS = pde::Diffusion<opt::PayoffDigitCall, pde::BSVol>;

SAFE_DOUBLE(PriceDigitCallBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallBS eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaDigitCallBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallBS eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceDigitCallBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallBS eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaDigitCallBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallBS eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitCallBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionDigitPutBS = pde::Diffusion<opt::PayoffDigitPut, pde::BSVol>;

SAFE_DOUBLE(PriceDigitPutBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutBS eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaDigitPutBS,
    (double t, double S, double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutBS eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceDigitPutBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutBS eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaDigitPutBS,
    (double sigma, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutBS eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDigitPutBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionDoubleDigitBS = pde::Diffusion<opt::PayoffDoubleDigit, pde::BSVol>;

SAFE_DOUBLE(PriceDDBS,
    (double t, double S, double sigma, double T, double R, double K1, 
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitBS eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDoubleDigitBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaDDBS,
    (double t, double S, double sigma, double T, double R, double K1, 
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitBS eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDoubleDigitBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceDDBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitBS eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDoubleDigitBS> solver(eq, imax, jmax);
        solver.SolvePDE();
		auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaDDBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitBS eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionDoubleDigitBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionBullBS = pde::Diffusion<opt::PayoffBull, pde::BSVol>;

SAFE_DOUBLE(PriceBullBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullBS eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBullBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaBullBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullBS eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBullBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceBullBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullBS eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBullBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaBullBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullBS eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBullBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionBearBS = pde::Diffusion<opt::PayoffBear, pde::BSVol>;

SAFE_DOUBLE(PriceBearBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearBS eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBearBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaBearBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearBS eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBearBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceBearBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearBS eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBearBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaBearBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearBS eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionBearBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionStrangleBS = pde::Diffusion<opt::PayoffStrangle, pde::BSVol>;

SAFE_DOUBLE(PriceStrangleBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleBS eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionStrangleBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaStrangleBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleBS eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionStrangleBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceStrangleBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleBS eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionStrangleBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaStrangleBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleBS eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionStrangleBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionButterflyBS = pde::Diffusion<opt::PayoffButterfly, pde::BSVol>;

SAFE_DOUBLE(PriceButterflyBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyBS eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionButterflyBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaButterflyBS,
    (double t, double S, double sigma, double T, double R, double K1,
        double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyBS eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionButterflyBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceButterflyBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyBS eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionButterflyBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaButterflyBS,
    (double sigma, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyBS eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::BSVol(sigma));
        pde::CNMethod<DiffusionButterflyBS> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

//=============================================================================
// Volatilité Locale
//=============================================================================

SAFE_DOUBLE(Vol,
    (double t, double S, double alfa, double beta),
    return pde::LocalVol(alfa, beta)(t, S);
)

using DiffusionCallVL = pde::Diffusion<opt::PayoffCall, pde::LocalVol>;

SAFE_DOUBLE(PriceEuCallVL,
    (double t, double S, double alfa, double beta, double T, double R, 
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallVL eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaEuCallVL,
    (double t, double S, double alfa, double beta, double T, double R, 
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallVL eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceEuCallVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallVL eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
		auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaEuCallVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionCallVL eq(T, Smin, Smax, R, opt::PayoffCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionPutVL = pde::Diffusion<opt::PayoffPut, pde::LocalVol>;

SAFE_DOUBLE(PriceEuPutVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutVL eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaEuPutVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutVL eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceEuPutVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutVL eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaEuPutVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionPutVL eq(T, Smin, Smax, R, opt::PayoffPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionDigitCallVL = pde::Diffusion<opt::PayoffDigitCall, pde::LocalVol>;

SAFE_DOUBLE(PriceDigitCallVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallVL eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaDigitCallVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallVL eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceDigitCallVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallVL eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaDigitCallVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitCallVL eq(T, Smin, Smax, R, opt::PayoffDigitCall(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitCallVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionDigitPutVL = pde::Diffusion<opt::PayoffDigitPut, pde::LocalVol>;

SAFE_DOUBLE(PriceDigitPutVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutVL eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaDigitPutVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutVL eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceDigitPutVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutVL eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaDigitPutVL,
    (double alfa, double beta, double T, double R, double K, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDigitPutVL eq(T, Smin, Smax, R, opt::PayoffDigitPut(K), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDigitPutVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionDoubleDigitVL = pde::Diffusion<opt::PayoffDoubleDigit, pde::LocalVol>;

SAFE_DOUBLE(PriceDDVL,
    (double t, double S, double alfa, double beta, double T, double R, 
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitVL eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDoubleDigitVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaDDVL,
    (double t, double S, double alfa, double beta, double T, double R, 
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitVL eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDoubleDigitVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceDDVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitVL eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDoubleDigitVL> solver(eq, imax, jmax);
        solver.SolvePDE();
		auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaDDVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionDoubleDigitVL eq(T, Smin, Smax, R, opt::PayoffDoubleDigit(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionDoubleDigitVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionBullVL = pde::Diffusion<opt::PayoffBull, pde::LocalVol>;

SAFE_DOUBLE(PriceBullVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullVL eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBullVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaBullVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullVL eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBullVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceBullVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullVL eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBullVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaBullVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBullVL eq(T, Smin, Smax, R, opt::PayoffBull(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBullVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionBearVL = pde::Diffusion<opt::PayoffBear, pde::LocalVol>;

SAFE_DOUBLE(PriceBearVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearVL eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBearVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaBearVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearVL eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBearVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceBearVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearVL eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBearVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaBearVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionBearVL eq(T, Smin, Smax, R, opt::PayoffBear(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionBearVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionStrangleVL = pde::Diffusion<opt::PayoffStrangle, pde::LocalVol>;

SAFE_DOUBLE(PriceStrangleVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleVL eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionStrangleVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaStrangleVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleVL eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionStrangleVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceStrangleVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleVL eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionStrangleVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaStrangleVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionStrangleVL eq(T, Smin, Smax, R, opt::PayoffStrangle(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionStrangleVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)

using DiffusionButterflyVL = pde::Diffusion<opt::PayoffButterfly, pde::LocalVol>;

SAFE_DOUBLE(PriceButterflyVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyVL eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionButterflyVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.v(t, S);
    }
)

SAFE_DOUBLE(DeltaButterflyVL,
    (double t, double S, double alfa, double beta, double T, double R,
        double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyVL eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionButterflyVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        return solver.delta(t, S);
    }
)

SAFE_VARIANT(GridPriceButterflyVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyVL eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionButterflyVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.val);
    }
)

SAFE_VARIANT(GridDeltaButterflyVL,
    (double alfa, double beta, double T, double R, double K1, double K2, double Smin, double Smax, int imax, int jmax),
    {
        DiffusionButterflyVL eq(T, Smin, Smax, R, opt::PayoffButterfly(K1, K2), pde::LocalVol(alfa, beta));
        pde::CNMethod<DiffusionButterflyVL> solver(eq, imax, jmax);
        solver.SolvePDE();
        auto G = solver.grid();
        return toVariant(G.del);
    }
)


