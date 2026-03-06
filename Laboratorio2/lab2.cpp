#include "lab2.hpp"
#include <vector>
#include <string>
#include <iostream>
#include "extras.hpp"
#include <thread>
#include <chrono>
#include <future>
#include <functional>
#include <cmath>
#include <cstdio>


using namespace std;

vector<int> v1 = {4,3,5,1,2};
vector<int> v2 = {5,8,9,10};
List<int> l1 = extras::from_vector(v1);

int test_reduceLeft()
{
  auto to_id = [](List<int> acc, int x) {return extras::snoc(acc,x);};

  List<int> l1p = reduceLeft(to_id, List<int>(), l1);
  int sum = reduceLeft([](int acc, int x) { return acc + x; }, 0, l1);
  List<int> l2p = reduceLeft([](List<int> acc, int x) { return acc.cons(x); }, List<int>(), l1);
  cout << "l1p: " << show(l1p) << ", should be: " << show(l1) << endl;
  cout << "l2p: " << show(l2p) << ", should be: " << show(extras::reverse(l1)) << endl;
  cout << "sum: " << sum << ", should be: " << 15 << endl;

  double score = (l1p == l1) + (l2p == extras::reverse(l1)) + (sum == 15);
  score = score / 3.0;

  return score;
}

int test_map()
{
  auto id = [](int x) -> int { return x; };
  auto p2 = [](int x) { return 2+x; };
  auto t5 = [](int x) { return 5*x; };
  auto p2t5 = [&t5,&p2](int x) { return t5(p2(x)); };
  List<int> l1p = map<int,int,function<int(int)>>(id, l1);
  List<int> l1p2 = map<int,int,function<int(int)>>(p2t5, l1);
  List<int> l1p2_ = map<int,int,function<int(int)>>(t5, map<int,int,function<int(int)>>(p2, l1));

  cout << "l1p: " << show(l1p) << ", should be: " << show(l1) << endl;
  cout << "l1p2: " << show(l1p2) << ", should be: " << show(l1p2_) << endl;

  double score = (l1p == l1) + (l1p2 == l1p2_);
  score = score / 2.0;

  return score;

}

int test_concat()
{
  List<List<int>> xss  = extras::from_vector<List<int>>(vector<List<int>>{
      List<int>(),
      extras::from_vector<int>(vector<int>{4,3}),
      extras::from_vector<int>(vector<int>{5}),
      List<int>(),
      extras::from_vector<int>(vector<int>{1,2})
      })  ;
  cout << "xss: " << show(concat(xss)) << ", should be: " << show(l1) << endl;

  double score = (concat(xss) == l1);
  score = score / 1.0;
  return score;
}

int test_filter()
{
  auto even = [](int x) { return x % 2 == 0; };
  auto constT = []( int x ) { return true; };
  auto constF = []( int x ) { return false; };
  auto gt2    = []( int x ) { return x > 2; };
  auto evengt2 = [&gt2, &even]( int x ) { return gt2(x) && even(x); };

  List<int> evens   = filter(even, l1);
  List<int> constTs = filter(constT, l1);
  List<int> constFs = filter(constF, l1);
  List<int> gt2s    = filter(gt2, l1);
  List<int> evengt2s = filter(evengt2, l1);
  List<int> evengt2s_ = filter(even, filter(gt2, l1));
  List<int> evengt2s__ = filter(gt2, filter(even, l1));

  cout << "evens: "
       << show(evens)
       << ", should be: "
       << show(extras::from_vector(vector<int>{4,2}))
       << endl;

  cout << "constTs: "
       << show(constTs)
       << ", should be: "
       << show(l1)
       << endl;

  cout << "constFs: "
        << show(constFs)
        << ", should be: "
        << show(List<int>())
        << endl;

  cout << "gt2s: "
        << show(gt2s)
        << ", should be: "
        << show(extras::from_vector(vector<int>{4,3,5}))
        << endl;

  cout << "evengt2s: "
        << show(evengt2s)
        << ", should be: "
        << show(extras::from_vector(vector<int>{4}))
        << endl;

  cout << "evengt2s_: "
        << show(evengt2s_)
        << ", should be: "
        << show(extras::from_vector(vector<int>{4}))
        << endl;

  cout << "evengt2s__: "
        << show(evengt2s__)
        << ", should be: "
        << show(extras::from_vector(vector<int>{4}))
        << endl;

  double score = (evens == extras::from_vector(vector<int>{4,2})) +
                 (constTs == l1) +
                 (constFs == List<int>()) +
                 (gt2s == extras::from_vector(vector<int>{4,3,5})) +
                 (evengt2s == extras::from_vector(vector<int>{4})) +
                 (evengt2s_ == extras::from_vector(vector<int>{4})) +
                 (evengt2s__ == extras::from_vector(vector<int>{4}));

  score = score / 7.0;
  return score;
}

