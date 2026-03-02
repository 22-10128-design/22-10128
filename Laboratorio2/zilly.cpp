#include <iostream>
#include <functional>
#define ifZ(b,t,f) (b ? t : f)

using namespace std;
using namespace std::placeholders;
typedef int Z;

/*
Sintaxis de lambda funciones:

auto nombre = [captura](parametros) -> tipo_retorno { cuerpo };

*/

auto lt  =  [](Z b, Z a) { return a < b; };
auto sub = [](Z b, Z a) { return a - b; };
// auto ifZ = [](Z b, Z a, Z c) { return b ? a : c; };
auto chs = [](Z a) { return sub(a,0); };
auto add = [](Z b, Z a) { return sub(chs(b),a); };
auto eq  = [](Z b, Z a) {
 return ifZ
   ( lt(b,a)
   , 0
   , ifZ(lt(a,b), 0, 1 )
   );
};
auto orZ  = [](Z b, Z a) { return ifZ(a, 1, b); };
auto andZ = [](Z b, Z a) { return ifZ(a, b, 0); };
auto notZ = [](Z a) { return ifZ(a, 0, 1); };
auto le   = [](Z b, Z a) { return orZ(lt(b,a), eq(b,a)); };
auto gt   = [](Z b, Z a) { return not(le(b,a)); };
auto ge   = [](Z b, Z a) { return not(lt(b,a)); };
auto mulP = [](const auto&  self, Z b, Z a) -> Z {
  return ifZ
    ( eq(0,b)
    , 0
    , add(a, self(self, sub(1,b),a))
    );
};

int main() {
  auto f = bind(lt, _1, 10);
  cout << f(30) << endl;
  cout << ifZ(f(30), 1, 0) << endl;
  cout << ifZ(f(5), 1, 0) << endl;
  cout << sub(1,5) << endl;
  cout << add(1,5) << endl;
  cout << mulP(mulP,0,5) << endl;
  cout << mulP(mulP,1,5) << endl;
  cout << mulP(mulP,5,0) << endl;
  cout << mulP(mulP,5,1) << endl;
  cout << mulP(mulP,5,5) << endl;


  return 0;
}
