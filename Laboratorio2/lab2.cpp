#include "lab2.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

int main() {
    // Preparación: Creamos algunas listas de prueba
    // l4 = [1, 2, 3, 4, 5]
    List<int> l4 = List<int>().cons(5).cons(4).cons(3).cons(2).cons(1);
    // l_rep = [1, 1, 2, 2, 2, 3]
    List<int> l_rep = List<int>().cons(3).cons(2).cons(2).cons(2).cons(1).cons(1);

    cout << "===========================================" << endl;
    cout << "   PRUEBAS DEL LABORATORIO 2" << endl;
    cout << "   Lista base l4: " << show(l4) << endl;
    cout << "===========================================" << endl;

    // 1. reduceLeft
    cout << "\n1. reduceLeft (Suma): ";
    int suma = reduceLeft<int, int>([](int acc, int x){ return acc + x; }, 0, l4);
    cout << suma << " (Esperado: 15)" << endl;

    // 2. reduceLeftRec
    cout << "2. reduceLeftRec (Suma): ";
    int sumaRec = reduceLeftRec<int, int>([](int acc, int x){ return acc + x; }, 0, l4);
    cout << sumaRec << " (Esperado: 15)" << endl;

    // 3. map
    cout << "3. map (x * 2): ";
    List<int> m1 = map<int, int>([](int x){ return x * 2; }, l4);
    cout << show(m1) << " (Esperado: [2, 4, 6, 8, 10])" << endl;

    // 4. mapRec
    cout << "4. mapRec (x + 10): ";
    List<int> m2 = mapRec<int, int>([](int x){ return x + 10; }, l4);
    cout << show(m2) << " (Esperado: [11, 12, 13, 14, 15])" << endl;

    // 5. mapReduce
    cout << "5. mapReduce (x * x): ";
    List<int> m3 = mapReduce<int, int>([](int x){ return x * x; }, l4);
    cout << show(m3) << " (Esperado: [1, 4, 9, 16, 25])" << endl;

    // 6. concat
    cout << "6. concat: ";
    List<List<int>> lss = List<List<int>>().cons(l4).cons(l4);
    cout << show(concat(lss)) << " (Esperado: [1, 2, 3, 4, 5, 1, 2, 3, 4, 5])" << endl;

    // 7. filter
    cout << "7. filter (Pares): ";
    List<int> f1 = filter([](int x){ return x % 2 == 0; }, l4);
    cout << show(f1) << " (Esperado: [2, 4])" << endl;

    // 8. filterRec
    cout << "8. filterRec (Impares): ";
    List<int> f2 = filterRec([](int x){ return x % 2 != 0; }, l4);
    cout << show(f2) << " (Esperado: [1, 3, 5])" << endl;

    // 9. filterMap
    cout << "9. filterMap (> 3): ";
    List<int> f3 = filterMap([](int x){ return x > 3; }, l4);
    cout << show(f3) << " (Esperado: [4, 5])" << endl;

    // 10. filterReduce
    cout << "10. filterReduce (< 3): ";
    List<int> f4 = filterReduce([](int x){ return x < 3; }, l4);
    cout << show(f4) << " (Esperado: [1, 2])" << endl;

    // 11. groupEqual
    cout << "11. groupEqual: ";
    cout << show(groupEqual(l_rep)) << " (Esperado: [[1, 1], [2, 2, 2], [3]])" << endl;

    // 12. encode
    cout << "12. encode: ";
    auto encoded = encode(l_rep);
    cout << show(encoded) << " (Esperado: [(1, 2), (2, 3), (3, 1)])" << endl;

    // 13. decode
    cout << "13. decode: ";
    cout << show(decode(encoded)) << " (Esperado: [1, 1, 2, 2, 2, 3])" << endl;

    // 14. zip
    cout << "14. zip: ";
    List<string> ls = List<string>().cons("E").cons("D").cons("C").cons("B").cons("A");
    auto zipped = zip(l4, ls);
    cout << show(zipped) << " (Esperado: [(1, A), (2, B), (3, C), (4, D), (5, E)])" << endl;

    // 15. dropEveryN
    cout << "15. dropEveryN (n=2): ";
    cout << show(dropEveryN(2, l4)) << " (Esperado: [1, 3, 5])" << endl;

    // 16. sortBy
    cout << "16. sortBy (Descendente): ";
    List<int> desordenada = List<int>().cons(1).cons(4).cons(2).cons(5).cons(3);
    auto ordenado = sortBy(desordenada, [](int a, int b){ return a >= b; });
    cout << show(ordenado) << " (Esperado: [5, 4, 3, 2, 1])" << endl;

    // 17. rotateByN
    cout << "17. rotateByN (n=2): ";
    cout << show(rotateByN(l4, 2)) << " (Esperado: [3, 4, 5, 1, 2])" << endl;

    cout << "===========================================" << endl;
    cout << "   TODAS LAS PRUEBAS FINALIZADAS" << endl;
    cout << "===========================================" << endl;

    return 0;
}