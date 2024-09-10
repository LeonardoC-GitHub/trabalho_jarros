#include <iostream>
#include <vector>
#include <queue>  // Para BFS
#include <stack>  // Para DFS
#include <chrono> // Para medir o tempo de execucao

using namespace std;
using namespace std::chrono;


struct Capacidade
{
    int capacidadeAtual = 0;
    int capacidadeMax = 0;
    int objetivo = 0;
};

// Funcao para verificar se um jarro atingiu seu objetivo
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
    vector<int> caminho;
    int profundidade = 0;
    int custo = 0;
    int numNosExpandidos = 0;
    int numNosVisitados = 0;
    int somaFatorRamificacao = 0;

    auto start = high_resolution_clock::now(); // Inicia o cronometro

    fila.push(0); // Comecamos a partir do primeiro jarro
    visitado[0] = true;

    while (!fila.empty())
    {
        int i = fila.front();
        fila.pop();

        numNosVisitados++;
        caminho.push_back(i + 1); // Adiciona o jarro atual ao caminho
        profundidade++;

        cout << "Visitando Jarro " << i + 1 << " (Capacidade Atual: "
             << jarros[i].capacidadeAtual << ")" << endl;

        if (atingiuObjetivo(jarros[i]))
        {
            cout << "Jarro " << i + 1 << " atingiu o objetivo!" << endl;
            break; // Objetivo atingido, interrompe a busca
        }

        // Expande os jarros vizinhos nao visitados
        int vizinhosExpandidos = 0;
        for (int j = 0; j < jarros.size(); ++j)
        {
            if (!visitado[j])
            {
                fila.push(j);
                visitado[j] = true;
                vizinhosExpandidos++;
            }
        }

        numNosExpandidos++;
        somaFatorRamificacao += vizinhosExpandidos;
    }

    auto stop = high_resolution_clock::now();                  // Para o cronometro
    auto duration = duration_cast<milliseconds>(stop - start); // Calcula a duracao

    cout << "\n--- Estatisticas da Busca em Largura ---" << endl;
    cout << "Caminho: ";
    for (int i : caminho)
    {
        cout << i << " ";
    }
    cout << "\nProfundidade da solucao: " << profundidade << endl;
    cout << "Custo da solucao: " << numNosVisitados << endl;
    cout << "Numero de nos expandidos: " << numNosExpandidos << endl;
    cout << "Numero de nos visitados: " << numNosVisitados << endl;
    cout << "Fator de ramificacao medio: " << (numNosExpandidos > 0 ? static_cast<float>(somaFatorRamificacao) / numNosExpandidos : 0) << endl;
    cout << "Tempo de execucao: " << duration.count() << " ms" << endl;
}

// Busca em Profundidade (DFS)
void buscaEmProfundidade(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca em Profundidade (DFS)..." << endl;

    stack<int> pilha;
    vector<bool> visitado(jarros.size(), false); // Vetor de controle de visitados
    vector<int> caminho;
    int profundidade = 0;
    int custo = 0;
    int numNosExpandidos = 0;
    int numNosVisitados = 0;
    int somaFatorRamificacao = 0;

    auto start = high_resolution_clock::now(); // Inicia o cronometro

    pilha.push(0); // Comecamos a partir do primeiro jarro

    while (!pilha.empty())
    {
        int i = pilha.top();
        pilha.pop();

        if (!visitado[i])
        {
            visitado[i] = true;
            caminho.push_back(i + 1); // Adiciona o jarro atual ao caminho
            profundidade++;
            numNosVisitados++;

            cout << "Visitando Jarro " << i + 1 << " (Capacidade Atual: "
                 << jarros[i].capacidadeAtual << ")" << endl;

            if (atingiuObjetivo(jarros[i]))
            {
                cout << "Jarro " << i + 1 << " atingiu o objetivo!" << endl;
                break; // Objetivo atingido, interrompe a busca
            }

            // Expande os jarros vizinhos nao visitados
            int vizinhosExpandidos = 0;
            for (int j = 0; j < jarros.size(); ++j)
            {
                if (!visitado[j])
                {
                    pilha.push(j);
                    vizinhosExpandidos++;
                }
            }

            numNosExpandidos++;
            somaFatorRamificacao += vizinhosExpandidos;
        }
    }

    auto stop = high_resolution_clock::now();                  // Para o cronometro
    auto duration = duration_cast<milliseconds>(stop - start); // Calcula a duracao

    cout << "\n--- Estatisticas da Busca em Profundidade ---" << endl;
    cout << "Caminho: ";
    for (int i : caminho)
    {
        cout << i << " ";
    }
    cout << "\nProfundidade da solucao: " << profundidade << endl;
    cout << "Custo da solucao: " << numNosVisitados << endl;
    cout << "Numero de nos expandidos: " << numNosExpandidos << endl;
    cout << "Numero de nos visitados: " << numNosVisitados << endl;
    cout << "Fator de ramificacao medio: " << (numNosExpandidos > 0 ? static_cast<float>(somaFatorRamificacao) / numNosExpandidos : 0) << endl;
    cout << "Tempo de execucao: " << duration.count() << " ms" << endl;
}

int main()
{
    int tam = 0;
    cout << "Informe a quantidade de jarros: ";
    cin >> tam;

    vector<Capacidade> jarros(tam);

    cout << "Informe o volume maximo dos jarros" << endl;
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
    buscaEmProfundidade(jarros);

    return 0;
}
