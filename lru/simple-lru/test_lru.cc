/*
 * file   test_lru.cc
 * brief
 *
 *  Created on:
 *    Author: anhthd
 */

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "lru.hh"

int main(int argc, char** argv)
{
  //=========================================
  //            Very Simple Test
  //=========================================
  anhthd::cpplibs::lru l(5);
  fprintf(stdout, "LRU size: %d\n", l.size());

  l.put("a", "aha");
  l.put("b", "bede");
  fprintf(stdout, "LRU size: %d\n", l.size());

  auto a = l.get("a");
  if (a) {
    std::cout << "Found: " << *a << std::endl;
  }
  auto b = l.get("b");
  if (b) {
    std::cout << "Found: " << *b << std::endl;
  }
  auto c = l.get("c");
  if (c) {
    std::cout << "Found: " << *c << std::endl;
  }

  return 0;
}
