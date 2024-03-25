/**************************************************************************************
* Simple Unique Pointer
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: unique_ptr.hh
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
#ifndef UNIQUE_PTR_H_
#define UNIQUE_PTR_H_

#include <utility>

namespace anhthd {
namespace cpplibs {
template <typename T>
class unique_ptr {
private:
  T* obj_;

public:
  template <typename _T, typename ... Args>
  friend unique_ptr<_T> make_unique(Args&& ... args);

  unique_ptr() { obj_ = nullptr; }

  explicit unique_ptr(T* _obj) noexcept: obj_(_obj) { }

  ~unique_ptr() noexcept { if (obj_) delete obj_; }

  unique_ptr(unique_ptr&& other) noexcept: obj_(other.obj_) {
    other.obj_ = nullptr;
  }

  unique_ptr& operator=(unique_ptr&& other) noexcept {
    if (this != &other) {
      delete obj_;
      obj_ = other.obj_;
      other.obj_ = nullptr;
    }
    return *this;
  }

  unique_ptr(const unique_ptr&) = delete;

  unique_ptr& operator=(const unique_ptr&) = delete;

  /**
   * Get raw pointer
   */
  T* get() const noexcept { return obj_; }

  /**
   * Abandon unique ownership and return the raw pointer
   */
  T* release() noexcept {
    T* returned_obj = obj_;
    obj_ = nullptr;
    return returned_obj;
  }

  /**
   * Release unique_ptr and assign new managed object to it
   */
  void reset(T* _obj) noexcept {
    delete obj_;
    obj_ = _obj;
  }

  T& operator*() const noexcept {
    return *obj_;
  }

  T* operator->() const noexcept {
    return obj_;
  }
};  // class unique_ptr

template <typename T, typename ... Args>
unique_ptr<T> make_unique(Args&& ... args) {
  return unique_ptr<T>(new T(std::forward<Args>(args)...));
}

};  // namespace cpplibs
};  // namespace anhthd

#endif /* UNIQUE_PTR_H_ */

