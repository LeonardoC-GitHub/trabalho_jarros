#include <iostream>
#include <vector>
#include <queue>  // Para BFS
#include <stack>  // Para DFS
#include <chrono> // Para medir o tempo de execução
#include <set>    // Para armazenar estados visitados

using namespace std;
using namespace std::chrono;

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

// Função para verificar se todos os jarros atingiram seus objetivos
bool todosAtingiramObjetivo(const vector<Capacidade> &jarros)
{
    for (const auto &jarro : jarros)
    {
        if (!atingiuObjetivo(jarro))
            return false;
    }
    return true;
}

// Função para encher um jarro
void encherJarro(vector<Capacidade> &jarros, int index)
{
    jarros[index].capacidadeAtual = jarros[index].capacidadeMax;
}

// Função para esvaziar um jarro
void esvaziarJarro(vector<Capacidade> &jarros, int index)
{
    jarros[index].capacidadeAtual = 0;
}

// Função para transferir água de um jarro para outro
void transferirAgua(vector<Capacidade> &jarros, int from, int to)
{
    int quantidade = min(jarros[from].capacidadeAtual, jarros[to].capacidadeMax - jarros[to].capacidadeAtual);
    jarros[from].capacidadeAtual -= quantidade;
    jarros[to].capacidadeAtual += quantidade;
}

// Função para converter o estado dos jarros em um vetor de inteiros (para comparação)
vector<int> converterEstado(const vector<Capacidade> &jarros)
{
    vector<int> estado;
    for (const auto &jarro : jarros)
    {
        estado.push_back(jarro.capacidadeAtual);
    }
    return estado;
}

// Função de heurística para Busca Gulosa
int heuristica(const vector<Capacidade> &jarros)
{
    int heuristica = 0;
    for (const auto &jarro : jarros)
    {
        heuristica += abs(jarro.capacidadeAtual - jarro.objetivo);
    }
    return heuristica;
}

// Função para exibir estatísticas de busca
void exibirEstatisticas(const string &metodo, const vector<vector<int>> &caminho, int nosVisitados, int nosExpandidos, double fatorRamificacao, long long tempoExecucao)
{
    cout << "-----------------------------" << endl;
    cout << "Metodo de Busca: " << metodo << endl;
    cout << "Caminho da Solucao:" << endl;
    for (const auto &estado : caminho)
    {
        for (int cap : estado)
            cout << cap << " ";
        cout << endl;
    }
    cout << "Profundidade da Solucao: " << caminho.size() - 1 << endl;
    cout << "Custo da Solucao: " << caminho.size() - 1 << endl; // Custo unitário por passo
    cout << "Numero Total de Nos Visitados: " << nosVisitados << endl;
    cout << "Numero Total de Nos Expandidos: " << nosExpandidos << endl;
    cout << "Valor Medio do Fator de Ramificacao: " << fatorRamificacao << endl;
    cout << "Tempo de Execucao: " << tempoExecucao << " ms" << endl;
    cout << "-----------------------------" << endl;
}

// Função de Backtracking
bool backtracking(vector<Capacidade> &jarros, set<vector<int>> &visitado, vector<vector<int>> &caminho, int &nosExpandidos)
{
    if (todosAtingiramObjetivo(jarros))
    {
        caminho.push_back(converterEstado(jarros)); // Inclui o estado final
        return true;
    }

    // Cria um estado atual para verificar se já foi visitado
    vector<int> estadoAtual = converterEstado(jarros);

    if (visitado.count(estadoAtual))
        return false;

    visitado.insert(estadoAtual);
    caminho.push_back(estadoAtual);

    nosExpandidos++; // Contador de nós expandidos

    // Tenta encher, esvaziar, e transferir entre os jarros
    for (int i = 0; i < jarros.size(); ++i)
    {
        vector<Capacidade> novoEstado = jarros;

        // Tenta encher o jarro i
        encherJarro(novoEstado, i);
        if (backtracking(novoEstado, visitado, caminho, nosExpandidos))
            return true;

        // Tenta esvaziar o jarro i
        novoEstado = jarros;
        esvaziarJarro(novoEstado, i);
        if (backtracking(novoEstado, visitado, caminho, nosExpandidos))
            return true;

        // Tenta transferir água entre jarros
        for (int j = 0; j < jarros.size(); ++j)
        {
            if (i != j)
            {
                novoEstado = jarros;
                transferirAgua(novoEstado, i, j);
                if (backtracking(novoEstado, visitado, caminho, nosExpandidos))
                    return true;
            }
        }
    }

    caminho.pop_back(); // Remove o estado se não conduzir à solução
    return false;
}

