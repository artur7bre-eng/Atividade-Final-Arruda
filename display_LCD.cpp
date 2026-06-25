#include "display_lcd.h"
#include "Farmacia.h"

// Configuração do LCD I2C 20x4
LiquidCrystal_I2C lcd(0x20, 20, 4);

void iniciarDisplay() {
  lcd.init();
  lcd.backlight();
}

void mostrarMensagemInicial() {
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
}

void mostrarTelaSetores() {
  lcd.clear();

  for (int i = 0; i < 4; i++) {
    lcd.setCursor(0, i);

    if (setorSelecionado == i) {
      lcd.print(">");
    } else {
      lcd.print(" ");
    }

    lcd.print(i + 1);
    lcd.print(" ");
    lcd.print(setores[i]);

    lcd.setCursor(17, i);
    lcd.print("L:");
    lcd.print(i + 1);
  }
}

void mostrarTelaMedicamentos() {
  lcd.clear();

  int inicioSetor = setorSelecionado * remediosPorSetor;
  int inicioPagina = inicioSetor + (paginaDentroSetor * remediosPorTela);
  int fimSetor = inicioSetor + remediosPorSetor - 1;

  for (int linha = 0; linha < 4; linha++) {
    int indice = inicioPagina + linha;

    lcd.setCursor(0, linha);

    if (indice <= fimSetor) {
      if (remedioSelecionado == indice) {
        lcd.print(">");
      } else {
        lcd.print(" ");
      }

      imprimirNomeRemedio(indice);

      lcd.setCursor(12, linha);
      lcd.print("Q:");
      lcd.print(estoque[indice]);

      lcd.setCursor(17, linha);
      lcd.print("L:");
      lcd.print(setorSelecionado + 1);
    }
  }
}

void mostrarTelaQuantidade() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Selecionado:");

  lcd.setCursor(0, 1);
  imprimirNomeRemedio(remedioSelecionado);

  lcd.setCursor(13, 1);
  lcd.print("L:");
  lcd.print(setorSelecionado + 1);

  lcd.setCursor(0, 2);
  lcd.print("Qtd desejada: ");
  lcd.print(quantidadeDesejada);

  lcd.setCursor(0, 3);
  lcd.print("Estoque: ");
  lcd.print(estoque[remedioSelecionado]);
}

void imprimirNomeRemedio(int indice) {
  lcd.print(nomesRemedios[indice]);
}
