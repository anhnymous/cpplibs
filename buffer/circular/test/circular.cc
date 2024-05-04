/*
 * file   circular.cc
 * brief
 *
 *    Author: anhthd
 */

#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "../circular.hh"

using namespace std;

int main(int argc, char** argv)
{
  anhthd::cpplibs::buffer::circular<int> ci;
  cout << "Current capacity: " << ci.get_capacity() << endl;
  cout << "Current size: " << ci.size() << endl;
  cout << "================================================" << endl;

  ci.set_capacity(5);
  cout << "Current capacity: " << ci.get_capacity() << endl;
  cout << "Current size: " << ci.size() << endl;
  cout << "================================================" << endl;

  ci.push(0);
  ci.push(1);
  ci.push(2);
  ci.push(3);
  ci.push(4);
  cout << "Current capacity: " << ci.get_capacity() << endl;
  cout << "Current size: " << ci.size() << endl;
  cout << "================================================" << endl;

  ci.push(5);
  ci.push(6);
  ci.push(7);
  cout << "Current capacity: " << ci.get_capacity() << endl;
  cout << "Current size: " << ci.size() << endl;
  cout << "================================================" << endl;

  while (ci.size()) {
    auto v = ci.pop();
    cout << *v << endl;
  }

  return 0;
}
