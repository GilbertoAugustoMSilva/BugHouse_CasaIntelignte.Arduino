#include "Elemento.h"

struct Dispositivos
{
    Elemento Luz = Elemento();
    Elemento Controlador = Elemento();
    Elemento Ventilador = Elemento();
    Elemento Exaustor = Elemento();
    void LoadElementos()
    {
        //Controlador
        Controlador.Descricacao = "Controlador";
        Controlador.NumberPin = 4;
        Controlador.Ativo = false;

        //Luz
        Luz.Descricacao = "Luz";
        Luz.NumberPin = 13;
        Luz.Ativo = false;

        // Ventilador
        Ventilador.Descricacao = "Ventilador";
        Ventilador.NumberPin = 14;
        Ventilador.Ativo = false;


        // Exaustor
        Exaustor.Descricacao = "Exaustor";
        Exaustor.NumberPin = 10;
        Exaustor.Ativo = false;
    }
};
