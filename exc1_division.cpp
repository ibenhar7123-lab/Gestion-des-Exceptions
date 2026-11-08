// Exercice 1 : division sécurisée
// Compilation : g++ -std=c++17 -Wall -Wextra exc1_division.cpp -o exc1

#include <iostream>
#include <stdexcept>
using namespace std;

// Retourne a / b ; lève une exception si b == 0
int diviser(int a, int b) {
    if (b == 0) {
        throw invalid_argument("Division par zero impossible (b == 0)");
    }
    return a / b;
}

int main() {
    // Jeux de tests : { a, b }
    int tests[][2] = {
        {10, 2},
        {7, 3},      // division entière : 7 / 3 = 2
        {-12, 4},
        {5, 0},      // erreur
        {0, 9},
        {8, 0}       // erreur
    };

    for (const auto& t : tests) {
        int a = t[0];
        int b = t[1];
        try {
            int resultat = diviser(a, b);
            cout << a << " / " << b << " = " << resultat << endl;
        }
        catch (const invalid_argument& e) {
            cerr << "Erreur pour " << a << " / " << b << " : " << e.what() << endl;
        }
    }

    cout << "\nLe programme continue normalement apres les erreurs." << endl;
    return 0;
}
