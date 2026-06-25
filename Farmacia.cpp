#include "Farmacia.h"
#include "display_lcd.h"

// -------------------- PINOS DOS BOTOES --------------------
extern const int botaoSobe = 22;
extern const int botaoDesce = 23;
extern const int botaoTela = 24;
extern const int botaoOK = 25;
extern const int botaoCancela = 26;
extern const int botaoReabastecer = 27;

// -------------------- LEDS DE SAIDA --------------------
extern const int ledsSaida[4] = {30, 31, 32, 33};

// -------------------- LEDS DE ESTOQUE VAZIO --------------------
extern const int ledsEstoqueVazio[4] = {40, 41, 42, 43};

// -------------------- MOTOR DC --------------------
extern const int motorDC = 8;

// -------------------- CONFIGURACOES GERAIS --------------------
extern const int totalRemedios = 100;
extern const int remediosPorSetor = 25;
extern const int remediosPorTela = 4;
extern const int estoqueMaximo = 5;

int estoque[100];

// -------------------- CONTROLE DAS TELAS --------------------
int modoTela = 0;
int setorSelecionado = 0;
int paginaDentroSetor = 0;
int remedioSelecionado = 0;
int quantidadeDesejada = 1;

// -------------------- NOMES DOS SETORES --------------------
extern const char* const setores[4] = {
  "Dor cabeca",
  "Dor estomago",
  "Dor muscular",
  "Dor garganta"
};

// -------------------- NOMES DOS MEDICAMENTOS --------------------
extern const char* const nomesRemedios[100] = {
  "Dipirona", "Paracetamol", "Ibuprofeno", "AAS", "Naproxeno",
  "Dorflex", "Neosaldina", "Cefaliv", "Enxak", "Buscofem",
  "Tylenol", "Advil", "Novalgina", "Anador", "Lisador",
  "Torsilax", "Alivium", "Benegrip", "Resfenol", "Cimegripe",
  "Dorspan", "Maxalgina", "Migrane", "Sonridor", "Cafiaspirina",

  "Buscopan", "Luftal", "Omeprazol", "Pantoprazol", "Simeticona",
  "Eno", "Estomazil", "Pepsamar", "Mylanta", "Hidroxido",
  "Dramin", "Vonau", "Plasil", "Epocler", "Engov",
  "Floratil", "Imosec", "Enterogerm", "Lactulona", "Colidis",
  "Gaviscon", "Nausicalm", "Ranitidina", "Domperidona", "Digestil",

  "Torsilax", "Dorflex", "Miosan", "Nimesulida", "Cataflam",
  "Voltaren", "Profenid", "Flanax", "Naproxeno", "Ibuprofeno",
  "Alivium", "Advil", "Biofenac", "Tandrilax", "Infralax",
  "Benevran", "Salonpas", "Gelol", "Calminex", "Feldene",
  "Meloxicam", "Cetoprofeno", "Diclofenaco", "Relaxmed", "Musculare",

  "Benalet", "Strepsils", "Neopiridin", "Pastilha", "Flogoral",
  "Hexomedine", "Malvatricin", "Cepacol", "Melagriao", "Vick",
  "Dequadin", "Tantum", "Ciflogex", "Coristina", "Benegrip",
  "Resfenol", "Cimegripe", "Naldecon", "Apracur", "Tylenol",
  "Novalgina", "Propolis", "Melzinho", "Xarope", "Expector"
};

// -------------------- ESTADO ANTERIOR DOS BOTOES --------------------
bool ultimoSobe = HIGH;
bool ultimoDesce = HIGH;
bool ultimoTela = HIGH;
bool ultimoOK = HIGH;
bool ultimoCancela = HIGH;
bool ultimoReabastecer = HIGH;

void configurarPinos() {
  pinMode(botaoSobe, INPUT_PULLUP);
  pinMode(botaoDesce, INPUT_PULLUP);
  pinMode(botaoTela, INPUT_PULLUP);
  pinMode(botaoOK, INPUT_PULLUP);
  pinMode(botaoCancela, INPUT_PULLUP);
  pinMode(botaoReabastecer, INPUT_PULLUP);

  for (int i = 0; i < 4; i++) {
    pinMode(ledsSaida[i], OUTPUT);
    pinMode(ledsEstoqueVazio[i], OUTPUT);
  }

  pinMode(motorDC, OUTPUT);
}

void inicializarEstoque() {
  for (int i = 0; i < totalRemedios; i++) {
    estoque[i] = estoqueMaximo;
  }
}

void subirSetor() {
  setorSelecionado--;

  if (setorSelecionado < 0) {
    setorSelecionado = 3;
  }
}

void descerSetor() {
  setorSelecionado++;

  if (setorSelecionado > 3) {
    setorSelecionado = 0;
  }
}

