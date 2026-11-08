// Exercice 3 : lecture d'un fichier avec gestion d'erreurs


#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

// Lit un fichier ligne par ligne et l'affiche.
// Lève une exception si le fichier ne peut pas être ouvert.
void lireFichier(const string& nomFichier) {
    ifstream fichier(nomFichier);

    if (!fichier.is_open()) {
        throw runtime_error("Impossible d'ouvrir le fichier '" + nomFichier +
                            "' (introuvable ou acces refuse)");
    }

    string ligne;
    int numero = 1;
    while (getline(fichier, ligne)) {
        cout << numero << " : " << ligne << endl;
        ++numero;
    }

    // Distingue la fin normale du fichier (eof) d'une vraie erreur de lecture (bad)
    if (fichier.bad()) {
        throw runtime_error("Erreur pendant la lecture de '" + nomFichier + "'");
    }

    // Le fichier est fermé automatiquement à la destruction de 'fichier' (RAII)
}

// Petite fonction utilitaire pour que le programme soit autonome :
// elle crée un fichier d'exemple à lire.
void creerFichierExemple(const string& nom) {
    ofstream out(nom);
    out << "Premiere ligne du fichier\n";
    out << "Deuxieme ligne\n";
    out << "Troisieme et derniere ligne\n";
}

int main() {
    creerFichierExemple("exemple.txt");

    // Test 1 : fichier existant
    cout << "=== Lecture de exemple.txt ===" << endl;
    try {
        lireFichier("exemple.txt");
    }
    catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
    }

    // Test 2 : fichier inexistant
    cout << "\n=== Lecture de inexistant.txt ===" << endl;
    try {
        lireFichier("inexistant.txt");
    }
    catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
    }

    cout << "\nFin du programme (aucun plantage)." << endl;
    return 0;
}
