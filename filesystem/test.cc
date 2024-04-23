#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "filesystem.hh"
namespace fs = anhthd::cpplibs::filesystem;

int main(int argc, char** argv)
{
  /*******************************************************************************
   * Test basic Constructors and Assignment operators
   *******************************************************************************/
  fs::path c1{"/tmp/c1////"};
  std::cout << c1;

  fs::path c2{string{"////tmp/////c2/"}};
  std::cout << c2;

  const string c3_s = "/tmp/c3//////";
  fs::path c3{c3_s};
  std::cout << c3;

  fs::path c4{fs::path{"//tmp///c4///"}};
  std::cout << c4;

  fs::path c5_temp{"/tmp//c5//"};
  fs::path c5{c5_temp};
  std::cout << c5;

  fs::path a1 = fs::path{"/tmp/a1//"};
  std::cout << a1;

  const fs::path a2_temp{"///tmp/a2//"};
  fs::path a2 = a2_temp;
  std::cout << a2;

  fs::path a3 = string{"/tmp///a3/////"};
  std::cout << a3;

  fs::path a4 = "////////tmp///a4//";
  std::cout << a4;

  string a5_temp = "/tmp///a5//";
  fs::path a5 = a5_temp;
  std::cout << a5;

  auto c12 = c1/c2;
  std::cout << c12;

  /*******************************************************************************
   * Test basic interfaces: is_directory, is_regular_file, copy_file
   *******************************************************************************/
  fs::error_code ec;
  fs::path dir{"./dir-exist//"};
  fs::path file{"./file-exist"};
  fs::path symlink_file{"./symlink-file-exist"};

  if (fs::is_directory(dir, ec)) {
    fprintf(stdout, "%s is dir\n", dir.raw());
  } else {
    fprintf(stdout, "%s is NOT dir\n", dir.raw());
  }
  if (fs::is_regular_file(dir, ec)) {
    fprintf(stdout, "%s is regular file\n", dir.raw());
  } else {
    fprintf(stdout, "%s is NOT regular file\n", dir.raw());
  }

  if (fs::is_directory(file, ec)) {
    fprintf(stdout, "%s is directory\n", file.raw());
  } else {
    fprintf(stdout, "%s is NOT directory\n", file.raw());
  }
  if (fs::is_regular_file(file, ec)) {
    fprintf(stdout, "%s is regular file\n", file.raw());
  } else {
    fprintf(stdout, "%s is NOT regular file\n", file.raw());
  }

  if (fs::is_regular_file(symlink_file, ec)) {
    fprintf(stdout, "%s is regular file\n", symlink_file.raw());
  } else {
    fprintf(stdout, "%s is NOT regular file\n", symlink_file.raw());
  }
  if (fs::is_symbolic_link(symlink_file, ec)) {
    fprintf(stdout, "%s is symbolic link\n", symlink_file.raw());
  } else {
    fprintf(stdout, "%s is NOT symbolic link\n", symlink_file.raw());
  }

  fs::path src_bin = "src.bin_file";
  fs::path dst_bin = "dst.bin_file";
  if (!fs::copy_file(src_bin, dst_bin, ec)) {
    fprintf(stdout, "copy file failed, err: %s\n", ec.what());;
  } else {
    fprintf(stdout, "copy file OK\n");;
  }

  fs::path a = "/tmp";
  fs::path b = "abc";
  auto ab = a/b;
  std::cout << ab;

  fs::path test{"./test.ccc"};
  if (fs::is_file_exists(test, ec)) {
    std::cout << "File exists\n";
  }
  return 0;
}