void subirMedicamento() {
  int inicioSetor = setorSelecionado * remediosPorSetor;
  int inicioPagina = inicioSetor + (paginaDentroSetor * remediosPorTela);
  int fimPagina = inicioPagina + remediosPorTela - 1;
  int fimSetor = inicioSetor + remediosPorSetor - 1;

  if (fimPagina > fimSetor) {
    fimPagina = fimSetor;
  }

  remedioSelecionado--;

  if (remedioSelecionado < inicioPagina) {
    remedioSelecionado = fimPagina;
  }
}

void descerMedicamento() {
  int inicioSetor = setorSelecionado * remediosPorSetor;
  int inicioPagina = inicioSetor + (paginaDentroSetor * remediosPorTela);
  int fimPagina = inicioPagina + remediosPorTela - 1;
  int fimSetor = inicioSetor + remediosPorSetor - 1;

  if (fimPagina > fimSetor) {
    fimPagina = fimSetor;
  }

  remedioSelecionado++;

  if (remedioSelecionado > fimPagina) {
    remedioSelecionado = inicioPagina;
  }
}

void trocarPaginaDentroSetor() {
  paginaDentroSetor++;

  if (paginaDentroSetor > 6) {
    paginaDentroSetor = 0;
  }

  remedioSelecionado = (setorSelecionado * remediosPorSetor) + (paginaDentroSetor * remediosPorTela);
}

void aumentarQuantidade() {
  if (estoque[remedioSelecionado] == 0) {
    quantidadeDesejada = 0;
    return;
  }

  quantidadeDesejada++;

  if (quantidadeDesejada > estoque[remedioSelecionado]) {
    quantidadeDesejada = estoque[remedioSelecionado];
  }
}

void diminuirQuantidade() {
  if (estoque[remedioSelecionado] == 0) {
    quantidadeDesejada = 0;
    return;
  }

  quantidadeDesejada--;

  if (quantidadeDesejada < 1) {
    quantidadeDesejada = 1;
  }
}

void dispensarRemedio() {
  lcd.clear();

  if (estoque[remedioSelecionado] == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Estoque vazio!");
    lcd.setCursor(0, 1);
    imprimirNomeRemedio(remedioSelecionado);
    delay(1500);
    return;
  }

  if (quantidadeDesejada <= 0) {
    lcd.setCursor(0, 0);
    lcd.print("Qtd invalida!");
    delay(1500);
    return;
  }

  if (quantidadeDesejada > estoque[remedioSelecionado]) {
    lcd.setCursor(0, 0);
    lcd.print("Estoque");
    lcd.setCursor(0, 1);
    lcd.print("insuficiente!");
    delay(1500);
    return;
  }

  estoque[remedioSelecionado] -= quantidadeDesejada;

  lcd.setCursor(0, 0);
  lcd.print("Liberando:");
  lcd.setCursor(0, 1);
  imprimirNomeRemedio(remedioSelecionado);
  lcd.setCursor(0, 2);
  lcd.print("Quantidade: ");
  lcd.print(quantidadeDesejada);

  int linhaSelecionada = remedioSelecionado % 4;

  acenderLedSaida(linhaSelecionada);
  ligarMotor();
  delay(2000);

  apagarLedsSaida();
  desligarMotor();
  atualizarLedsEstoque();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Medicamento");
  lcd.setCursor(0, 1);
  lcd.print("liberado!");
  delay(1000);
}

void cancelarOperacao() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Operacao");
  lcd.setCursor(0, 1);
  lcd.print("cancelada");

  apagarLedsSaida();
  desligarMotor();

  delay(1000);
}

void reabastecerSetorAtual() {
  int inicio = setorSelecionado * remediosPorSetor;
  int fim = inicio + remediosPorSetor;

  for (int i = inicio; i < fim; i++) {
    estoque[i] = estoqueMaximo;
  }

  atualizarLedsEstoque();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Setor reabastecido");
  lcd.setCursor(0, 1);
  lcd.print("Setor: ");
  lcd.print(setorSelecionado + 1);
  lcd.setCursor(0, 2);
  lcd.print("Remedios ");
  lcd.print(inicio + 1);
  lcd.print(" a ");
  lcd.print(fim);
  lcd.setCursor(0, 3);
  lcd.print("Estoque: 5 cada");

  delay(1500);
}

void acenderLedSaida(int linha) {
  apagarLedsSaida();

  if (linha >= 0 && linha < 4) {
    digitalWrite(ledsSaida[linha], HIGH);
  }
}

void apagarLedsSaida() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledsSaida[i], LOW);
  }
}

void atualizarLedsEstoque() {
  bool setorVazio[4] = {false, false, false, false};

  for (int i = 0; i < totalRemedios; i++) {
    if (estoque[i] == 0) {
      int setor = i / remediosPorSetor;
      setorVazio[setor] = true;
    }
  }

  for (int setor = 0; setor < 4; setor++) {
    if (setorVazio[setor] == true) {
      digitalWrite(ledsEstoqueVazio[setor], HIGH);
    } else {
      digitalWrite(ledsEstoqueVazio[setor], LOW);
    }
  }
}

void ligarMotor() {
  digitalWrite(motorDC, HIGH);
}

void desligarMotor() {
  digitalWrite(motorDC, LOW);
}
