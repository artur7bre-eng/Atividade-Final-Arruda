#ifndef FARMACIA_H
#define FARMACIA_H

#include <Arduino.h>

// -------------------- PINOS DOS BOTOES --------------------
extern const int botaoSobe;
extern const int botaoDesce;
extern const int botaoTela;
extern const int botaoOK;
extern const int botaoCancela;
extern const int botaoReabastecer;

// -------------------- LEDS E MOTOR --------------------
extern const int ledsSaida[4];
extern const int ledsEstoqueVazio[4];
extern const int motorDC;

// -------------------- CONFIGURACOES GERAIS --------------------
extern const int totalRemedios;
extern const int remediosPorSetor;
extern const int remediosPorTela;
extern const int estoqueMaximo;

extern int estoque[100];

// -------------------- CONTROLE DAS TELAS --------------------
extern int modoTela;
extern int setorSelecionado;
extern int paginaDentroSetor;
extern int remedioSelecionado;
extern int quantidadeDesejada;

// -------------------- NOMES --------------------
extern const char* const setores[4];
extern const char* const nomesRemedios[100];

// -------------------- ESTADO ANTERIOR DOS BOTOES --------------------
extern bool ultimoSobe;
extern bool ultimoDesce;
extern bool ultimoTela;
extern bool ultimoOK;
extern bool ultimoCancela;
extern bool ultimoReabastecer;

// -------------------- FUNCOES DO SISTEMA --------------------
void configurarPinos();
void inicializarEstoque();

void subirSetor();
void descerSetor();
void subirMedicamento();
void descerMedicamento();
void trocarPaginaDentroSetor();
void aumentarQuantidade();
void diminuirQuantidade();

void dispensarRemedio();
void cancelarOperacao();
void reabastecerSetorAtual();

void acenderLedSaida(int linha);
void apagarLedsSaida();
void atualizarLedsEstoque();
void ligarMotor();
void desligarMotor();

#endif
