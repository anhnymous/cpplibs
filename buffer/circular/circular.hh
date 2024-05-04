/**************************************************************************************
* Circular Buffer
* COPYRIGHT: (c) 2022 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: circular.hh
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

/**
 * The circular buffer is like a fixed capacity queue.
 *
 * We can `push` as often as we want, but if the buffer is full, the oldest
 * elements will be automatically poped out the buffer. The current size of
 * the buffer can be ranging from Zero to capacity (max size).
 *
 * The circular buffer is useful when we don't want a queue that has size grows
 * arbitrarily. Like, we are receiving a data stream into buffer but some pieces
 * of data can be obsolete after a certain time frame, so they are free to be
 * overwriten.
 *
 * The circular buffer class requires data type _T must provide at least a default
 * constructor with well-defined behavior. And it also requires data type _T provides
 * well-defined copy constructor.
 */
#ifndef CIRCULAR_H_
#define CIRCULAR_H_

#include <mutex>
#include <optional>
#include <stdexcept>

namespace anhthd {
namespace cpplibs {
namespace buffer {
template <typename _T>
class circular
{
public:
  struct Node;
  typedef struct Node node_t;
  struct Node {
    _T      _value;
    node_t* _next;
    node_t* _prev;
  };

private:
  std::mutex  _lk;
  node_t*     _head;
  node_t*     _tail;

  std::size_t _size;
  std::size_t _capacity;

public:
  /**
   * Non-parameter constructor set capacity to Zero.
   */
  circular():
    _head{nullptr}, _tail{nullptr}, _size{0}, _capacity{0} { }

  /**
   * Parameterized constructor set capacity to _capacity.
   */
  circular(std::size_t capacity_):
    _head{nullptr}, _tail{nullptr}, _size{0}, _capacity{capacity_} { }

  ~circular() {
    while (_size) (void)pop();
  }

  circular(circular&&) = delete;
  circular(const circular&) = delete;
  circular& operator=(circular&&) = delete;
  circular& operator=(const circular&) = delete;

  /**
   * Set/update buffer capacity to capacity_.
   * We can update buffer capacity in runtime by using this interface.
   */
  void set_capacity(std::size_t capacity_) {
    if ((long int)capacity_ < 0) {
      throw std::invalid_argument("Set nagative capacity to buffer!");
    }
    _capacity = capacity_;
  }

  /**
   * Get the currently configured capacity of buffer.
   */
  std::size_t get_capacity() const noexcept {
    return _capacity;
  }

  /**
   * Get current size of buffer.
   */
  std::size_t size() const noexcept {
    return _size;
  }

  /**
   * Method to push new value value_ into buffer.
   * Push in at one end, pop out the other end.
   *
   * @param[in] value_ New value of type T want to push in.
   */
  void push(const _T& value_) noexcept {
    node_t* nn = new node_t;
    nn->_value = value_;

    std::lock_guard<std::mutex> lk(_lk);

    if (!_size) {
      nn->_next = nullptr;
      nn->_prev = nullptr;
      _head = nn;
      _tail = nn;
    }
    else {
      nn->_next = _head;
      nn->_prev = nullptr;
      _head->_prev = nn;
      _head = nn;
    }

    _size++;

    while (_size > _capacity) {
      node_t* tmp = _tail;
      _tail = _tail->_prev;
      _tail->_next = nullptr;
      tmp->_prev = nullptr;
      delete tmp;
      _size--;
    }
  }

  /**
   * Interface to let user deliberately pop data out of buffer.
   * Push in at one end, pop out the other end.
   */
  std::optional<_T> pop() noexcept {
    std::lock_guard<std::mutex> lk(_lk);

    auto ret = std::optional<_T>{std::nullopt};
    if (!_size) {
      return ret;
    }
    else if (_size == 1) {
      ret = _head->_value;
      _head = nullptr;
      delete _tail;
      _tail = nullptr;
    }
    else {
      ret = _tail->_value;
      node_t* tmp = _tail;
      _tail = _tail->_prev;
      _tail->_next = nullptr;
      tmp->_prev = nullptr;
      delete tmp;
    }
    _size--;
    return ret;
  }
};
};  // namespace buffer
};  // namespace cpplibs
};  // namespace anhthd

#endif /* CIRCULAR_H_ */