// Função para iniciar a busca usando Backtracking
void buscaBacktracking(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca com Backtracking..." << endl;

    vector<Capacidade> estadoInicial = jarros;
    set<vector<int>> visitado;
    vector<vector<int>> caminho;
    int nosVisitados = 0, nosExpandidos = 0;

    auto start = high_resolution_clock::now();

    if (backtracking(estadoInicial, visitado, caminho, nosExpandidos))
    {
        nosVisitados = visitado.size();
        double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start).count();
        exibirEstatisticas("Backtracking", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
    }
    else
    {
        cout << "Nenhuma solucao encontrada." << endl;
    }
}

// Função de BFS (Busca em Largura)
void buscaEmLargura(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca em Largura (BFS)..." << endl;

    queue<vector<Capacidade>> fila;
    set<vector<int>> visitado;
    vector<vector<int>> caminho;
    int nosVisitados = 0, nosExpandidos = 0;

    fila.push(jarros);

    auto start = high_resolution_clock::now();

    while (!fila.empty())
    {
        vector<Capacidade> estadoAtual = fila.front();
        fila.pop();

        vector<int> estado = converterEstado(estadoAtual);

        if (visitado.count(estado))
            continue;

        visitado.insert(estado);
        caminho.push_back(estado);
        nosExpandidos++;

        // Verifica se atingimos o objetivo
        if (todosAtingiramObjetivo(estadoAtual))
        {
            nosVisitados = visitado.size();
            double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start).count();
            exibirEstatisticas("BFS", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
            return;
        }

        // Gera estados filhos
        for (int i = 0; i < estadoAtual.size(); ++i)
        {
            vector<Capacidade> novoEstado = estadoAtual;

            encherJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push(novoEstado);

            novoEstado = estadoAtual;
            esvaziarJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push(novoEstado);

            for (int j = 0; j < estadoAtual.size(); ++j)
            {
                if (i != j)
                {
                    novoEstado = estadoAtual;
                    transferirAgua(novoEstado, i, j);
                    if (!visitado.count(converterEstado(novoEstado)))
                        fila.push(novoEstado);
                }
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start).count();

    nosVisitados = visitado.size();
    double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
    exibirEstatisticas("BFS", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
}

// Função de DFS (Busca em Profundidade)
void buscaEmProfundidade(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca em Profundidade (DFS)..." << endl;

    stack<vector<Capacidade>> pilha;
    set<vector<int>> visitado;
    vector<vector<int>> caminho;
    int nosVisitados = 0, nosExpandidos = 0;

    pilha.push(jarros);

    auto start = high_resolution_clock::now();

    while (!pilha.empty())
    {
        vector<Capacidade> estadoAtual = pilha.top();
        pilha.pop();

        vector<int> estado = converterEstado(estadoAtual);

        if (visitado.count(estado))
            continue;

        visitado.insert(estado);
        caminho.push_back(estado);
        nosExpandidos++;

        // Verifica se atingimos o objetivo
        if (todosAtingiramObjetivo(estadoAtual))
        {
            nosVisitados = visitado.size();
            double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start).count();
            exibirEstatisticas("DFS", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
            return;
        }

        // Gera estados filhos
        for (int i = 0; i < estadoAtual.size(); ++i)
        {
            vector<Capacidade> novoEstado = estadoAtual;

            encherJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                pilha.push(novoEstado);

            novoEstado = estadoAtual;
            esvaziarJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                pilha.push(novoEstado);

            for (int j = 0; j < estadoAtual.size(); ++j)
            {
                if (i != j)
                {
                    novoEstado = estadoAtual;
                    transferirAgua(novoEstado, i, j);
                    if (!visitado.count(converterEstado(novoEstado)))
                        pilha.push(novoEstado);
                }
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start).count();

    nosVisitados = visitado.size();
    double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
    exibirEstatisticas("DFS", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
}

// Função de UCS (Busca de Custo Uniforme)
void buscaOrdenada(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca Ordenada (UCS)..." << endl;

    auto cmp = [](const pair<vector<Capacidade>, int> &a, const pair<vector<Capacidade>, int> &b)
    {
        return a.second > b.second; // Min-heap com base no custo acumulado
    };
    priority_queue<pair<vector<Capacidade>, int>, vector<pair<vector<Capacidade>, int>>, decltype(cmp)> fila(cmp);
    set<vector<int>> visitado;
    vector<vector<int>> caminho;
    int nosVisitados = 0, nosExpandidos = 0;

    fila.push({jarros, 0}); // {estado, custo}

    auto start = high_resolution_clock::now();

    while (!fila.empty())
    {
        auto [estadoAtual, custoAtual] = fila.top();
        fila.pop();

        vector<int> estado = converterEstado(estadoAtual);

        if (visitado.count(estado))
            continue;

        visitado.insert(estado);
        caminho.push_back(estado);
        nosExpandidos++;

        if (todosAtingiramObjetivo(estadoAtual))
        {
            nosVisitados = visitado.size();
            double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start).count();
            exibirEstatisticas("UCS", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
            return;
        }

        // Gerar novos estados
        for (int i = 0; i < estadoAtual.size(); ++i)
        {
            vector<Capacidade> novoEstado = estadoAtual;

            encherJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push({novoEstado, custoAtual + 1});

            novoEstado = estadoAtual;
            esvaziarJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push({novoEstado, custoAtual + 1});

            for (int j = 0; j < estadoAtual.size(); ++j)
            {
                if (i != j)
                {
                    novoEstado = estadoAtual;
                    transferirAgua(novoEstado, i, j);
                    if (!visitado.count(converterEstado(novoEstado)))
                        fila.push({novoEstado, custoAtual + 1});
                }
            }
        }
    }

    cout << "Nenhuma solucao encontrada." << endl;
}

// Função de Busca Gulosa
void buscaGulosa(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca Gulosa..." << endl;

    auto cmp = [](const pair<vector<Capacidade>, int> &a, const pair<vector<Capacidade>, int> &b)
    {
        return a.second > b.second; // Min-heap com base na heurística
    };
    priority_queue<pair<vector<Capacidade>, int>, vector<pair<vector<Capacidade>, int>>, decltype(cmp)> fila(cmp);
    set<vector<int>> visitado;
    vector<vector<int>> caminho;
    int nosVisitados = 0, nosExpandidos = 0;

    fila.push({jarros, heuristica(jarros)});

    auto start = high_resolution_clock::now();

    while (!fila.empty())
    {
        auto [estadoAtual, heuristicaAtual] = fila.top();
        fila.pop();

        vector<int> estado = converterEstado(estadoAtual);

        if (visitado.count(estado))
            continue;

        visitado.insert(estado);
        caminho.push_back(estado);
        nosExpandidos++;

        if (todosAtingiramObjetivo(estadoAtual))
        {
            nosVisitados = visitado.size();
            double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start).count();
            exibirEstatisticas("Busca Gulosa", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
            return;
        }

        // Gerar novos estados
        for (int i = 0; i < estadoAtual.size(); ++i)
        {
            vector<Capacidade> novoEstado = estadoAtual;

            encherJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push({novoEstado, heuristica(novoEstado)});

            novoEstado = estadoAtual;
            esvaziarJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push({novoEstado, heuristica(novoEstado)});

            for (int j = 0; j < estadoAtual.size(); ++j)
            {
                if (i != j)
                {
                    novoEstado = estadoAtual;
                    transferirAgua(novoEstado, i, j);
                    if (!visitado.count(converterEstado(novoEstado)))
                        fila.push({novoEstado, heuristica(novoEstado)});
                }
            }
        }
    }

    cout << "Nenhuma solucao encontrada." << endl;
}

// Função de busca A*
void buscaAEstrela(const vector<Capacidade> &jarros)
{
    cout << "Iniciando Busca A*..." << endl;

    auto cmp = [](const pair<vector<Capacidade>, int> &a, const pair<vector<Capacidade>, int> &b)
    {
        return a.second > b.second; // Min-heap com base no custo acumulado + heurística
    };

    priority_queue<pair<vector<Capacidade>, int>, vector<pair<vector<Capacidade>, int>>, decltype(cmp)> fila(cmp);
    set<vector<int>> visitado;
    vector<vector<int>> caminho;
    int nosVisitados = 0, nosExpandidos = 0;

    fila.push({jarros, heuristica(jarros)}); // {estado, heurística}

    auto start = high_resolution_clock::now();

    while (!fila.empty())
    {
        auto [estadoAtual, custoHeuristica] = fila.top();
        fila.pop();

        vector<int> estado = converterEstado(estadoAtual);

        if (visitado.count(estado))
            continue;

        visitado.insert(estado);
        caminho.push_back(estado);
        nosExpandidos++;

        // Verifica se atingimos o objetivo
        if (todosAtingiramObjetivo(estadoAtual))
        {
            nosVisitados = visitado.size();
            double fatorRamificacao = (nosVisitados > 1) ? static_cast<double>(nosExpandidos) / (nosVisitados - 1) : 0;
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<milliseconds>(stop - start).count();
            exibirEstatisticas("A*", caminho, nosVisitados, nosExpandidos, fatorRamificacao, duration);
            return;
        }

        // Gera novos estados (encher, esvaziar e transferir)
        for (int i = 0; i < estadoAtual.size(); ++i)
        {
            vector<Capacidade> novoEstado = estadoAtual;

            encherJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push({novoEstado, custoHeuristica + 1 + heuristica(novoEstado)});

            novoEstado = estadoAtual;
            esvaziarJarro(novoEstado, i);
            if (!visitado.count(converterEstado(novoEstado)))
                fila.push({novoEstado, custoHeuristica + 1 + heuristica(novoEstado)});

            for (int j = 0; j < estadoAtual.size(); ++j)
            {
                if (i != j)
                {
                    novoEstado = estadoAtual;
                    transferirAgua(novoEstado, i, j);
                    if (!visitado.count(converterEstado(novoEstado)))
                        fila.push({novoEstado, custoHeuristica + 1 + heuristica(novoEstado)});
                }
            }
        }
    }

    cout << "Nenhuma solucao encontrada." << endl;
}

/*_____________MAIN___________*/
int main()
{
    int tam = 0;
    /*
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

    cout << "\nInforme a capacidade atual de cada jarro" << endl;
    for (int i = 0; i < tam; ++i)
    {
        cout << "Jarro " << i + 1 << ": ";
        cin >> jarros[i].capacidadeAtual;
    }
      */
    // Executar as buscas
    /*
    buscaEmLargura(jarros);
    cout << endl;
    buscaEmProfundidade(jarros);
    cout << endl;
    buscaBacktracking(jarros);
    */
    vector<Capacidade> jarros = {{0, 8, 4}, {0, 5, 4}, {0, 3, 2}};
    cout << endl;
    // buscaOrdenada(jarros);
    cout << endl;
    // buscaGulosa(jarros);
    buscaAEstrela(jarros);
    return 0;
}
