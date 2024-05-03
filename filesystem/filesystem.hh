/**************************************************************************************
* File System utils
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: filesystem.hh
* License: GPLv3
*
* This program is free software: you can redistribute it and/or modify it under
* the terms of the GNU General Public License as published by the Free Software
* Foundation, either version 3 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
* PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this
* program. If not, see <https://www.gnu.org/licenses/>.
**************************************************************************************/
#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>
#include <ostream>

using string = std::string;

namespace anhthd {
namespace cpplibs {
namespace filesystem {
class path
{
public:
  path(void);
  path(const char* p_);
  path(string&& p_);
  path(const string& p_);
  path(path&& p_);
  path(const path& p_);

  path& operator=(path&& p_);
  path& operator=(const path& p_);
  path& operator=(string&& p_);
  path& operator=(const char* p_);
  path& operator=(const string& p_);

  ~path();

  const char* raw() const noexcept;
  size_t length() const noexcept;
  path operator/(const path& p);

  friend void operator<<(std::ostream& os, path& p);

private:
  char*  _start{NULL};
  size_t _length{0};
};

class error_code
{
public:
  error_code() = default;
  ~error_code() = default;

  error_code(error_code&&) = delete;
  error_code(const error_code&) = delete;
  error_code& operator=(error_code&&) = delete;
  error_code& operator=(const error_code&) = delete;

  const char* what() const noexcept { return _error_msg.c_str(); }
  void set_err_msg(const char* err_msg) { _error_msg = err_msg; }

private:
  string _error_msg{};
};

/**
 * Check if file exists
 */
bool is_file_exists(const path& p_, error_code& ec_);

/**
 * Check if a path is a directory in Linux.
 */
bool is_directory(const path& p_, error_code& ec_);

/**
 * Check if a path is a regular file in Linux.
 */
bool is_regular_file(const path& p_, error_code& ec_);

/**
 * Check if a path is a symbolic link (also a regular file) in Linux.
 */
bool is_symbolic_link(const path& p_, error_code& ec_);

/**
 * Copy file (not directory) from _s to _d and maintain file's stats.
 */
bool copy_file(const path& s_, const path& d_, error_code& ec_);
};  // namespace filesystem
};  // namespace cpplibs
};  // namespace anhthd

#endif /* FILESYSTEM_H_ */
