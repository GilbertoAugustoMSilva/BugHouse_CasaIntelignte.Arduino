#include "Elemento.h"

struct ConfigPalmas
{

  Elemento InputSensor = Elemento();

  void Load()
  {

    InputSensor.Ativo = true;
    InputSensor.Descricacao = "Sensor Palmas";
    InputSensor.NumberPin = 12;
  }

  void Service()
  {
   }

}
