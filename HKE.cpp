/*
Equivalencia de dois automatos utilizando o
algoritmo de equivalencia de Hopcroft e Karp
e obtendo uma palavra que os distinguem

Rafael Cardoso da Silva    21048012
26/11/2015
*/
#include <iostream>
#include <cstdio>
#include <stack>
#include <string>
#include <sys/time.h>

using namespace std;

class ListaConjuntos {
public:
    ListaConjuntos() {}
    virtual ~ListaConjuntos() {}
    void init(int num) { // O(n)
        int i;
        N = num;
        itens = new int[N];
        for(i = 0; i < N; i++) {
            itens[i] = i;
        }
    }
    void MERGE(int A, int B) {
        /// Implemtenacao do MERGE conforme o artigo
        /// "A Linear List Merging Algorithm" de Hopcroft e Ullman

        // 1- achar a raiz do conjunto A e B
        int raizA = FIND(A);
        int raizB = FIND(B);

        // 2- fazer a raiz A ser filho da raiz B
        itens[raizA] = raizB;       // faz A filho do B

        // 3- Dar a raiz o nome C e #C = #A + #B

        // 4- remova o ponteiro NAME(A) e NAME(B) e set NAME(C)
        // ... nao necessario nesta implemtentacao
    }
    int FIND(int el) {
        /// Implemtenacao do FIND recursivo para encontrar a
        /// raiz de um elemento e otimizar a arvore de conjunto

        // 1- se ele ja for RAIZ,
        if(itens[el] == el) return el;

        // 2- senao chama recursivo seu pai ateh achar a raiz, e atualiza seu pai
        itens[el] = FIND(itens[el]);

        // 3- assim retorna seu pai atualizado, q sera a raiz apos toda atualizacao do caminho ateh a raiz
        return itens[el];
    }
     void deletarTudo() {
        delete itens;
    }
class ListaConjuntos { retorna EL
protected:
private:
    int N;
    int *itens;
};

class HKE {
public:
    HKE(){};
    virtual ~HKE(){};
    void lerAlfabeto();
    void lerResposta();
    void lerGabarito();
    void unirDFA();
    void Executar();
    bool Resultado();
    std::string distinguirPalavra();
    void deletarTudo();
protected:
private:
    // numero de simbolos |E| (somente o INgabarito)
    int numE;
    char *alfabeto;


    // numero de estado |Q|
    int g_numQ, r_numQ;
    //string* Qr, Qg; //< nao necessita os nomes dos estados

    // delta: tabela de trasnsicao
    int **g_D, **r_D;

    // indice do q0
    int g_q0, r_q0;

    // estados de aceitacao
    int g_numF, r_numF;
    int *g_F, *r_F;


    // variaveis globais da UNIAO
    int Qtotal;
    // delta da uniao dos DFA gabarito e resposta
    int **Delta;

    // conjunto F da unia DFA
    bool *F;

    ListaConjuntos baldes;