int test_groupEqual(){
  List<int> l1p = extras::from_vector(vector<int>{1,2,2,3,3,3,4,4,4,4,5});
  List<List<int>> l1pg = groupEqual(l1p);
  List<List<int>> l1pg_ = extras::from_vector<List<int>>(vector<List<int>>{
      extras::from_vector(vector<int>{1}),
      extras::from_vector(vector<int>{2,2}),
      extras::from_vector(vector<int>{3,3,3}),
      extras::from_vector(vector<int>{4,4,4,4}),
      extras::from_vector(vector<int>{5})
      });
  cout << "l1pg: " << show(l1pg) << ", should be: " << show(l1pg_) << endl;

  double score = (l1pg == l1pg_);
  score = score / 1.0;
  return score;
}

int test_encode(){
  List<int> l1p = extras::from_vector(vector<int>{1,2,2,3,3,3,4,4,4,4,5});
  List<std::tuple<int,int>> l1pe = encode(l1p);
  List<std::tuple<int,int>> l1pe_ = extras::from_vector<std::tuple<int,int>>(vector<std::tuple<int,int>>{
      std::make_tuple(1,1),
      std::make_tuple(2,2),
      std::make_tuple(3,3),
      std::make_tuple(4,4),
      std::make_tuple(5,1)
      });
  cout << "l1pe: " << show(l1pe) << ", should be: " << show(l1pe_) << endl;
  double score = (l1pe == l1pe_);
  score = score / 1.0;
  return score;
}

int test_zip(){
  List<int> l1p = extras::from_vector(vector<int>{1,2,3});
  List<int> l2p = extras::from_vector(vector<int>{4,5,6});
  List<std::tuple<int,int>> zipped = zip(l1p, l2p);
  List<std::tuple<int,int>> zipped_ = extras::from_vector<std::tuple<int,int>>(vector<std::tuple<int,int>>{
      std::make_tuple(1,4),
      std::make_tuple(2,5),
      std::make_tuple(3,6)
      });
  List<int> l3p = extras::from_vector(vector<int>{7,8});
  List<std::tuple<int,int>> zipped2 = zip(l1p, l3p);
  List<std::tuple<int,int>> zipped2_ = extras::from_vector<std::tuple<int,int>>(vector<std::tuple<int,int>>{
      std::make_tuple(1,7),
      std::make_tuple(2,8)
      });
  List<int> l4p = extras::from_vector(vector<int>{9,10,11,12});
  List<std::tuple<int,int>> zipped3 = zip(l1p, l4p);
  List<std::tuple<int,int>> zipped3_ = extras::from_vector<std::tuple<int,int>>(vector<std::tuple<int,int>>{
      std::make_tuple(1,9),
      std::make_tuple(2,10),
      std::make_tuple(3,11)
      });


  cout << "zipped: " << show(zipped) << ", should be: " << show(zipped_) << endl;
  cout << "zipped2: " << show(zipped2) << ", should be: " << show(zipped2_) << endl;
  cout << "zipped3: " << show(zipped3) << ", should be: " << show(zipped3_) << endl;

  double score = (zipped == zipped_) + (zipped2 == zipped2_) + (zipped3 == zipped3_);
  score = score / 3.0;
  return score;
}

int test_dropEveryN()
{
  List<int> l1p = extras::from_vector(vector<int>{1,2,3,4,5,6,7,8,9});
  List<int> dropped = dropEveryN(3, l1p);
  List<int> dropped_ = extras::from_vector(vector<int>{1,2,4,5,7,8});
  List<int> dropped2 = dropEveryN(1, l1p);
  List<int> dropped2_ = extras::from_vector(vector<int>());
  List<int> dropped3 = dropEveryN(0, l1p);
  List<int> dropped3_ = extras::from_vector(vector<int>{1,2,3,4,5,6,7,8,9});
  List<int> dropped4 = dropEveryN(1000, l1p);
  List<int> dropped4_ = extras::from_vector(vector<int>{1,2,3,4,5,6,7,8,9});

  cout << "dropped: " << show(dropped) << ", should be: " << show(dropped_) << endl;
  cout << "dropped2: " << show(dropped2) << ", should be: " << show(dropped2_) << endl;
  cout << "dropped3: " << show(dropped3) << ", should be: " << show(dropped3_) << endl;
  cout << "dropped4: " << show(dropped4) << ", should be: " << show(dropped4_) << endl;

  double score = (dropped == dropped_) + (dropped2 == dropped2_) + (dropped3 == dropped3_) + (dropped4 == dropped4_);
  score = score / 4.0;
  return score;
}

