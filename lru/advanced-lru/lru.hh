/**************************************************************************************
* LRU: Last Recently Used (Cache)
*
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: lru.hh
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
#ifndef LRU_H_
#define LRU_H_

#include <stdlib.h>

#include <string>
#include <optional>
#include <stdexcept>
#include <functional>

namespace anhthd {
namespace cpplibs {
template <typename _Kp, typename _Tp, std::size_t _Nm>
class lru
{
public:
  typedef _Kp                                   key_type;
  typedef _Tp                                   value_type;
  typedef value_type*                           pointer;
  typedef const value_type*                     const_pointer;
  typedef value_type&                           reference;
  typedef const value_type&                     const_reference;
  typedef value_type*                           iterator;
  typedef const value_type*                     const_iterator;
  typedef std::size_t                           size_type;
  typedef std::ptrdiff_t                        difference_type;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::size_t                           hashed_key_type;

  /**
   * Get the current size of LRU
   */
  constexpr size_type
  size() const noexcept { return _Nm; }

  /**
   * Put key-value into cache
   */
  void put(key_type&& _key, value_type&& _value) noexcept {
    return dllc_->put(std::move(_key), std::move(_value));
  }

  /**
   * Look for the value associated with a key from cache
   */
  std::optional<value_type> get(key_type&& _key) noexcept {
    return dllc_->get(_key);
  }

  lru() {
    try {
      dllc_ = new dllc(_Nm);
    } catch (const std::runtime_error& re) {
      throw re;
    }
  }

  ~lru() { if (dllc_) delete dllc_; }

  lru(lru&&) = delete;
  lru(const lru&) = delete;
  lru& operator=(lru&&) = delete;
  lru& operator=(const lru&) = delete;

private:
  class dllc;
  dllc* dllc_{nullptr};

  class dllc {
  private:
    struct doubly_linked_list_node;
    typedef struct doubly_linked_list_node dlln;
    struct doubly_linked_list_node {
      hashed_key_type   key;
      value_type        value;
      dlln*             next;
      dlln*             prev;
    };

    typedef struct doubly_linked_list {
      dlln*     hest_prio;  ///! Highest priority
      dlln*     lest_prio;  ///! Lowest priority
      size_type dll_size;   ///! DDL current size
      size_type dll_cap;    ///! DDL capacity
    } dll;

    dll* dll_{nullptr};

    static bool dll_init(dll** _dll, std::size_t _capacity) {
      if (*_dll) {
        fprintf(stderr, "ERROR: The dll handle is not clean!");
        return false;
      }
      (*_dll) = (dll*)malloc(sizeof(dll));
      (*_dll)->hest_prio = nullptr;
      (*_dll)->lest_prio = nullptr;
      (*_dll)->dll_size  = 0;
      (*_dll)->dll_cap   = _capacity;
      return true;
    }

    static std::size_t string_hash(const char* _key, std::size_t table_size) {
      std::size_t hash = 5381;
      std::size_t c;
      while ((c = *_key++))
        hash = ((hash << 5) + hash) + (std::size_t)c;
      return (hash % table_size);
    }

    static inline dlln* new_dll_node(hashed_key_type _hk, value_type _v) {
      auto nn = new dlln();
      if (nullptr != nn) {
        nn->key = _hk;
        nn->value = _v;
      }
      return nn;
    }

    static inline void trim_dll(dll* _dll) {
      if (!_dll) return;
      if (_dll->dll_size <= _dll->dll_cap) return;
      dlln* tmp = _dll->lest_prio;
      _dll->lest_prio->prev->next = nullptr;
      _dll->lest_prio = _dll->lest_prio->prev;
      tmp->prev = nullptr;
      delete tmp;
    }

    static void dll_insert(dll* _dll, key_type&& _key, value_type&& _value) {
      auto hashed_key = string_hash(_key.c_str(), _dll->dll_cap);
      if (_dll->dll_size == 0) { // Insert new node into an empty dll
        dlln* node = nullptr;
        node = new_dll_node(hashed_key, _value);
        node->prev = node->next = nullptr;
        _dll->hest_prio = node;
        _dll->lest_prio = node;
        _dll->dll_size++;
        trim_dll(_dll);
      } else if(_dll->dll_size == 1) {  // The current dll has 1 node
        // The implementation does not allow the same key associated different values.
        if (hashed_key == _dll->hest_prio->key) return;

        dlln* node = new_dll_node(hashed_key, _value);
        node->prev = nullptr;
        node->next = _dll->lest_prio;
        _dll->lest_prio->prev = node;
        _dll->hest_prio = node;
        _dll->dll_size++;
        trim_dll(_dll);
      } else { // The current dll has more than 1 node
        // The implementation does not allow the same key associated different values.
        if (hashed_key == _dll->hest_prio->key) return;

        dlln* probe = _dll->hest_prio->next;
        while (probe) {
          if (hashed_key == probe->key) break;
          probe = probe->next;
        }

        if (nullptr == probe) { // Not found, so insert a new node
          dlln* node = new_dll_node(hashed_key, _value);
          node->next = _dll->hest_prio;
          node->prev = nullptr;
          _dll->hest_prio->prev = node;
          _dll->hest_prio = node;
          _dll->dll_size++;
          trim_dll(_dll);
        } else {  // Found, so move the found node to hest_prio
          probe->prev->next = probe->next;
          if (probe->next) probe->next->prev = probe->prev;
          else _dll->lest_prio = probe->prev;
          probe->prev = nullptr;
          probe->next = _dll->hest_prio;
          _dll->hest_prio->prev = probe;
          _dll->hest_prio = probe;
        }
      }
    }

    static dlln* dll_lookup(dll* _dll, key_type _key) {
      if (!_dll || !_dll->dll_size) return nullptr;

      auto hashed_key = string_hash(_key.c_str(), _dll->dll_cap);
      if (hashed_key == _dll->hest_prio->key) return _dll->hest_prio;

      dlln* probe = _dll->hest_prio->next;
      while (probe) {
        if (probe->key == hashed_key) break;
        probe = probe->next;
      }

      if (!probe) return nullptr;

      // Move the probe (found node) to hest_prio
      probe->prev->next = probe->next;
      if (probe->next) probe->next->prev = probe->prev;
      probe->prev = nullptr;
      probe->next = _dll->hest_prio;
      _dll->hest_prio->prev = probe;
      _dll->hest_prio = probe;
      return probe;
    }

    static void dll_deinit(dll* _dll) {
      if (!_dll) return;
      dlln* probe = _dll->hest_prio;
      while (probe) {
        dlln* tmp = probe;
        probe = probe->next;
        delete tmp;
      }
      free(_dll);
    }

  public:
    dllc(std::size_t _capacity) {
      if (!dll_init(&dll_, _capacity)) {
        throw std::runtime_error("ERROR: failed to initialize dll");
      }
    }

    ~dllc() { dll_deinit(dll_); }

    void put(key_type&& _key, value_type&& _value) noexcept {
      dll_insert(dll_, std::move(_key), std::move(_value));
    }

    std::optional<value_type> get(key_type _key) noexcept {
      auto lk = dll_lookup(dll_, _key);
      if (!lk) {
        return std::nullopt;
      } else {
        return std::optional<value_type>(lk->value);
      }
    }
  };
};
};  // namespace cpplibs
};  // namespace anhthd

#endif /* LRU_H_ */
