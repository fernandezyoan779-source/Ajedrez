#include "Jugador.h"

Jugador::Jugador(Color c)
{
    color=c;
}
Jugador::~Jugador(){}
Color Jugador::getColor() const
{
    return color;
}