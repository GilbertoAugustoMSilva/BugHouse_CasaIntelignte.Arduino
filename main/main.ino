#include "WifiSetup.h"
#include "Configuracao.h"
#include "Dispositivos.h"
#include "ConfigPalmas.h"
#include <fauxmoESP.h>
#include <Arduino.h>
#include <RCSwitch.h>


ConfigPalmas _configPalmas = ConfigPalmas();
Dispositivos _dispositivos = Dispositivos();

Configuracao _config = Configuracao();
RCSwitch _rfSwitch = RCSwitch();
WifiSetup _wifiService = WifiSetup();
fauxmoESP fauxmo = fauxmoESP();

void setup()
{

  //Inicia Serial
  Serial.begin(_config.SERIAL_BAUDRATE);
  //Inicia
  Serial.println();

  //Chama configuração do Wifi
  _wifiService.Start(_config.WIFI_SSID, _config.WIFI_PASS);

  //Carrega os Elementos
  _dispositivos.LoadElementos();

  //Ativo o rfSwitch
  _rfSwitch.enableReceive(_config.RF_PIN_NUMBER);

  pinMode(_dispositivos.Controlador.NumberPin, OUTPUT);
  digitalWrite(_dispositivos.Controlador.NumberPin, HIGH);

  pinMode(_dispositivos.Ventilador.NumberPin, OUTPUT);
  digitalWrite(_dispositivos.Ventilador.NumberPin, HIGH);

  pinMode(_dispositivos.Luz.NumberPin, OUTPUT);
  digitalWrite(_dispositivos.Luz.NumberPin, HIGH);

  fauxmo.createServer(true); // not needed, this is the default value
  fauxmo.setPort(80);        // This is required for gen3 devices

  fauxmo.enable(true);

  fauxmo.addDevice(_dispositivos.Luz.Descricacao);
  fauxmo.addDevice(_dispositivos.Ventilador.Descricacao);
  fauxmo.addDevice(_dispositivos.Exaustor.Descricacao);

  fauxmo.onSetState([](unsigned char device_id, const char *device_name, bool state, unsigned char value)
                    {
                      Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
                      if ((strcmp(device_name, _dispositivos.Luz.Descricacao) == 0))
                      {

                        Serial.println("Chamada no dispositivo Luz");
                        ExecutaLampada(state);
                        
                      }
                      else if ((strcmp(device_name, _dispositivos.Ventilador.Descricacao) == 0))
                      {
                        Serial.println("Chamada no dispositivo Ventilador");

                        ExecutarVentilador(state);
                      }
                      else if ((strcmp(device_name, _dispositivos.Exaustor.Descricacao) == 0))
                      {
                        Serial.println("Chamada no dispositivo Exaustor");

                        ExecutarExaustor(state);
                      }
                    });

  _configPalmas.Load();

}

void loop() {
  // fauxmoESP uses an async TCP server but a sync UDP server
  // Therefore, we have to manually poll for UDP packets
  fauxmo.handle();
   
  if (_rfSwitch.available()) {  
      
    Serial.printf("[MAIN] Value recepited %d ",_rfSwitch.getReceivedValue());
    if (_rfSwitch.getReceivedValue()==15927316) {
      ExecutaLampada(!_dispositivos.Luz.Ativo);
      delay(1000);
    }
    if (_rfSwitch.getReceivedValue()==15927320) {
      ExecutarVentilador(!_dispositivos.Ventilador.Ativo);
    delay(1000);
    }

    if (_rfSwitch.getReceivedValue()==15927314) {
      ExecutarExaustor(!_dispositivos.Exaustor.Ativo);
      delay(1000);
    }

    delay(100);
    _rfSwitch.resetAvailable();
  }
  
}


void ExecutarVentilador(bool ativo)
{
  if (ativo)
  {
    DesativarVentilador();
    ExecutarControlador(true);
    digitalWrite(_dispositivos.Ventilador.NumberPin, LOW);
    _dispositivos.Ventilador.Ativo = true;
    Serial.println("Chamada no dispositivo Ventilador Ativa");
  }
  else
  {
    DesativarVentilador();
  }
}

void ExecutarExaustor(bool ativo)
{
  if (ativo)
  {
    DesativarVentilador();
    ExecutarControlador(true);
    digitalWrite(_dispositivos.Exaustor.NumberPin, LOW);
    _dispositivos.Exaustor.Ativo = true;
    Serial.println("Chamada no dispositivo Exaustor Ativa");
  }
  else
  {
    DesativarVentilador();
  }
}

void ExecutarControlador(bool ativo)
{
  if (ativo)
  {
    DesativarVentilador();
   
    digitalWrite(_dispositivos.Controlador.NumberPin, LOW);
    _dispositivos.Controlador.Ativo = true;
    Serial.println("Chamada no dispositivo Controlador Ativa");
  }
  else
  {
    DesativarVentilador();
  }
}

void DesativarVentilador()
{
 
  //Desativa Controlador
  digitalWrite(_dispositivos.Controlador.NumberPin, HIGH);
  _dispositivos.Controlador.Ativo = false;
  Serial.println("Chamada no dispositivo Controlador Desativada");

  //Desativa Exaustor
  digitalWrite(_dispositivos.Exaustor.NumberPin, HIGH);
  _dispositivos.Exaustor.Ativo = false;
  Serial.println("Chamada no dispositivo Exaustor Desativada");

  //Desativa Ventilador
  digitalWrite(_dispositivos.Ventilador.NumberPin, HIGH);
  _dispositivos.Ventilador.Ativo = false;
  Serial.println("Chamada no dispositivo Ventilador Desativada");
}

void ExecutaLampada(bool ativo)
{
  if (ativo)
  {
    digitalWrite(_dispositivos.Luz.NumberPin, LOW);
    _dispositivos.Luz.Ativo = true;
    Serial.println("Chamada no dispositivo Luz Ativa");
  }
  else
  {
    digitalWrite(_dispositivos.Luz.NumberPin, HIGH);
    _dispositivos.Luz.Ativo = false;
    Serial.println("Chamada no dispositivo Luz Desativada");
  }
}
