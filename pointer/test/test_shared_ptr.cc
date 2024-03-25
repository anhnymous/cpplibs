#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "../shared_ptr.hh"

using namespace anhthd::cpplibs;

int main(int argc, char** argv)
{
  std::cout << "========================================================\n";
  shared_ptr<int> p1;
  std::cout << "p1.use_count: " << p1.use_count() << "\n";

  std::cout << "========================================================\n";
  shared_ptr<int> p2(nullptr);
  std::cout << "p2.use_count: " << p2.use_count() << "\n";

  std::cout << "========================================================\n";
  auto int_value = new int(42);
  shared_ptr<int> p3(int_value);
  fprintf(stdout, "p3 = %d\n", *p3);
  std::cout << "p3.use_count: " << p3.use_count() << "\n";

  std::cout << "========================================================\n";
  shared_ptr<int> p4(p3);
  fprintf(stdout, "p3 = %d\n", *p3);
  fprintf(stdout, "p4 = %d\n", *p4);
  std::cout << "p3.use_count: " << p3.use_count() << "\n";
  std::cout << "p4.use_count: " << p4.use_count() << "\n";

  std::cout << "========================================================\n";
  auto int_value2 = new int(90);
  shared_ptr<int> p5(int_value2);
  fprintf(stdout, "p5 = %d\n", *p5);
  std::cout << "p5.use_count: " << p5.use_count() << "\n";

  std::cout << "========================================================\n";
  shared_ptr<int> p6(std::move(p5));
  fprintf(stdout, "p6 = %d\n", *p6);
  std::cout << "p6.use_count: " << p6.use_count() << "\n";

  std::cout << "========================================================\n";
  shared_ptr<int> p7; p7 = p6;
  fprintf(stdout, "p6 = %d\n", *p6);
  std::cout << "p6.use_count: " << p6.use_count() << "\n";
  fprintf(stdout, "p7 = %d\n", *p7);
  std::cout << "p7.use_count: " << p7.use_count() << "\n";

  std::cout << "========================================================\n";
  shared_ptr<int> p8 = p7;
  fprintf(stdout, "p7 = %d\n", *p7);
  std::cout << "p7.use_count: " << p7.use_count() << "\n";
  fprintf(stdout, "p8 = %d\n", *p8);
  std::cout << "p8.use_count: " << p8.use_count() << "\n";

  std::cout << "========================================================\n";
  shared_ptr<int> p9; p9 = std::move(p7);
  fprintf(stdout, "p8 = %d\n", *p8);
  std::cout << "p8.use_count: " << p8.use_count() << "\n";
  fprintf(stdout, "p9 = %d\n", *p9);
  std::cout << "p9.use_count: " << p9.use_count() << "\n";

  std::cout << "========================================================\n";
  if (int_value) delete int_value;
  if (int_value2) delete int_value2;
  return 0;
}

























