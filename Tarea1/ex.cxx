#include <vector>
#include <cmath> // Para usar sqrt (raíz) y pow (potencia)

using namespace std;

int main(){
  return 0;
}

// Ejercicio 3 Estadistica 

double mean(vector<double> v){
  if (v.size() == 0) return 0;
  double suma = 0;
  for (int i = 0; i < v.size(); i++) {
    suma = suma + v[i];
  }
  return suma / v.size();
}

double variance(vector<double> v){
  if (v.size() == 0) return 0;
  double promedio = mean(v);
  double suma_distancias = 0;
  for (int i = 0; i < v.size(); i++) {
    suma_distancias = suma_distancias + pow(v[i] - promedio, 2);
  }
  return suma_distancias / v.size();
}

// Ejercicio 4 Pearson

double pearson_r(vector<double> A, vector<double> B){
  if (A.size() != B.size() || A.size() == 0) return 0;
  
  double m_a = mean(A);
  double m_b = mean(B);
  double arriba = 0;
  double abajo_a = 0;
  double abajo_b = 0;
  
  for (int i = 0; i < A.size(); i++) {
    arriba = arriba + (A[i] - m_a) * (B[i] - m_b);
    abajo_a = abajo_a + pow(A[i] - m_a, 2);
    abajo_b = abajo_b + pow(B[i] - m_b, 2);
  }
  
  if (abajo_a == 0 || abajo_b == 0) return 0;
  return arriba / (sqrt(abajo_a) * sqrt(abajo_b));
}

// Ejercicio 5 Conversiones (Solo usando lo básico) 

// Esta función ayuda a las demás. Convierte un número a cualquier base.
vector<char> convertir_a_base(int n, int base) {
  if (n == 0) return {'0'};
  vector<char> resultado;
  int copia_n = n;
  if (n < 0) n = -n;
  
  while (n > 0) {
    int residuo = n % base;
    char digito;
    if (residuo < 10) digito = residuo + '0';
    else digito = (residuo - 10) + 'A';
    
    // Insertamos al principio para que no salga al revés
    resultado.insert(resultado.begin(), digito);
    n = n / base;
  }
  if (copia_n < 0) resultado.insert(resultado.begin(), '-');
  return resultado;
}

// Esta función ayuda a pasar de una base a un número normal (decimal)
int pasar_a_decimal(vector<char> s, int base) {
  int n = 0;
  int inicio = 0;
  bool es_negativo = false;
  if (s.size() > 0 && s[0] == '-') {
    es_negativo = true;
    inicio = 1;
  }
  for (int i = inicio; i < s.size(); i++) {
    int valor;
    if (s[i] >= '0' && s[i] <= '9') valor = s[i] - '0';
    else valor = s[i] - 'A' + 10;
    n = n * base + valor;
  }
  return es_negativo ? -n : n;
}

vector<char> dec_to_septapus(int n){ return convertir_a_base(n, 7); }
vector<char> dec_to_octopus(int n){ return convertir_a_base(n, 8); }
vector<char> dec_to_hexakaidecapus(int n){ return convertir_a_base(n, 16); }
vector<char> septapus_to_dec(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 7), 10); }
vector<char> octopus_to_dec(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 8), 10); }
vector<char> hexakaidecapus_to_dec(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 16), 10); }
vector<char> septapus_to_octopus(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 7), 8); }
vector<char> septapus_to_hexakaidecapus(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 7), 16); }
vector<char> octapus_to_septapus(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 8), 7); }
vector<char> octopus_to_hexakaidecapus(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 8), 16); }
vector<char> hexakaidecapus_to_septapus(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 16), 7); }
vector<char> hexakaidecapus_to_octopus(vector<char> s){ return convertir_a_base(pasar_a_decimal(s, 16), 8); }
