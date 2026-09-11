#include <Servo.h>
#include <LiquidCrystal.h>

// 1. CONFIGURACIÓN DE PINES DEL TECLADO 
const int pinesFilas[4] = {9, 8, 7, 6};     
const int pinesColumnas[4] = {5, 4, 3, 2};  

const char matriz[4][4] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const int LED_VERDE = 10;
const int LED_ROJO = 11;
const int BUZZER = 12;
const int PIN_SERVO = 13;

// 3. CONFIGURACIÓN DE PANTALLA LCD 
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

Servo miServo;

String claveCorrecta = "1234";
String claveIngresada = "";
int intentosFallidos = 0;

void setup() {
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO, LOW);
  digitalWrite(BUZZER, LOW); 

  lcd.begin(16, 2);
  mostrarPantallaInicio();

  miServo.attach(PIN_SERVO);
  miServo.write(0); 

  for (int i = 0; i < 4; i++) {
    pinMode(pinesFilas[i], OUTPUT);
    digitalWrite(pinesFilas[i], HIGH);
    pinMode(pinesColumnas[i], INPUT_PULLUP);
  }
}

void mostrarPantallaInicio() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CERRADURA LISTA");
  lcd.setCursor(0, 1);
  lcd.print("Clave: ");
}

char leerTeclado() {
  for (int f = 0; f < 4; f++) {
    digitalWrite(pinesFilas[f], LOW); 
    for (int c = 0; c < 4; c++) {
      if (digitalRead(pinesColumnas[c]) == LOW) { 
        delay(90); 
        while (digitalRead(pinesColumnas[c]) == LOW); 
        digitalWrite(pinesFilas[f], HIGH);
        return matriz[f][c]; 
      }
    }
    digitalWrite(pinesFilas[f], HIGH); 
  }
  return 0; 
}

void loop() {
  char tecla = leerTeclado(); 
  
  if (tecla != 0) {
    
    // --- BORRAR CLAVE (*) ---
    if (tecla == '*') {
      claveIngresada = "";
      digitalWrite(LED_ROJO, HIGH); delay(50); digitalWrite(LED_ROJO, LOW);
      mostrarPantallaInicio();
    }
    
    // --- CONFIRMAR (#) ---
    else if (tecla == '#') {
      if (claveIngresada == claveCorrecta) {
        digitalWrite(LED_VERDE, HIGH);
        miServo.write(90); 
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("ACCESO CONCEDIDO");
        
        for (int i = 5; i > 0; i--) {
          lcd.setCursor(0, 1);
          lcd.print("Cierra en: ");
          lcd.print(i);
          lcd.print("s   ");
          delay(1000);
        }
        
        miServo.write(0);
        digitalWrite(LED_VERDE, LOW);
        intentosFallidos = 0;
        claveIngresada = "";
        mostrarPantallaInicio();
      } 
      else {
        intentosFallidos++;
        
        if (intentosFallidos >= 3) {
          digitalWrite(LED_ROJO, HIGH);
          
         
          for (int i = 0; i < 4; i++) {
            digitalWrite(BUZZER, HIGH); delay(150);
            digitalWrite(BUZZER, LOW);  delay(150);
          }
          
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("SISTEMA BLOQUEADO");
          
          
          for (int i = 30; i > 0; i--) {
            lcd.setCursor(0, 1);
            lcd.print("Espere: ");
            if (i < 10) lcd.print("0");
            lcd.print(i);
            lcd.print("s     ");
            delay(1000);
          }
          
          digitalWrite(LED_ROJO, LOW);
          intentosFallidos = 0;
          claveIngresada = "";
          mostrarPantallaInicio();
        } 
        else {
          
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(" CLAVE INCORRECTA ");
          lcd.setCursor(0, 1);
          lcd.print("  INTENTOS: ");
          lcd.print(intentosFallidos);
          lcd.print("/3  ");
          
          digitalWrite(LED_ROJO, HIGH);
          digitalWrite(BUZZER, HIGH); delay(100); digitalWrite(BUZZER, LOW); 
          delay(100);
          digitalWrite(BUZZER, HIGH); delay(100); digitalWrite(BUZZER, LOW);
          
          delay(1000);
          digitalWrite(LED_ROJO, LOW);
          claveIngresada = "";
          mostrarPantallaInicio();
        }
      }
    }
    
    
    else {
      claveIngresada += tecla;
      lcd.setCursor(7 + (claveIngresada.length() - 1), 1);
      lcd.print("*");
      digitalWrite(LED_VERDE, HIGH); delay(50); digitalWrite(LED_VERDE, LOW);
    }
  }
}
