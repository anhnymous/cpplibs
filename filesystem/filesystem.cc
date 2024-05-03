/**************************************************************************************
* File System utils
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: filesystem.cc
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
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <queue>
#include <fstream>
#include <stdexcept>

#include "filesystem.hh"

namespace anhthd {
namespace cpplibs {
namespace filesystem {
#define SIZE 1024

static std::queue<char> strip_slash(const char* s_, size_t length_)
{
  std::queue<char> tmp_queue{};

  if (!s_ || !length_) {
    return tmp_queue;
  }

  bool slash_counter = false;
  size_t i;

  for (i = 0; i < length_; ++i) {
    if ('/' == s_[i] && !slash_counter) {
      tmp_queue.emplace(s_[i]);
      slash_counter |= true;
    } else if ('/' == s_[i]) {
      ;
    } else {
      tmp_queue.emplace(s_[i]);
      slash_counter &= false;;
    }
  }

  return tmp_queue;
}

path:: path(void) {}

path::path(const char* p_)
{
  if (!p_) {
    _start = NULL;
    _length = 0;
  } else {
    _length = strlen(p_);
    auto stripped = strip_slash(p_, _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
}

path::path(string&& p_)
{
  _length = p_.length();
  auto stripped = strip_slash(p_.c_str(), _length);
  _length = stripped.size();
  _start = (char*)malloc(sizeof(char) * (_length + 1));
  memset((void*)_start, '\0', (_length + 1));
  for (size_t i = 0; i < _length; ++i) {
    _start[i] = stripped.front();
    stripped.pop();
  }
}

path::path(const string& p_)
{
  _length = p_.length();
  auto stripped = strip_slash(p_.c_str(), _length);
  _length = stripped.size();
  _start = (char*)malloc(sizeof(char) * (_length + 1));
  memset((void*)_start, '\0', (_length + 1));
  for (size_t i = 0; i < _length; ++i) {
    _start[i] = stripped.front();
    stripped.pop();
  }
}

path::path(path&& p_)
{
  _length = p_._length;
  if (_length) {
    auto stripped = strip_slash(p_.raw(), _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
}

path::path(const path& p_)
{
  _length = p_._length;
  if (_length) {
    auto stripped = strip_slash(p_.raw(), _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
}

path& path::operator=(path&& p_)
{
  _length = p_._length;
  if (_length) {
    auto stripped = strip_slash(p_.raw(), _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
  return *this;
}

path& path::operator=(const path& p_)
{
  _length = p_._length;
  if (_length) {
    auto stripped = strip_slash(p_.raw(), _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
  return *this;
}

path& path::operator=(string&& p_)
{
  _length = p_.length();
  if (_length) {
    auto stripped = strip_slash(p_.c_str(), _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
  return *this;
}

path& path::operator=(const char* p_)
{
  if (!p_) {
    _start = NULL;
    _length = 0;
  } else {
    _length = strlen(p_);
    auto stripped = strip_slash(p_, _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
  return *this;
}

path& path::operator=(const string& p_)
{
  _length = p_.length();
  if (_length) {
    auto stripped = strip_slash(p_.c_str(), _length);
    _length = stripped.size();
    _start = (char*)malloc(sizeof(char) * (_length + 1));
    memset((void*)_start, '\0', (_length + 1));
    for (size_t i = 0; i < _length; ++i) {
      _start[i] = stripped.front();
      stripped.pop();
    }
  }
  return *this;
}

path::~path()
{
  if (_start) free(_start);
  _length = 0;
}

const char* path::raw() const noexcept
{
  return _start;
}

size_t path::length() const noexcept
{
  return _length;
}

path path::operator/(const path& p_)
{
  size_t l = 0;
  char* path_ = NULL;

  if (this->_start[this->_length - 1] != '/' && p_._start[0] != '/') {
    l = this->_length + p_._length + 1;
    path_ = (char*)malloc(sizeof(char) * (l + 1));
    memset(path_, '\0', (l + 1));
    path_[this->_length] = '/';
    memcpy(path_, this->_start, this->_length);
    memcpy(path_ + this->_length + 1, p_._start, p_._length);
  } else {
    l = this->_length + p_._length;
    path_ = (char*)malloc(sizeof(char) * (l + 1));
    memcpy(path_, this->_start, this->_length);
    memcpy(path_ + this->_length, p_._start, p_._length);
  }

  auto _q = strip_slash(path_, l);
  path _rp;
  _rp._length = _q.size();
  _rp._start = (char*)malloc(sizeof(char) * (_rp._length + 1));
  memset(_rp._start, '\0', (_rp._length + 1));

  for (size_t i = 0; i < _rp._length; ++i) {
    _rp._start[i] = _q.front();
    _q.pop();
  }

  free(path_);
  return _rp;
}

void operator<<(std::ostream& os_, path& p_)
{
  (void)os_;
  fprintf(stdout, "%s\n", p_._start);
}

bool is_file_exists(const path& p_, error_code& ec)
{
  std::ifstream f(p_.raw());
  return f.good();
}

bool is_directory(const path& p_, error_code& ec_)
{
  errno = 0;
  DIR* dir = opendir(p_.raw());
  if (dir) {
    closedir(dir);
    return true;
  } else {
    ec_.set_err_msg(strerror(errno));
    return false;
  }
}

bool is_regular_file(const path& p_, error_code& ec_)
{
  errno = 0;
  struct stat path_stat;
  if (-1 == stat(p_.raw(), &path_stat)) {
    ec_.set_err_msg(strerror(errno));
    return false;
  } else {
    return S_ISREG(path_stat.st_mode);
  }
}

bool is_symbolic_link(const path& p_, error_code& ec_)
{
  errno = 0;
  struct stat path_stat;
  if (-1 == lstat(p_.raw(), &path_stat)) {
    ec_.set_err_msg(strerror(errno));
    return false;
  } else {
    return S_ISLNK(path_stat.st_mode);
  }
}

bool copy_file(const path& s_, const path& d_, error_code& ec_)
{
  if (!is_regular_file(s_, ec_)) {
    ec_.set_err_msg("source file is not a regular file");
    return false;
  }

  FILE* sfp = fopen(s_.raw(), "rb");
  if (!sfp) {
    ec_.set_err_msg("cannot open the source file");
    return false;
  }

  FILE* dfp = fopen(d_.raw(), "wb");
  if (!dfp) {
    ec_.set_err_msg("cannot open the dest file to write");
    fclose(sfp);
    return false;
  }

  struct stat fst;
  if (-1 == fstat(fileno(sfp), &fst)) {
    ec_.set_err_msg("cannot get source file's stats");
    fclose(sfp);
    fclose(dfp);
    return false;
  }

  fseek(sfp, 0L, SEEK_END);
  long src_len = ftell(sfp);
  rewind(sfp);

  char buf[SIZE];
  size_t rc = SIZE;
  size_t wc = 0;
  long wroten_len = 0;
  long origin_len = src_len;
  while (rc > 0) {
    memset(buf, '\0', SIZE * sizeof(char));
    rc = fread(buf, sizeof(char), SIZE, sfp);
    src_len -= (long)rc;
    wc = fwrite(buf, sizeof(char), rc, dfp);
    wroten_len += (long)wc;
  }

  if (wroten_len != origin_len) {
    ec_.set_err_msg("dest file and and source file have different sizes");
    fclose(sfp);
    fclose(dfp);
    return false;
  }

  int rc1 __attribute__((unused)) = fchown(fileno(dfp), fst.st_uid, fst.st_gid);
  int rc2 __attribute__((unused)) = fchmod(fileno(dfp), fst.st_mode);
  fclose(sfp);
  fclose(dfp);
  return true;
}
};  // namespace filesystem
};  // namespace cpplibs
};  // namespace anhthd
