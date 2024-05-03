/*
 * file   test_lru.cc
 * brief
 *
 *    Author: anhthd
 */

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "lru.hh"

using string = std::string;
int main(int argc, char** argv)
{
  anhthd::cpplibs::cache::lru<string, string, 5> l;
  fprintf(stdout, "LRU size: %ld\n", l.size());

  l.put("a", "aa");
  fprintf(stdout, "Put(a), LRU size: %ld\n", l.size());
  l.put("b", "bb");
  fprintf(stdout, "Put(b), LRU size: %ld\n", l.size());
  l.put("c", "cc");
  fprintf(stdout, "Put(c), LRU size: %ld\n", l.size());

  auto a = l.get("a");
  if (a) {
    std::cout << "Found(a): " << *a << std::endl;
  }
  auto b = l.get("b");
  if (b) {
    std::cout << "Found(b): " << *b << std::endl;
  }
  auto c = l.get("c");
  if (c) {
    std::cout << "Found(c): " << *c << std::endl;
  }

  return 0;
}
