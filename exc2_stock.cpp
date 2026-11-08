// Exercice 2 : gestion d'un stock de produits (exception personnalisée)
// Compilation : g++ -std=c++17 -Wall -Wextra exc2_stock.cpp -o exc2

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

// ----------------------------------------------------------------------
// Exception personnalisée : dérive de std::exception et redéfinit what()
// Elle transporte aussi les informations utiles (demandé / disponible).
// ----------------------------------------------------------------------
class StockInsuffisantException : public exception {
private:
    int demande;
    int disponible;
    string message;

public:
    StockInsuffisantException(int demande, int disponible)
        : demande(demande), disponible(disponible) {
        message = "Stock insuffisant : " + to_string(demande) +
                  " demande(s), seulement " + to_string(disponible) + " disponible(s)";
    }

    const char* what() const noexcept override { return message.c_str(); }

    int getDemande() const { return demande; }
    int getDisponible() const { return disponible; }
};

// ----------------------------------------------------------------------
// Classe Produit
// ----------------------------------------------------------------------
class Produit {
private:
    string nom;
    int stock;

public:
    Produit(const string& nom, int stockInitial) : nom(nom), stock(stockInitial) {
        if (stockInitial < 0) {
            throw invalid_argument("Le stock initial ne peut pas etre negatif");
        }
    }

    void vendre(int quantite) {
        if (quantite <= 0) {
            throw invalid_argument("La quantite vendue doit etre strictement positive");
        }
        if (quantite > stock) {
            throw StockInsuffisantException(quantite, stock);
        }
        stock -= quantite;
    }

    int getStock() const { return stock; }
    const string& getNom() const { return nom; }

    void afficher() const {
        cout << "Produit '" << nom << "' - stock : " << stock << endl;
    }
};

int main() {
    Produit clavier("Clavier", 10);
    clavier.afficher();

    int ventes[] = {3, 4, 5, 2, -1};   // 3 + 4 = 7 OK, puis 5 > 3 restant : erreur

    for (int q : ventes) {
        try {
            cout << "\nVente de " << q << " unite(s)..." << endl;
            clavier.vendre(q);
            cout << "Vente reussie. ";
            clavier.afficher();
        }
        catch (const StockInsuffisantException& e) {
            cerr << "Erreur : " << e.what() << endl;
            cerr << "  -> il manque " << (e.getDemande() - e.getDisponible())
                 << " unite(s)" << endl;
        }
        catch (const invalid_argument& e) {
            cerr << "Erreur : " << e.what() << endl;
        }
    }

    cout << "\nEtat final : ";
    clavier.afficher();
    return 0;
}