    struct hist_item{  /// <pai1, pai2> <filho1,f2> letra
        int p1, p2, f1, f2, idAlf;
        hist_item(int p1, int p2, int f1, int f2, int idAlf){
        	this->p1 = p1;
        	this->p2 = p2; 
        	this->f1 = f1; 
        	this->f2 = f2;
        	this->idAlf = idAlf;
        }
    };
    stack<hist_item *> pHist;
};



// 1 - funcao para ler do STDIN o alfabeto
void HKE::lerAlfabeto() {
    int i;
    cin >> numE;
    alfabeto = new char[numE];
    for (i = 0; i < numE; i++) {
        cin >> alfabeto[i];
    }
}

// 2 - funcao para ler do STDIN a 5-tupla do DFA gabarito
void HKE::lerGabarito() {
    int i, j;

    cin >> g_numQ;

    g_D = new int*[g_numQ];
    for (i = 0; i < g_numQ; i++) {
        g_D[i] = new int[numE];
        for (j = 0; j < numE; j++) {
            cin >> g_D[i][j];
        }
    }

    cin >> g_q0;

    cin >> g_numF;
    g_F = new int[g_numF];
    for (i = 0; i < g_numF; i++) {
        cin >> g_F[i];
    }
}

// 3 - funcao para ler do STDIN a 5-tupla do DFA resposta
void HKE::lerResposta() {
    int i, j;

    cin >> r_numQ;

    r_D = new int*[r_numQ];
    for (i = 0; i < r_numQ; i++) {
        r_D[i] = new int[numE];
        for (j = 0; j < numE; j++) {
            cin >> r_D[i][j];
        }
    }

    cin >> r_q0;

    cin >> r_numF;
    r_F = new int[r_numF];
    for (i = 0; i < r_numF; i++) {
        cin >> r_F[i];
    }
}


// 4 - uni as duas tabelas de transicao delta em uma unica
void HKE::unirDFA() {
    // matriz Delta de tamanho |Qtotal|*|Qtotal|
    Qtotal = g_numQ + r_numQ;
    Delta = new int*[Qtotal];

    // uni o delta gabarito
    for(int i = 0; i < g_numQ; i++) {
        Delta[i] = new int[numE];
        for(int j = 0; j < numE; j++) {
            Delta[i][j] = g_D[i][j];
        }
    }
    // uni o delta resposta
    for(int i = 0; i < r_numQ; i++) {
        Delta[i + g_numQ] = new int[numE];
        for(int j = 0; j < numE; j++) {
            Delta[i + g_numQ][j] = r_D[i][j] + g_numQ;
        }
    }

    // uni o conjunto F
    F = new bool[Qtotal];
    for(int i = 0; i < Qtotal; i++) {
        F[i] = false;
    }
    for(int i = 0; i < g_numF; i++) {
        F[g_F[i]] = true;
    }
    for(int i = 0; i < r_numF; i++) {
        F[r_F[i] + g_numQ] = true;
    }
}

// 5 - algoritimos de Equivalencia de Hopcroft e Karp
void HKE::Executar() {
    int q0 = g_q0;
    int p0 = g_numQ + r_q0;
    int q1, q2, r1, r2;

    baldes.init(Qtotal);
    baldes.MERGE(q0, p0);
    stack < pair<int, int> > pilha;
    pilha.push(pair<int, int>(q0, p0));
    pHist.push(new hist_item(-1, -1, q0, p0, -1));
    if( F[q0] != F[p0] ){
        pHist.push(new hist_item(-2, -2, -2, -2, -2)); // <pai1, pai2> <filho1,filho2> letra
        return;
    }
    pair<int, int> par;
    while(!pilha.empty()) {
        par = pilha.top();
        pilha.pop();
        q1 = par.first;
        q2 = par.second;
        for(int i = 0; i < numE; i++) {
            r1 = baldes.FIND( Delta[q1][i] );
            r2 = baldes.FIND( Delta[q2][i] );
            pHist.push(new hist_item(q1, q2, r1, r2, i));
            if(r1 != r2) {
                if( F[r1] != F[r2] ){
                    pHist.push(new hist_item(-2, -2, -2, -2, -2));
                    return;
                }
                baldes.MERGE(r1, r2);
                pilha.push(pair<int, int>(r1, r2));
            }
        }
    }
}

// 6 - funcao q retorna se S1 e S2 sao equivalentes
bool HKE::Resultado() {
    if(pHist.top()->p1 == -2 && pHist.top()->p2 == -2){
        return false;
    }
    return true;
}


// 7 - funcao q encontra palavra q distingue dos dois DFA
std::string HKE::distinguirPalavra() {
    string palavra = "";
    hist_item *h;
    h = pHist.top();
    if(h->p1==-2 && h->p2==-2) // foi distinguido
        pHist.pop();

    while(!pHist.empty()){
        h = pHist.top();
        int p1 = h->p1;
        int p2 = h->p2;
        if(p1==-1 && p2==-1) // cheguei no q0 e p0
            break;
        palavra = alfabeto[h->idAlf] + palavra;

        while (!( pHist.top()->f1 == p1 && pHist.top()->f2 == p2 ) )// <pai1, pai2> <filho1,filho2> letra
            pHist.pop();
    }
    pHist.pop();

    if(palavra.length()==0)
        palavra += "#";

    return palavra;
}
// 8 - Delete todas as variaveis alocadas
void HKE::deletarTudo() {
    int i;

    delete alfabeto;

    for (i = 0; i < g_numQ; i++) {
        delete g_D[i];
    }
    delete g_D;

    for (i = 0; i < r_numQ; i++) {
        delete r_D[i];
    }
    delete r_D;

    delete g_F;
    delete r_F;

    for (i = 0; i < g_numQ + r_numQ; i++) {
        delete Delta[i];
    }
    delete Delta;

    delete F;

    baldes.deletarTudo();
}

int main() {
    HKE M;

    M.lerAlfabeto(); // le o conj do alfabeto (o msm para ambos)

    M.lerGabarito(); // le primiero o gabarito

    M.lerResposta(); // le depois a resposta

    M.unirDFA(); // uni os dois DFA em uma unica delta"

    struct timeval t1, t2;
    gettimeofday(&t1, 0);

    M.Executar();  // executa o algoritmo de Equivalencia de Hopcroft e Karp

    gettimeofday(&t2, 0);

    if( M.Resultado() ) {
        cout << "1";
    } else {
        cout << "2" << endl;
        cout << M.distinguirPalavra();
    }

    M.deletarTudo();

    double tempo = (t2.tv_sec*1000.0 + t2.tv_usec/1000.0) - (t1.tv_sec*1000.0 + t1.tv_usec/1000.0);
    printf("\ntime=%.10lf\n", tempo);

    return 0;
}
