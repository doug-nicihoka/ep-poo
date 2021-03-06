#include <iostream>
#include <string>

#include "Serie.h"
#include "SerieTemporal.h"
#include "Ponto.h"
#include "Eixo.h"
#include "EixoDinamico.h"
#include "InterfaceSerial.h"
#include "Grafico.h"

using namespace std;

#define COMM "\\\\.\\COM3"

int main() {
    InterfaceSerial* is = new InterfaceSerial(COMM);
    is->inicializar();
    string nomeDaSerie, canalX, canalY;
    int numX, numY;


    // Obtem o nome da Serie e os canais escolhidos
    cout << "Informe o nome da serie: ";
    cin >> nomeDaSerie;

    cout << "Escolha o canal X:" << endl;
    cout << "0) Tempo" << endl;
    for(int i = 1; i <= is->getQuantidadeDeCanais(); i++)
        cout << i << ") " << *(is->getNomeDosCanais() + (i - 1)) << endl;
    cin >> numX;
    if (numX > 0)
        canalX = *(is->getNomeDosCanais() + (numX - 1));
    else if (numX == 0)
        canalX = "Tempo";

    cout << "Escolha o canal Y:" << endl;
    for(int i = 1; i <= is->getQuantidadeDeCanais(); i++)
        cout << i << ") " << *(is->getNomeDosCanais() + (i - 1)) << endl;
    cin >> numY;
    canalY = *(is->getNomeDosCanais() + (numY - 1));


    // Obtem o numero de Pontos a adicionar
    int quantidade;
    cout << "Obter quantos pontos? ";
    cin >> quantidade;
    Serie* serie;
    if (numX == 0)
        serie = new SerieTemporal(nomeDaSerie, canalY);
    else
        serie = new Serie(nomeDaSerie, canalX, canalY);


    // Obtem os Pontos
    cout << "Obtendo os pontos" << endl;
    SerieTemporal* serieT = dynamic_cast<SerieTemporal *>(serie);
    if(serieT) {
        for (int i = 0; i < quantidade; i++) {
           is->atualizar();
           serieT->adicionar(is->getValor(canalY));
        }
    }
    else {
        for (int i = 0; i < quantidade; i++) {
            is->atualizar();
            serie->adicionar(is->getValor(canalX), is->getValor(canalY));
        }
    }
    //Gerar o Grafico
    cout << "Gerando o grafico" << endl;
    char tipoEixoX;
    Eixo* eixoX;
    cout << "O eixo X e estatico ou dinamico (e/d): ";
    cin >> tipoEixoX;

    if(tipoEixoX == 'e') {
        string tituloEixoX;
        double maximoEixoX, minimoEixoX;
        cout << "Informe o titulo: ";
        cin >> tituloEixoX;
        cout << "Valor minimo: ";
        cin >> minimoEixoX;
        cout << "Valor maximo: ";
        cin >> maximoEixoX;
        eixoX = new Eixo(tituloEixoX, minimoEixoX, maximoEixoX);
    }
    else if (tipoEixoX == 'd') {
        double maximoPadraoEixoX, minimoPadraoEixoX;
        cout << "Valor minimo padrao: ";
        cin >> minimoPadraoEixoX;
        cout << "Valor maximo padrao: ";
        cin >> maximoPadraoEixoX;
        if (numX == 0)
            eixoX = new EixoDinamico(minimoPadraoEixoX, maximoPadraoEixoX, serieT, true);
        else
            eixoX = new EixoDinamico(minimoPadraoEixoX, maximoPadraoEixoX, serie, true);
    }

    char tipoEixoY;
    Eixo* eixoY;
    cout << "O eixo Y e estatico ou dinamico (e/d): ";
    cin >> tipoEixoY;

    if(tipoEixoY == 'e') {
        string tituloEixoY;
        double maximoEixoY, minimoEixoY;
        cout << "Informe o titulo: ";
        cin >> tituloEixoY;
        cout << "Valor minimo: ";
        cin >> minimoEixoY;
        cout << "Valor maximo: ";
        cin >> maximoEixoY;
        eixoY = new Eixo(tituloEixoY, minimoEixoY, maximoEixoY);
    }
    else if (tipoEixoY == 'd') {
        double maximoPadraoEixoY, minimoPadraoEixoY;
        cout << "Valor minimo padrao: ";
        cin >> minimoPadraoEixoY;
        cout << "Valor maximo padrao: ";
        cin >> maximoPadraoEixoY;
        if (numX == 0)
            eixoY = new EixoDinamico(minimoPadraoEixoY, maximoPadraoEixoY, serieT, false);
        else
            eixoY = new EixoDinamico(minimoPadraoEixoY, maximoPadraoEixoY, serie, false);
    }

    Grafico* g;
    if (numX == 0)
        g = new Grafico(eixoX, eixoY, serieT);
    else
        g = new Grafico(eixoX, eixoY, serie);
    g->desenhar();
    return 0;
}
