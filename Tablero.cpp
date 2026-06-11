#include "Tablero.h"
#include "Pieza.h"
#include "Movimientos.h"
#include "Torre.h"
#include "Caballo.h"
#include "Alfil.h"
#include "Reina.h"
#include "Rey.h"
#include "Peon.h"
#include "Estado.h"

Tablero::Tablero()
{
    casillas.resize(
        8,
        std::vector<
            std::shared_ptr<Pieza>
        >(8,nullptr)
    );
}
bool Tablero::posicionValida(
    int fila,
    int columna
)const
{
    return fila >=0 &&
           fila <8 &&
           columna >=0 &&
           columna <8;
}
std::shared_ptr<Pieza>
Tablero::obtenerPieza(
    int fila,
    int columna
)const
{
    return casillas[fila][columna];
}
void Tablero::colocarPieza(
    int fila,
    int columna,
    std::shared_ptr<Pieza> pieza
)
{
    casillas[fila][columna] = pieza;
}//ME
void Tablero::mover(const Movimiento& m){
    casillas[m.filaDestino][m.columnaDestino] = casillas[m.filaOrigen][m.columnaOrigen];
    casillas[m.filaOrigen][m.columnaOrigen] = nullptr;
    auto pieza = casillas[m.filaDestino][m.columnaDestino];
    if(!pieza)
    return;
char s = pieza->getSimbolo();
if(s=='P' && m.filaDestino==0){
    casillas[m.filaDestino][m.columnaDestino] = std::make_shared<Reina>(BLANCO);
}
if(s=='p' && m.filaDestino==7){
    casillas[m.filaDestino][m.columnaDestino] = std::make_shared<Reina>(NEGRO);
}
}
//Pando
void Tablero::inicializar()
{
    for(int i=0;i<8;i++){
        colocarPieza(1, i, std::make_shared<Peon>(NEGRO));
        colocarPieza(6, i, std::make_shared<Peon>(BLANCO));
    }
    colocarPieza(
        0,0,
        std::make_shared<Torre>(NEGRO));
    colocarPieza(
        0,7,
        std::make_shared<Torre>(NEGRO));
    colocarPieza(
        7,0,
       std::make_shared<Torre>(BLANCO));
    colocarPieza(
        7,7,
        std::make_shared<Torre>(BLANCO));
    colocarPieza(
        0,1,
        std::make_shared<Caballo>(NEGRO));
    colocarPieza(
        0,6,
        std::make_shared<Caballo>(NEGRO));
    colocarPieza(
        7,1,
        std::make_shared<Caballo>(BLANCO));
    colocarPieza(
        7,6,
        std::make_shared<Caballo>(BLANCO));
    colocarPieza(
        0,2,
        std::make_shared<Alfil>(NEGRO));
    colocarPieza(
        0,5,
        std::make_shared<Alfil>(NEGRO));
    colocarPieza(
        7,2,
        std::make_shared<Alfil>(BLANCO));
    colocarPieza(
        7,5,
        std::make_shared<Alfil>(BLANCO));
    colocarPieza(
        0,3,
        std::make_shared<Reina>(NEGRO));
    colocarPieza(
        7,3,
        std::make_shared<Reina>(BLANCO));
    colocarPieza(
        0,4,
        std::make_shared<Rey>(NEGRO));
    colocarPieza(
        7,4,
        std::make_shared<Rey>(BLANCO));
}
std::vector<Movimiento>
Tablero::obtenerMovimientos(
    Color color
) const
{
    std::vector<Movimiento> todos;
    for(int f=0;f<8;f++) {
        for(int c=0;c<8;c++){
            auto pieza =
                obtenerPieza(f,c);
            if(!pieza)
                continue;
            if(pieza->getColor()
                != color)
                continue;
            auto movimientos =
                pieza->movimientosLegales(
                    *this,
                    f,
                    c
                );
            todos.insert(
                todos.end(),
                movimientos.begin(),
                movimientos.end()
            );
        }
    }
    return todos;
}
//Amanda
bool Tablero::esEnemigo(
    int fila,
    int columna,
    Color color
) const
{
    auto pieza=obtenerPieza(fila, columna);
    if(!pieza)
        return false;
    return pieza->getColor()
        != color;
}
bool Tablero::existeRey(
    Color color
) const
{
    for(int f=0;f<8;f++){
        for(int c=0;c<8;c++){   
            auto pieza = obtenerPieza(f, c);
            if(!pieza)
                continue;
            if(pieza->getColor() == color &&(pieza->getSimbolo() == 'K' || pieza->getSimbolo() == 'k')){
                return true;
            }
        }
    }
    return false;
}
//YOAN
bool Tablero::reyEnJaque(
    Color color
) const{
    int reyFila = -1;
    int reyCol = -1;
    for(int f=0;f<8;f++){
        for(int c=0;c<8;c++){
            auto pieza = obtenerPieza(f,c);
            if(!pieza)
                continue;
            if(pieza->getColor() == color){
                char s = pieza->getSimbolo();
                if(s=='K' || s=='k'){
                    reyFila = f;
                    reyCol = c;
                }
            }
        }
    }
    Color enemigo = color == BLANCO ? NEGRO : BLANCO;
    auto movimientos = obtenerMovimientos(enemigo);
    for(auto& m : movimientos){
        if(m.filaDestino == reyFila && m.columnaDestino == reyCol){
            return true;
        }
    }
    return false;
}
//ME
std::vector<Movimiento>
Tablero::movimientosValidos(
    Color color
) const{
    std::vector<Movimiento>
    resultado;
    auto movimientos = obtenerMovimientos(color);
    for(auto& m : movimientos){
        Tablero copia = *this;
            copia.mover(m);
        if(!copia.reyEnJaque(color)){
            resultado.push_back(m);
        }
    }
    return resultado;
}
Tablero::Tablero(const Tablero& otro){
    casillas.resize(8, std::vector < std::shared_ptr<Pieza> > (8, nullptr));
    for(int f=0;f<8;f++){
        for(int c=0;c<8;c++){
            auto pieza = otro.casillas[f][c];
            if(pieza){
                casillas[f][c] = pieza->clonar();
            }
        }
    }
}
JuegoAjedrez::obtenerEstado() const{
    if(tablero.reyEnJaque(BLANCO)){
        return JAQUE_BLANCO;
    }
    if(tablero.reyEnJaque(NEGRO)){
        return JAQUE_NEGRO;
    }
    return EN_CURSO;
}
