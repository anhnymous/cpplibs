/**************************************************************************************
* Insertion Sort
*
* COPYRIGHT: (c) 2022 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: insertion_sort.hh
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
#ifndef INSERTION_SORT_H_
#define INSERTION_SORT_H_

namespace anhthd {
namespace cpplibs {
namespace sorting {
template <class random_it__>
void insertion(random_it__ first_, random_it__ last_) {
  if (first_ == last_) return;
  if (first_ == (last_ - 1)) return;
  random_it__ i = first_ + 1;
  for (; i != last_; ++i) {
    random_it__ j = i;
    while ((j - 1) >= first_ && *j < *(j - 1)) {
      std::swap(*j, *(j - 1)); --j;
    }
  }
}
};  // namespace sorting
};  // namespace cpplibs
};  // namespace anhthd

#endif /* INSERTION_SORT_H_ */
