#pragma once
#include "../piezas/Pieza.h"
class Jugador{
protected:
    Color color;
public:
    Jugador(Color c);
    virtual ~Jugador();
    Color getColor() const;
};
