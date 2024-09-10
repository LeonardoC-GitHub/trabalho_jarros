#include <iostream>
#include <vector>
#include <queue> // Para BFS
#include <stack> // Para DFS

using namespace std;

struct Capacidade
{
    int capacidadeAtual = 0;
    int capacidadeMax = 0;
    int objetivo = 0;
};

// Função para verificar se um jarro atingiu seu objetivo
bool atingiuObjetivo(const Capacidade &jarro)
{
    return jarro.capacidadeAtual == jarro.objetivo;
}

// Busca em Largura
void buscaEmLargura(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca em Largura ..." << endl;

    queue<int> fila;
    vector<bool> visitado(jarros.size(), false); // Vetor de controle de visitados

    fila.push(0); // Começamos a partir do primeiro jarro
    visitado[0] = true;


    while (!fila.empty())
    {
 
        int i = fila.front();
        fila.pop();

        cout << "Visitando Jarro " << i + 1 << " (Capacidade Atual: "
             << jarros[i].capacidadeAtual << ")" << endl;

        if (atingiuObjetivo(jarros[i]))
        {
            cout << "Jarro " << i + 1 << " atingiu o objetivo!" << endl;
        }

        // Adiciona jarros vizinhos não visitados à fila
        for (int j = 0; j < jarros.size(); ++j)
        {
            if (!visitado[j])
            {
                fila.push(j);
                visitado[j] = true;
            }
        }
    }
   
    for (int i = 0; i < jarros.size(); i++)
    {
        cout << "Jarro " << i + 1 << " \n " << jarros[i].capacidadeMax << " / " << jarros[i].capacidadeAtual << endl;
    }
}

// Busca em Profundidade (DFS)
void buscaEmProfundidade(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca em Profundidade (DFS)..." << endl;

    stack<int> pilha;
    vector<bool> visitado(jarros.size(), false); // Vetor de controle de visitados

    pilha.push(0); // Começamos a partir do primeiro jarro

    while (!pilha.empty())
    {
        int i = pilha.top();
        pilha.pop();

        if (!visitado[i])
        {
            visitado[i] = true;

            cout << "Visitando Jarro " << i + 1 << " (Capacidade Atual: "
                 << jarros[i].capacidadeAtual << ")" << endl;

            if (atingiuObjetivo(jarros[i]))
            {
                cout << "Jarro " << i + 1 << " atingiu o objetivo!" << endl;
            }

            // Adiciona jarros vizinhos não visitados à pilha
            for (int j = 0; j < jarros.size(); ++j)
            {
                if (!visitado[j])
                {
                    pilha.push(j);
                }
            }
        }
    }
}

int main()
{
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

    // Definindo algumas capacidades atuais de exemplo
    cout << "\nInforme a capacidade atual de cada jarro" << endl;
    for (int i = 0; i < tam; ++i)
    {
        cout << "Jarro " << i + 1 << ": ";
        cin >> jarros[i].capacidadeAtual;
    }

    // Executar BFS e DFS
    buscaEmLargura(jarros);
    cout << endl;
    // buscaEmProfundidade(jarros);

    return 0;
}
