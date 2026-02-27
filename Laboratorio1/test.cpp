#include <cstdio>
#include <string>
#include "lab1.hpp"
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <cmath>

using namespace std;

// vector de origen en 4 dimensiones
double _o[] = { 0.0, 0.0, 0.0, 0.0 };

double _a[] = { 5.0, 5.0, 5.0, 5.0 };
double _b[] = { 2.2, 0.0, 1.1, 3.3 };
double _c[] = { 5.6, 9.8, 3.9, 5.7 };
double _x[] = { 1.0, 2.1, 3.0, 5.1, 2.0, 0.0, 6.2, 7.3 };
double _y[] = { 154564, -165458, 0, 12.154 , -0.00002, 145.04411 };

const int _No = sizeof(_o) / sizeof(_o[0]);
const int _Nx = sizeof(_x) / sizeof(_x[0]);
const int _Ny = sizeof(_y) / sizeof(_y[0]);

auto comp_arrs = [](int n, double a[], double b[]){
    for(int i = 0; i < n; ++i){
      if(a[i] != b[i])
        return false;

    }
    return true;
  };

double my_distance(int n, double v[], double w[]){
  double result = 0;
  double temp;
  for(int i = 0; i < n; ++i){
    temp = v[i] - w[i];
    result += temp*temp;
  }
  result = sqrt(result);
  return result;
}

double t_distance(int n, double v[], double w[]){

  future<double> future = async(launch::async, [n,v,w](){return distance(n,v,w);});
  future_status status = future.wait_for(chrono::seconds(2));
  if (status == std::future_status::ready)
    return future.get();
  return -1;
}

void my_vec_sum(int n, double v[], double w[], double r[]) {
  for (int i = 0; i < n; ++i)
    r[i] = v[i] +  w[i];

}

bool t_vec_sum(int n, double v[], double w[], double r[]) {
  future<void> future = async(launch::async, [n,v,w,r](){vec_sum(n,v,w,r);});
  future_status status = future.wait_for(chrono::seconds(2));
  if (status == std::future_status::ready)
    return true;

  return false;

}

int my_min(int i, int n, double a[]){
  int min_index = i;
  for(int j = i + 1; j < n; ++j){
    if(a[j] < a[min_index]){
      min_index = j;
    }
  }
  return min_index;
}

int t_min(int i, int n, double a[]){
  future<int> future = async(launch::async, [i,n,a](){return min(i,n,a);});
  future_status status = future.wait_for(chrono::seconds(2));
  if (status == std::future_status::ready)
    return future.get();
  return -1;
}

void my_sort(const int n, double a[], double r []){
   copy_array(n, a, r);
  int min_index;
  double temp;
  for (int i = 0; i < n; ++i){
    min_index = my_min(i, n, r);
    temp = r[i];
    r[i] = r[min_index];
    r[min_index] = temp;
  }
}

bool t_sort(const int n, double a[], double r []){
  future<void> future = async(launch::async, [n,a,r](){sort(n,a,r);});
  future_status status = future.wait_for(chrono::seconds(2));
  if (status == std::future_status::ready)
    return true;
  return false;

}

bool t_sort_AI(const int n, double a[], double r []){
  future<void> future = async(launch::async, [n,a,r](){sort_AI(n,a,r);});
  future_status status = future.wait_for(chrono::seconds(2));
  if (status == std::future_status::ready)
    return true;
  return false;

}

int _test_distance(){
  fprintf(stdout, "running test_distance...\n");
  bool tests [] =
  {  my_distance(_No, _a, _o) == t_distance(_No, _a, _o)
  ,  my_distance(_No, _o, _a) == t_distance(_No, _o, _a)
  ,  my_distance(_No, _b, _c) == t_distance(_No, _b, _c)
  ,  my_distance(_No, _c, _b) == t_distance(_No, _c, _b)
  ,  my_distance(_Nx, _x, _x) == t_distance(_Nx, _x, _x)
  };

  const int Ntest = sizeof(tests) / sizeof(tests[0]);

  int passed = 0;

  for (int i = 0; i < Ntest; ++i){
    if(tests[i])
      passed++;

  }
  return passed;
}

