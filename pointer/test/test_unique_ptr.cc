#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "../unique_ptr.hh"

int main(int argc, char** argv)
{
  anhthd::cpplibs::unique_ptr<int> p1;

  auto upi = anhthd::cpplibs::make_unique<int>(10);
  std::cout << "upi = " << *upi << std::endl;

  auto raw = upi.get();
  std::cout << "raw = " << *raw << std::endl;

  //p1 = upi; <- FAILED

  auto another = std::move(upi);
  std::cout << "another = " << *another << std::endl;

  return 0;
}
