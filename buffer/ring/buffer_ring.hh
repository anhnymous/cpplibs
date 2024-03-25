/**************************************************************************************
* Buffer Ring (Circular Buffer)
* COPYRIGHT: (c) 2023 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: buffer_ring.hh
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
#ifndef BUFFER_RING_H_
#define BUFFER_RING_H_

template <<F6>
class buffer_ring
{
public:
  buffer_ring() = default;
  ~buffer_ring() = default;

  buffer_ring(buffer_ring&&) = delete;
  buffer_ring(const buffer_ring&) = delete;
  buffer_ring& operator=(buffer_ring&&) = delete;
  buffer_ring& operator=(const buffer_ring&) = delete;

private:

};

#endif /* BUFFER_RING_H_ */
