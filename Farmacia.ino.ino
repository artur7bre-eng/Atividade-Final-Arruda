#include "Farmacia.h"
#include "display_LCD.h"

void setup() {
  configurarPinos();
  inicializarEstoque();

  iniciarDisplay();

  apagarLedsSaida();
  desligarMotor();
  atualizarLedsEstoque();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Farmacia");
  lcd.setCursor(0, 1);
  lcd.print("Automatizada");
  lcd.setCursor(0, 2);
  lcd.print("Por sintomas");
  lcd.setCursor(0, 3);
  lcd.print("Iniciando...");
  delay(1500);

  mostrarTelaSetores();
}

void loop() {
  bool leituraSobe = digitalRead(botaoSobe);
  bool leituraDesce = digitalRead(botaoDesce);
  bool leituraTela = digitalRead(botaoTela);
  bool leituraOK = digitalRead(botaoOK);
  bool leituraCancela = digitalRead(botaoCancela);
  bool leituraReabastecer = digitalRead(botaoReabastecer);

  // -------------------- BOTAO SOBE --------------------
  if (ultimoSobe == HIGH && leituraSobe == LOW) {
    if (modoTela == 0) {
      subirSetor();
      mostrarTelaSetores();
    } 
    else if (modoTela == 1) {
      subirMedicamento();
      mostrarTelaMedicamentos();
    } 
    else if (modoTela == 2) {
      aumentarQuantidade();
      mostrarTelaQuantidade();
    }

    delay(200);
  }

  // -------------------- BOTAO DESCE --------------------
  if (ultimoDesce == HIGH && leituraDesce == LOW) {
    if (modoTela == 0) {
      descerSetor();
      mostrarTelaSetores();
    } 
    else if (modoTela == 1) {
      descerMedicamento();
      mostrarTelaMedicamentos();
    } 
    else if (modoTela == 2) {
      diminuirQuantidade();
      mostrarTelaQuantidade();
    }

    delay(200);
  }

  // -------------------- BOTAO TROCA TELA --------------------
  if (ultimoTela == HIGH && leituraTela == LOW) {
    if (modoTela == 1) {
      trocarPaginaDentroSetor();
      mostrarTelaMedicamentos();
    }

    delay(200);
  }

  // -------------------- BOTAO OK --------------------
  if (ultimoOK == HIGH && leituraOK == LOW) {
    if (modoTela == 0) {
      modoTela = 1;
      paginaDentroSetor = 0;
      remedioSelecionado = setorSelecionado * remediosPorSetor;
      mostrarTelaMedicamentos();
    } 
    else if (modoTela == 1) {
      modoTela = 2;

      if (estoque[remedioSelecionado] > 0) {
        quantidadeDesejada = 1;
      } else {
        quantidadeDesejada = 0;
      }

      mostrarTelaQuantidade();
    } 
    else if (modoTela == 2) {
      dispensarRemedio();
      modoTela = 1;
      mostrarTelaMedicamentos();
    }

    delay(200);
  }

  // -------------------- BOTAO CANCELA --------------------
  if (ultimoCancela == HIGH && leituraCancela == LOW) {
    if (modoTela == 2) {
      modoTela = 1;
      mostrarTelaMedicamentos();
    } 
    else {
      cancelarOperacao();
      modoTela = 0;
      mostrarTelaSetores();
    }

    delay(200);
  }

  // -------------------- BOTAO REABASTECER --------------------
  if (ultimoReabastecer == HIGH && leituraReabastecer == LOW) {
    reabastecerSetorAtual();

    if (modoTela == 0) {
      mostrarTelaSetores();
    } 
    else if (modoTela == 1) {
      mostrarTelaMedicamentos();
    } 
    else if (modoTela == 2) {
      mostrarTelaQuantidade();
    }

    delay(200);
  }

  ultimoSobe = leituraSobe;
  ultimoDesce = leituraDesce;
  ultimoTela = leituraTela;
  ultimoOK = leituraOK;
  ultimoCancela = leituraCancela;
  ultimoReabastecer = leituraReabastecer;

  atualizarLedsEstoque();
}