int _test_vec_sum(){
  fprintf(stdout, "running vec_distance...\n");
  double r0 [] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  double r1 [] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  auto test = [](int n, double v[], double w[], double r0[], double r1[]){
    my_vec_sum(n, v, w, r0);
    return t_vec_sum(n, v, w, r1)? comp_arrs(n, r0, r1) : false;
  };
  bool tests [] =
  {  test(_No, _a, _o, r0, r1)
  ,  test(_No, _o, _a, r0, r1)
  ,  test(_No, _b, _c, r0, r1)
  ,  test(_No, _c, _b, r0, r1)
  ,  test(_Nx, _x, _x, r0, r1)
  };

  const int Ntest = sizeof(tests) / sizeof(tests[0]);

  int passed = 0;

  for (int i = 0; i < Ntest; ++i){
    if(tests[i]){
      passed++;
    }
  }
  return passed;
}

int _test_min(){
  fprintf(stdout, "running min...\n");
  bool tests [] =
  { my_min(0, _No, _a) == t_min(0, _No, _a)
  , my_min(0, _No, _b) == t_min(0, _No, _b)
  , my_min(0, _No, _c) == t_min(0, _No, _c)
  , my_min(0, _Nx, _x) == t_min(0, _Nx, _x)
  , my_min(2, _Nx, _x) == t_min(2, _Nx, _x)
  };

  const int Ntest = sizeof(tests) / sizeof(tests[0]);

  int passed = 0;

  for (int i = 0; i < Ntest; ++i){
    if(tests[i])
      passed++;

  }
  return passed;
}

int _test_sort(){
  fprintf(stdout, "running sort...\n");
  double r0 [] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  double r1 [] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  auto test = [](int n, double a[], double r0[], double r1[]){
    my_sort(n, a, r0);
    return t_sort(n, a, r1)? comp_arrs(n, r0, r1) : false;
  };
  bool tests [] =
  {  test(_No, _a, r0, r1)
  ,  test(_No, _b, r0, r1)
  ,  test(_No, _c, r0, r1)
  ,  test(_Nx, _x, r0, r1)
  ,  test(_Ny, _y, r0, r1)
  };

  const int Ntest = sizeof(tests) / sizeof(tests[0]);
  int passed = 0;

  for (int i = 0; i < Ntest; ++i){
    if(tests[i])
      passed++;

  }
  return passed;
}

int _test_sort_AI(){
  fprintf(stdout, "running sort_AI...\n");
  double r0 [] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  double r1 [] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  auto test = [](int n, double a[], double r0[], double r1[]){
    my_sort(n, a, r0);
    return t_sort_AI(n, a, r1)? comp_arrs(n, r0, r1) : false;
  };
  bool tests [] =
  {  test(_No, _a, r0, r1)
  ,  test(_No, _b, r0, r1)
  ,  test(_No, _c, r0, r1)
  ,  test(_Nx, _x, r0, r1)
  ,  test(_Ny, _y, r0, r1)
  };

  const int Ntest = sizeof(tests) / sizeof(tests[0]);
  int passed = 0;

  for (int i = 0; i < Ntest; ++i){
    if(tests[i])
      passed++;

  }
  return passed;
}



int main(){
  int passed_dist = _test_distance();
  fprintf(stdout, "test_distance: %d/%d\n", passed_dist, 5);
  int passed_vec_sum = _test_vec_sum();
  fprintf(stdout, "test_vec_sum: %d/%d\n", passed_vec_sum, 5);
  int passed_min = _test_min();
  fprintf(stdout, "test_min: %d/%d\n", passed_min, 5);
  int passed_sort = _test_sort();
  fprintf(stdout, "test_sort: %d/%d\n", passed_sort, 5);
  int passed_sort_AI = _test_sort_AI();
  fprintf(stdout, "test_sort_AI: %d/%d\n", passed_sort_AI, 5);

  double total_score = double(passed_dist + passed_vec_sum + passed_min + passed_sort + passed_sort_AI) / 25.0 * 50.0;
  FILE* file_ptr = fopen("notas.txt", "w");
  if(file_ptr == nullptr){
    fprintf(stderr, "Error al abrir el archivo notas.txt\n");
    return 1;
  }
  fprintf(file_ptr, "c++: %.3f\n", total_score);
  return 0;
}
