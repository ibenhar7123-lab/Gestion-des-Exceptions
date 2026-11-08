// Exercice 4 : simulation bancaire avec plusieurs exceptions


#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

// ----------------------------------------------------------------------
// Exceptions personnalisées (ici dérivées de runtime_error :
// le message est géré par la classe de base, pas besoin de redéfinir what())
// ----------------------------------------------------------------------
class MontantInvalideException : public runtime_error {
public:
    explicit MontantInvalideException(int montant)
        : runtime_error("Montant invalide : " + to_string(montant) +
                        " (le montant doit etre strictement positif)") {}
};

class SoldeInsuffisantException : public runtime_error {
private:
    int solde;
    int montant;

public:
    SoldeInsuffisantException(int solde, int montant)
        : runtime_error("Solde insuffisant : retrait de " + to_string(montant) +
                        " demande, solde disponible " + to_string(solde)),
          solde(solde), montant(montant) {}

    int getManque() const { return montant - solde; }
};

// ----------------------------------------------------------------------
// Classe Compte
// ----------------------------------------------------------------------
class Compte {
private:
    int solde;

public:
    explicit Compte(int soldeInitial = 0) : solde(soldeInitial) {
        if (soldeInitial < 0) {
            throw MontantInvalideException(soldeInitial);
        }
    }

    void deposer(int montant) {
        if (montant <= 0) {
            throw MontantInvalideException(montant);
        }
        solde += montant;
    }

    void retirer(int montant) {
        if (montant <= 0) {
            throw MontantInvalideException(montant);
        }
        if (montant > solde) {
            throw SoldeInsuffisantException(solde, montant);
        }
        solde -= montant;
    }

    int getSolde() const { return solde; }
};

int main() {
    Compte compte(100);
    cout << "Solde initial : " << compte.getSolde() << endl;

    // --- 1. Plusieurs blocs catch pour un même try ---
    cout << "\n=== Operations successives (catch multiples) ===" << endl;

    // Chaque opération est : {type, montant} ; type 'D' = dépôt, 'R' = retrait
    struct Operation { char type; int montant; };
    Operation operations[] = {
        {'D', 50},     // OK       -> 150
        {'R', 30},     // OK       -> 120
        {'D', -20},    // montant invalide
        {'R', 500},    // solde insuffisant
        {'R', 0},      // montant invalide
        {'R', 120}     // OK       -> 0
    };

    for (const auto& op : operations) {
        try {
            if (op.type == 'D') {
                cout << "Depot de " << op.montant << "... ";
                compte.deposer(op.montant);
            } else {
                cout << "Retrait de " << op.montant << "... ";
                compte.retirer(op.montant);
            }
            cout << "OK, nouveau solde : " << compte.getSolde() << endl;
        }
        catch (const MontantInvalideException& e) {
            cout << "REFUSE\n  " << e.what() << endl;
        }
        catch (const SoldeInsuffisantException& e) {
            cout << "REFUSE\n  " << e.what()
                 << " (il manque " << e.getManque() << ")" << endl;
        }
    }

    // --- 2. Blocs try-catch imbriqués ---
    cout << "\n=== Try-catch imbriques ===" << endl;
    try {
        cout << "Creation d'un compte avec un solde initial negatif..." << endl;
        Compte invalide(-10);   // lève MontantInvalideException
        cout << "(jamais atteint)" << endl;
    }
    catch (const MontantInvalideException& e) {
        cout << "Erreur de creation : " << e.what() << endl;

        // Un nouveau try à l'intérieur du catch : on réessaie avec un compte valide
        try {
            cout << "Nouvelle tentative avec un compte valide de 20..." << endl;
            Compte valide(20);
            valide.retirer(50);   // lève SoldeInsuffisantException
        }
        catch (const SoldeInsuffisantException& e2) {
            cout << "Erreur interne : " << e2.what() << endl;
        }
    }

    // --- 3. catch générique en dernier recours ---
    cout << "\n=== Catch generique (runtime_error) ===" << endl;
    try {
        compte.retirer(1000);
    }
    catch (const runtime_error& e) {
        // Attrape les deux exceptions personnalisées (classe de base commune)
        cout << "Erreur bancaire interceptee : " << e.what() << endl;
    }

    cout << "\nSolde final : " << compte.getSolde() << endl;
    return 0;
}
