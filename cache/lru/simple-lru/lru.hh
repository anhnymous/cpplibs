/***************************************************************************************
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
***************************************************************************************/
#ifndef LRU_H_
#define LRU_H_

#include <string.h>

#include <string>
#include <cstdint>
#include <optional>
#include <stdexcept>

namespace anhthd {
namespace cpplibs {
namespace cache {
using string = std::string;
class lru
{
public:
  using key_t         = string;
  using value_t       = string;
  using hashed_key_t  = std::uint8_t;

  lru(std::uint8_t cache_size_) {
    _dllc = new dllc(cache_size_);
  }

  ~lru() {
    if (_dllc) delete _dllc;
  }

  lru(lru&&) = delete;
  lru(const lru&) = delete;
  lru& operator=(lru&&) = delete;
  lru& operator=(const lru&) = delete;

  std::uint8_t size() const noexcept {
    return _dllc->size();
  }

  void put(key_t&& key_, value_t&& value_) noexcept {
    return _dllc->put(key_, value_);
  }

  std::optional<value_t> get(key_t&& key_) noexcept {
    return _dllc->get(key_);
  }

private:
  class dllc;
  dllc* _dllc;

  class dllc
  {
  private:
    struct doubly_linked_list_node;
    typedef struct doubly_linked_list_node dlln;
    struct doubly_linked_list_node {
      hashed_key_t  key;
      value_t       value;
      dlln*         next;
      dlln*         prev;
    };

    typedef struct doubly_linked_list {
      dlln* hest_prio;
      dlln* lest_prio;
      std::uint8_t dll_len;
      std::uint8_t dll_cap;
    } dll;

    /**
     * A BIG assumption here is that this hash algo impose Zero collision
     * on tiny table size, i.e. uint8_t
     */
    static std::uint8_t string_hash(const void* key, const std::uint8_t& table_size) {
      const char* ptr = (const char*)key;
      int val = 0;

      while (*ptr != '\0') {
        int tmp;
        val = (val << 4) + (*ptr);
        if ((tmp = (val & 0xf0000000)))
        {
          val = val ^ (tmp >> 24);
          val = val ^ tmp;
        }
        ptr++;
      }
      return (std::uint8_t)(val % table_size);
    }

    dll* _dll{nullptr};

    bool dll_init(dll** dll_, std::uint8_t dll_cap_) {
      if (*dll_) {
        std::cerr << "ERROR: the dll handle is not clean!\n";
        return false;
      }
      *dll_ = (dll*)malloc(sizeof(dll));
      (*dll_)->hest_prio = nullptr;
      (*dll_)->lest_prio = nullptr;
      (*dll_)->dll_len = 0;
      (*dll_)->dll_cap = dll_cap_;
      return true;
    }

    inline dlln* new_node(hashed_key_t hashed_key_, value_t value_) {
      auto n = new dlln();
      if (nullptr!= n) {
        n->key = hashed_key_;
        n->value = value_;
      };
      return n;
    }

    // Remove the last node (lest_prio) and setup new lest_prio to maintain
    // the const dll's capacity.
    inline void trim_dll(dll* dll_) {
      if (dll_->dll_len <= dll_->dll_cap) return;
      dlln* tmp = dll_->lest_prio;
      dll_->lest_prio->prev->next = nullptr;
      dll_->lest_prio = dll_->lest_prio->prev;
      tmp->prev = nullptr;
      delete tmp;
    }

    void dll_insert(dll* dll_, key_t key_, value_t value_) {
      auto hashed_key = string_hash(key_.c_str(), dll_->dll_cap);
      if (dll_->dll_len == 0) {
        dlln* node = new_node(hashed_key, value_);
        node->prev = node->next = nullptr;
        dll_->hest_prio = node;
        dll_->lest_prio = node;
        dll_->dll_len++;
      } else if (dll_->dll_len == 1) {
        if (hashed_key == dll_->hest_prio->key) return;
        dlln* node = new_node(hashed_key, value_);
        node->prev = nullptr;
        node->next = dll_->lest_prio;
        dll_->lest_prio->prev = node;
        dll_->hest_prio = node;
        dll_->dll_len++;
        trim_dll(dll_);
      } else {
        if (hashed_key == dll_->hest_prio->key) return;
        dlln* probe = dll_->hest_prio->next;
        while (probe) {
          if (hashed_key == probe->key) break;
          probe = probe->next;
        }
        if (nullptr == probe) {
          dlln* node = new_node(hashed_key, value_);
          node->next = dll_->hest_prio;
          node->prev = nullptr;
          dll_->hest_prio->prev = node;
          dll_->hest_prio = node;
          dll_->dll_len++;
          trim_dll(dll_);
        } else {
          probe->prev->next = probe->next;
          if (probe->next) probe->next->prev = probe->prev;
          else dll_->lest_prio = probe->prev;
          probe->prev = nullptr;
          probe->next = dll_->hest_prio;
          dll_->hest_prio->prev = probe;
          dll_->hest_prio = probe;
        }
      }
    }

    dlln* dll_lookup(dll* dll_, key_t key_) {
      if (!dll_->dll_len) return nullptr;

      auto hashed_key = string_hash(key_.c_str(), dll_->dll_cap);
      if (hashed_key == dll_->hest_prio->key) return dll_->hest_prio;

      dlln* probe = dll_->hest_prio->next;;
      while (probe) {
        if (hashed_key == probe->key) {
          break;
        }
        probe = probe->next;
      }

      if (!probe) return nullptr;

      // Move the probe (found node) to hest_prio position
      probe->prev->next = probe->next;
      if (probe->next) probe->next->prev = probe->prev;
      probe->prev = nullptr;
      probe->next = dll_->hest_prio;
      dll_->hest_prio->prev = probe;
      dll_->hest_prio = probe;
      return probe;
    }

    void dll_deinit(dll* dll_) {
      if (!dll_) return;
      dlln* probe = dll_->hest_prio;
      while (probe) {
        dlln* tmp = probe;
        probe = probe->next;
        delete tmp;
      }
      free(dll_);
    }

  public:
    dllc(std::uint8_t dll_len_) {
      if (!dll_init(&_dll, dll_len_)) {
        throw std::runtime_error("dllc: failed to init a dll");
      }
    }

    ~dllc() { dll_deinit(_dll); }

    std::uint8_t size() const noexcept {
      return _dll->dll_len;
    }

    void put(key_t key_, value_t value_) noexcept {
      dll_insert(_dll, key_, value_);
    }

    std::optional<value_t> get(key_t key_) noexcept {
      auto lk = dll_lookup(_dll, key_);
      if (!lk) {
        return std::nullopt;
      } else {
        return std::optional<value_t>(lk->value);
      }
    }
  };
};
};  // namespace cache
};  // namespace cpplibs
};  // namespace anhthd

#endif /* LRU_H_ */
