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
  anhthd::cpplibs::lru l(3);
  fprintf(stdout, "LRU size: %d\n", l.size());

  l.put("a", "aa");
  l.put("b", "bb");
  l.put("c", "cc");
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

  // KNOWN INSSUE: hashing collision
  auto d = l.get("d");
  if (d) {
    std::cout << "Found: " << *d << std::endl;
  }

  return 0;
}
