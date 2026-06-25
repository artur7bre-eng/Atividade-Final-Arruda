#ifndef DISPLAY_LCD_H
#define DISPLAY_LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

extern LiquidCrystal_I2C lcd;

void iniciarDisplay();
void mostrarMensagemInicial();
void mostrarTelaSetores();
void mostrarTelaMedicamentos();
void mostrarTelaQuantidade();
void imprimirNomeRemedio(int indice);

#endif
