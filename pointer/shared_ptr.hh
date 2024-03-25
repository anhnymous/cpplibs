/**************************************************************************************
* Simple Shared Pointer
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: shared_ptr.hh
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
#ifndef SHARED_PTR_H_
#define SHARED_PTR_H_

#include <atomic>

namespace anhthd {
namespace cpplibs {
template <typename T>
class shared_ptr {
private:
  template <typename _T>
  struct control_block {
    std::atomic<std::uint32_t> ref_count{0};
    _T* mem_block{nullptr};
  };

private:
  control_block<T>* ctr_blk_{nullptr};

public:
  template <typename _T, typename ... Args>
  friend shared_ptr<_T> make_shared(Args&& ... args);

  ~shared_ptr() {
    if (ctr_blk_ != nullptr) {
      if (ctr_blk_->ref_count == 0) {
        if (ctr_blk_->mem_block) {
          delete ctr_blk_->mem_block;
          ctr_blk_->mem_block = nullptr;
        }
        delete ctr_blk_; ctr_blk_ = nullptr;
      } else if (ctr_blk_->ref_count == 1) {
        if (ctr_blk_->mem_block) {
          delete ctr_blk_->mem_block;
          ctr_blk_->mem_block = nullptr;
          ctr_blk_->ref_count--;
        }
        delete ctr_blk_; ctr_blk_ = nullptr;
      } else {
        ctr_blk_->ref_count--;
        ctr_blk_->mem_block = nullptr;
        ctr_blk_ = nullptr;
      }
    }
  }

  shared_ptr() noexcept {
    ctr_blk_ = new control_block<T>();
    ctr_blk_->ref_count = 0;
    ctr_blk_->mem_block = nullptr;
  }

  shared_ptr(T* _obj) noexcept {
    ctr_blk_ = new control_block<T>();
    ctr_blk_->mem_block = _obj;
    if (_obj) ctr_blk_->ref_count = 1;
  }

  shared_ptr(shared_ptr& other) {
    if (this != &other) {
      ctr_blk_ = other.ctr_blk_;
      if (other.ctr_blk_->mem_block) other.ctr_blk_->ref_count++;
    }
  }

  shared_ptr(shared_ptr&& other) {
    if (this != &other) {
      ctr_blk_ = new control_block<T>();
      if (other.ctr_blk_->mem_block) {
        ctr_blk_->mem_block = other.ctr_blk_->mem_block;
        ctr_blk_->ref_count = other.ctr_blk_->ref_count.load();
      } else {
        ctr_blk_->mem_block = nullptr;
        ctr_blk_->ref_count = 0;
      }

      other.ctr_blk_->ref_count = 0;
      other.ctr_blk_->mem_block = nullptr;
      delete other.ctr_blk_; other.ctr_blk_ = nullptr;
    }
  }

  shared_ptr& operator=(const shared_ptr& other) {
    if (this != &other) {
      if (ctr_blk_->ref_count == 0) {
        if (ctr_blk_->mem_block) {
          delete ctr_blk_->mem_block;
          ctr_blk_->mem_block = nullptr;
        }
        delete ctr_blk_; ctr_blk_ = nullptr;
      } else if (ctr_blk_->ref_count == 1) {
        if (ctr_blk_->mem_block) {
          delete ctr_blk_->mem_block;
          ctr_blk_->mem_block = nullptr;
          ctr_blk_->ref_count--;
        }
        delete ctr_blk_; ctr_blk_ = nullptr;
      } else {
        ctr_blk_->ref_count--;
        ctr_blk_->mem_block = nullptr;
        ctr_blk_ = nullptr;
      }

      ctr_blk_ = other.ctr_blk_;
      if (other.ctr_blk_->mem_block) other.ctr_blk_->ref_count++;
    }
    return *this;
  }

  shared_ptr& operator=(shared_ptr&& other) {
    if (this != &other) {
      if (ctr_blk_->ref_count == 0) {
        if (ctr_blk_->mem_block) {
          delete ctr_blk_->mem_block;
          ctr_blk_->mem_block = nullptr;
        }
        delete ctr_blk_; ctr_blk_ = nullptr;
      } else if (ctr_blk_->ref_count == 1) {
        if (ctr_blk_->mem_block) {
          delete ctr_blk_->mem_block;
          ctr_blk_->mem_block = nullptr;
          ctr_blk_->ref_count--;
        }
        delete ctr_blk_; ctr_blk_ = nullptr;
      } else {
        ctr_blk_->ref_count--;
        ctr_blk_->mem_block = nullptr;
        ctr_blk_ = nullptr;
      }

      ctr_blk_ = other.ctr_blk_;
      other.ctr_blk_ = nullptr;
    }
    return *this;
  }

  T* get() const noexcept { return ctr_blk_->mem_block; }

  int use_count() const noexcept { return ctr_blk_->ref_count; }

  T& operator*() const noexcept { return *(ctr_blk_->mem_block); }

  T* operator->() const noexcept { return ctr_blk_->mem_block; }

};  // class shared_ptr

template <typename T, typename ... Args>
shared_ptr<T> make_shared(Args&& ... args) {
  return shared_ptr<T>(new T(std::forward<Args>(args)...));
}

};  // namespace cpplibs
};  //namespace anhthd

#endif /* SHARED_PTR_H_ */
