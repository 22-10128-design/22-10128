///
/// arreglos
///

#include <cstdio>
#include <cmath>
#include <algorithm> // Necesario para std::sort en sort_AI

// vector de origen en 4 dimensiones
double o[] = { 0.0, 0.0, 0.0, 0.0 };

double a[] = { 1.0, 1.0, 1.0, 1.0 };
double b[] = { 2.2, 0.0, 1.1, 3.3 };

double x[] = { 1.0, 2.1, 3.0, 5.1, 2.0, 0.0, 6.2, 7.3 };

const int No = sizeof(o) / sizeof(o[0]);
const int Nx = sizeof(x) / sizeof(x[0]);

// 'distance': computa la distancia euclidiana
double distance(int n, double v[], double w[]) {
    double sum = 0.0;
    // Iteramos por cada dimensión
    for (int i = 0; i < n; ++i) {
        double diff = v[i] - w[i];
        sum += diff * diff; // Suma de los cuadrados de las diferencias
    }
    return sqrt(sum); // Raíz cuadrada de la suma
}

// 'vec_sum': calcula la suma de dos vectores
void vec_sum(int n, double v[], double w[], double r[]) {
    for (int i = 0; i < n; ++i) {
        // Sumamos componente a componente y guardamos en r
        r[i] = v[i] + w[i];
    }
}

// 'show' muestra un vector 'v'
void show(int n, double v[], const char *sep) {
    fprintf(stdout, "[ ");
    for (int i = 0; i < n; ++i) {
        fprintf(stdout, "%.2f%s", v[i], sep); // Agregué .2 para limitar decimales
    }
    fprintf(stdout, " ]\n");
}

// 'min' retorna el ÍNDICE del menor valor en un subrango
int min(int i, int n, double a[]) {
    int minIndex = i;       // Asumimos que el primero es el menor
    double minValue = a[i];

    for (int j = i + 1; j < n; ++j) {
        if (a[j] < minValue) {
            minValue = a[j]; // Nuevo valor mínimo encontrado
            minIndex = j;    // Guardamos su índice
        }
    }
    return minIndex; // Retornamos el índice, no el valor
}

// 'copy_array': copia un arreglo a otro
void copy_array(int n, double a[], double r[]) {
    for (int i = 0; i < n; ++i) {
        r[i] = a[i];
    }
}

// 'sort': ordena un arreglo usando Selection Sort
void sort(const int n, double a[], double r[]) {
    // 1. Primero copiamos 'a' en 'r' para no modificar el original
    copy_array(n, a, r);

    // 2. Algoritmo de Selección (Selection Sort)
    // Iteramos desde el primer elemento hasta el penúltimo
    for (int i = 0; i < n - 1; ++i) {
        // Buscamos el índice del menor elemento en el resto del arreglo (desde i hasta n)
        int k = min(i, n, r);

        // Intercambiamos (Swap) r[i] con r[k]
        if (k != i) {
            double temp = r[i];
            r[i] = r[k];
            r[k] = temp;
        }
    }
}

// Lo mismo que `sort`, solo que usando inteligencia artificial.
// IA Usada: Gemini 
// Insight: En C++ moderno, rara vez implementamos algoritmos de ordenamiento manualmente
// a menos que sea un ejercicio académico. La librería estándar <algorithm> provee
// std::sort, que usa "IntroSort" (una mezcla de QuickSort, HeapSort e InsertionSort).
// Es O(N log N), mucho más rápido que el Selection Sort implementado arriba que es O(N^2).
void sort_AI(const int n, double a[], double r[]) {
    // Copiamos los datos primero
    copy_array(n, a, r);
    
    // Usamos la función nativa de C++
    // r: puntero al inicio, r + n: puntero al final
    std::sort(r, r + n);
}

// PRUEBAS ------------------------------------------------

void test_distance() {
    fprintf(stdout, "test_distance\n");

    double ao = distance(No, a, o);
    fprintf(stdout, "a <-> o ==> %f\n", ao);

    double bo = distance(No, b, o);
    fprintf(stdout, "b <-> o ==> %f\n", bo);

    double ab = distance(No, a, b);
    fprintf(stdout, "a <-> b ==> %f\n", ab);

    double ba = distance(No, b, a);
    fprintf(stdout, "b <-> a ==> %f\n", ba);
}

void test_vec_sum() {
    fprintf(stdout, "test_vec_sum\n");
    double r[No];

    vec_sum(No, a, b, r); show(No, r, " ");
    vec_sum(No, a, o, r); show(No, r, " ");
    vec_sum(No, o, b, r); show(No, r, " ");
    vec_sum(No, b, a, r); show(No, r, " ");
}

void test_sort() {
    fprintf(stdout, "test_sort\n");
    // aqui reservamos memoria para almacenar el resultado de ordenar a x
    double r[Nx];
    
    fprintf(stdout, "Original:\n");
    show(Nx, x, " ");
    
    fprintf(stdout, "Manual Sort (Selection):\n");
    sort(Nx, x, r);
    show(Nx, r, " ");

    fprintf(stdout, "AI Sort (std::sort):\n");
    double r_ai[Nx];
    sort_AI(Nx, x, r_ai);
    show(Nx, r_ai, " ");
}

int main(int argc, const char *argv[]) {
    test_distance();
    fprintf(stdout, "----------------\n");
    test_vec_sum();
    fprintf(stdout, "----------------\n");
    test_sort();
    return 0;
}
