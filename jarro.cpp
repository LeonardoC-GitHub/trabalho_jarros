#include <iostream>
#include <vector>

using namespace std;

struct Capacidade
{
    int capacidadeAtual = 0;
    int capacidadeMax = 0;
    int objetivo = 0;  // Inicializando como 0 ao invés de NULL
};

int main()
{
    cout << "Trabalho de I.A.\n";

    int tam = 0;
    cout << "Informe a quantidade de jarros: ";
    cin >> tam;

    vector<Capacidade> jarros(tam);

    cout << "Informe o volume máximo dos jarros" << endl;
    for (int i = 0; i < tam; ++i)
    {
        cout << "Jarros " << i + 1 << ": ";
        cin >> jarros[i].capacidadeMax;
    }

    cout << endl;

    cout << "Informe o objetivo de cada jarro" << endl;
    for (int i = 0; i < tam; ++i)
    {
        cout << "Jarro " << i + 1 << ": ";
        cin >> jarros[i].objetivo;
    }

    // Exibindo as informações coletadas para verificar
    cout << "\nResumo das capacidades dos jarros e objetivos:\n";
    for (int i = 0; i < tam; ++i)
    {
        cout << "Jarro " << i + 1 << ": Max = " << jarros[i].capacidadeMax
             << ", Objetivo = " << jarros[i].objetivo << endl;
    }

    return 0;
}
