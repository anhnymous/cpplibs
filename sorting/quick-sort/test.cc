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

#include "quick_sort.hh"

void print_vector(const std::vector<int>& v) {
  for (const auto& _v : v) {
    std::cout << _v << " ";
  } std::cout << "\n";
}

int main(int argc, char** argv)
{
  auto succeed = false;

  std::vector<int> vi{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  print_vector(vi);
  succeed = anhthd::cpplibs::sorting::quick(vi.begin(), vi.end());
  if (!succeed) {
    std::cout << "Sorting vi failed\n";
  }
  print_vector(vi);

  std::vector<int> vi2{9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  print_vector(vi2);
  succeed = anhthd::cpplibs::sorting::quick(vi2.begin(), vi2.end());
  if (!succeed) {
    std::cout << "Sorting vi2 failed\n";
  }
  print_vector(vi2);

  return 0;
}

