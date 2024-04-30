/*
 * file   test.cc
 * brief
 *
 *    Author: anhthd
 */

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "insertion_sort.hh"

void print(const std::vector<int>& v) {
  for (const auto& e : v) {
    std::cout << e << " ";
  } std::cout << "\n";
}

int main(int argc, char** argv)
{
  std::vector<int> vi{5, 4, 3, 2, 1};
  print(vi);
  anhthd::cpplibs::sorting::insertion(vi.begin(), vi.end());
  print(vi);

  std::vector<int> vi2{};
  print(vi2);
  anhthd::cpplibs::sorting::insertion(vi2.begin(), vi2.end());
  print(vi2);

  std::vector<int> vi3{2, 1};
  print(vi3);
  anhthd::cpplibs::sorting::insertion(vi3.begin(), vi3.end());
  print(vi3);

  return 0;
}