int test_sortBy()
{
  List<int> l1p = extras::from_vector(vector<int>{4,3,5,1,2});
  auto gt = [](int x, int y) { return x > y; };
  auto lt = [](int x, int y) { return x < y; };
  List<int> sorted = sortBy(l1p, lt);
  List<int> sorted_ = extras::from_vector(vector<int>{1,2,3,4,5});
  List<int> sorted2 = sortBy(l1p, gt);
  List<int> sorted2_ = extras::from_vector(vector<int>{5,4,3,2,1});
  List<int> sorted3  = sortBy(List<int>(), lt);
  List<int> sorted3_ = extras::from_vector(vector<int>());
  List<int> sorted4  = sortBy(sorted, lt);
  List<int> sorted4_ = extras::from_vector(vector<int>{1,2,3,4,5});

  cout << "sorted: " << show(sorted) << ", should be: " << show(sorted_) << endl;
  cout << "sorted2: " << show(sorted2) << ", should be: " << show(sorted2_) << endl;
  cout << "sorted3: " << show(sorted3) << ", should be: " << show(sorted3_) << endl;
  cout << "sorted4: " << show(sorted4) << ", should be: " << show(sorted4_) << endl;

  double score = (sorted == sorted_) + (sorted2 == sorted2_) + (sorted3 == sorted3_) + (sorted4 == sorted4_);
  score = score / 4.0;
  return score;
}

int test_rotateByN(){

  List<int> l1p = extras::from_vector(vector<int>{1,2,3,4,5});
  List<int> rotated = rotateByN(l1p, 0);
  List<int> rotated_ = extras::from_vector(vector<int>{1,2,3,4,5});
  List<int> rotated2 = rotateByN(l1p, 1);
  List<int> rotated2_ = extras::from_vector(vector<int>{5,1,2,3,4});
  List<int> rotated3 = rotateByN(l1p, 2);
  List<int> rotated3_ = extras::from_vector(vector<int>{4,5,1,2,3});
  List<int> rotated4 = rotateByN(l1p, 4);
  List<int> rotated4_ = extras::from_vector(vector<int>{2,3,4,5,1});
  List<int> rotated5 = rotateByN(l1p, 5);
  List<int> rotated5_ = extras::from_vector(vector<int>{1,2,3,4,5});
  List<int> rotated6 = rotateByN(l1p, 10);
  List<int> rotated6_ = extras::from_vector(vector<int>{1,2,3,4,5});

  cout << "rotated: " << show(rotated) << ", should be: " << show(rotated_) << endl;
  cout << "rotated2: " << show(rotated2) << ", should be: " << show(rotated2_) << endl;
  cout << "rotated3: " << show(rotated3) << ", should be: " << show(rotated3_) << endl;
  cout << "rotated4: " << show(rotated4) << ", should be: " << show(rotated4_) << endl;
  cout << "rotated5: " << show(rotated5) << ", should be: " << show(rotated5_) << endl;
  cout << "rotated6: " << show(rotated6) << ", should be: " << show(rotated6_) << endl;


  double score = (rotated == rotated_) + (rotated2 == rotated2_) + (rotated3 == rotated3_) + (rotated4 == rotated4_) + (rotated5 == rotated5_) + (rotated6 == rotated6_);
  score = score / 6.0;

  return score;
}


int safely_execute_test(function<int()> test_function, int timeout_seconds = 5)
{
    auto future = std::async(std::launch::async, test_function);
    if (future.wait_for(std::chrono::seconds(timeout_seconds)) == std::future_status::ready) {
        return future.get();
    } else {
        std::cerr << "Test timed out after " << timeout_seconds << " seconds." << std::endl;
        return 0; // Return 0 score if the test times out
    }
}

int main()
{
  double score = 0;
  score += safely_execute_test(test_reduceLeft, 10);
  score += safely_execute_test(test_map, 10);
  score += safely_execute_test(test_concat, 10);
  score += safely_execute_test(test_filter, 10);
  score += safely_execute_test(test_groupEqual, 10);
  score += safely_execute_test(test_encode, 10);
  score += safely_execute_test(test_zip, 10);
  score += safely_execute_test(test_dropEveryN, 10);
  score += safely_execute_test(test_sortBy, 10);
  score += safely_execute_test(test_rotateByN, 10);

  score = score * 10.0;

  cout << "Total score: " << score << endl;
  FILE* file_ptr = fopen("notas.txt", "w");

  fprintf(file_ptr, "c++: %.3f\n", score);

  return 0;
}
