/**************************************************************************************
* Quick Sort
*
* COPYRIGHT: (c) 2022 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: quick_sort.hh
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
#ifndef QUICK_SORT_H_
#define QUICK_SORT_H_

#include <cstdlib>
#include <iterator>

#include "../insertion-sort/insertion_sort.hh"

namespace anhthd {
namespace cpplibs {
namespace sorting {
template <class random_it__>
static long int
partition(random_it__ first_, random_it__ last_, long int s_, long int e_) {
  auto x = std::rand() % (e_ - s_ + 1);
  auto y = std::rand() % (e_ - s_ + 1);
  auto z = std::rand() % (e_ - s_ + 1);
  std::vector<long int> m{x, y, z};
  sorting::insertion(m.begin(), m.end());
  long int p = *std::next(first_, m[1]);

  auto s = s_; s_--;
  auto e = e_; e_++;

  while (true) {
    do { s_++; }
    while (*std::next(first_, (s_ - s)) < p);
    do { e_--; }
    while (*std::prev(last_, (e - e_)) > p);

    if (s_ >= e_) {
      break;
    }
    else {
      std::swap(*std::next(first_, s_ - s), *std::prev(last_, e - e_));
    }
  }

  return e_;
}

namespace detail {
template <class random_it__>
bool quick_sort(random_it__ first_, random_it__ last_, long int s_, long int e_) {
  while (s_ < e_) {
    auto p = partition(first_ + s_, first_ + e_, s_, e_);
    if (p < 0) {
      return false;
    }
    if (!quick_sort(first_, last_, s_, p)) {
      return false;
    }
    s_ = p + 1;
  }
  return true;
}
};  // namespace detail

template <class random_it__>
bool quick(random_it__ first_, random_it__ last_) {
  auto d = std::distance(first_, last_);
  auto s = d - d; auto e = d - 1;
  return sorting::detail::quick_sort(first_, last_ - 1, s, e);
}
};  // namespace sorting
};  // namespace cpplibs
};  // namespace anhthd

#endif /* QUICK_SORT_H_ */
