/**************************************************************************************
* Lightweight Logger
* COPYRIGHT: (c) 2022 Anh Tran
* Author: Anh Tran (anhthd2017@gmail.com)
* File: logger.hh
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
#ifndef LIGHTWEIGHT_H_
#define LIGHTWEIGHT_H_

#include <stdarg.h>
#include <string.h>

#include <ctime>
#include <mutex>
#include <string>
#include <chrono>
#include <memory>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

#include "../../filesystem/filesystem.hh"

using namespace std::chrono;

namespace anhthd {
namespace cpplibs {
namespace logger {
namespace lightweight {
class log_stream
{
public:
  log_stream() = default;
  virtual ~log_stream() = default;

  log_stream(log_stream&&) = delete;
  log_stream(const log_stream&) = delete;
  log_stream& operator=(log_stream&&) = delete;
  log_stream& operator=(const log_stream&) = delete;

  virtual void print_log(const std::string_view& msg_) = 0;

private:
  std::mutex _lk;
};

class console_stream: public log_stream
{
public:
  console_stream() = default;
  ~console_stream() = default;

  console_stream(console_stream&&) = delete;
  console_stream(const console_stream&) = delete;
  console_stream& operator=(console_stream&&) = delete;
  console_stream& operator=(const console_stream&) = delete;

  void print_log(const std::string_view& msg_) override {
    std::lock_guard<std::mutex> lk(_mtx);
    std::cout << msg_ << std::endl;
  }

private:
  std::mutex  _mtx;
};

class file_stream: public log_stream
{
public:
  file_stream(const filesystem::path& file_, std::uint32_t fsize_, std::uint8_t nrt_):
    _file{file_}, _fsize{fsize_}, _nrt{nrt_} {
    _fstream.open(_file.raw(), std::ios_base::app);
    if (!_fstream.is_open()) {
      std::stringstream ss;
      ss << "Cannot open file stream at " << _file.raw();
      throw std::runtime_error(ss.str());
    }
  }

  ~file_stream() {
    if (_fstream.is_open()) {
      _fstream.flush();
      _fstream.close();
    }
  }

  file_stream(file_stream&&) = delete;
  file_stream(const file_stream&) = delete;
  file_stream& operator=(file_stream&&) = delete;
  file_stream& operator=(const file_stream&) = delete;

  void print_log(const std::string_view& msg_) override {
    std::lock_guard<std::recursive_mutex> lk(_mtx);
    filesystem::error_code ec;
    auto fs = file_size(_file, ec);
    if (fs >= _fsize) {
      _rotate();
    }
    else if (fs == -1) {
      _fstream << ec.what() << std::endl;
      throw std::runtime_error(ec.what());
    }
    else {
      _fstream << msg_ << std::endl;
    }
  }

private:
  filesystem::path      _file;    ///< Log file path
  std::uint32_t         _fsize;   ///< Backup file size
  std::uint8_t          _nrt;     ///< Maximum number of rotated files
  std::ofstream         _fstream; ///< Log file stream
  std::recursive_mutex  _mtx;

  void _rotate() {
    std::lock_guard<std::recursive_mutex> lk(_mtx);
    _fstream.flush();
    _fstream.close();

    filesystem::error_code ec;
    filesystem::path bk_file(_file/filesystem::path(".bak"));
    if (!copy_file(_file, bk_file, ec)) {
      throw std::runtime_error(ec.what());
    }
    else {
      (void)std::remove(_file.raw());
      _fstream.open(_file.raw(), std::ios_base::app);
    }
  }
};

#define __FILENAME__                  \
        strrchr(__FILE__, '/') ?      \
        strrchr(__FILE__, '/') + 1 :  \
        __FILE__

#define __CONTEXT__ \
        std::string(__FILENAME__).append(":").append(std::to_string(__LINE__)).c_str()

#define VA_ARGS(...),##__VA_ARGS__

#define LOGGER anhthd::cpplibs::logger::lightweight::get_instance()

enum class LogLevel {
  eTRACE    = 0x01,
  eDEBUG    = 0x02,
  eINFO     = 0x04,
  eWARNING  = 0x08,
  eERROR    = 0x16,
  eFATAL    = 0x32
};

#define LOG_LEVEL(level, fmt, ...) \
  try { \
    LOGGER.print_log(level, __CONTEXT__, fmt VA_ARGS(__VA_ARGS__)); \
  } catch (const std::system_error& err) { \
    (void)err; \
  } catch (...) { \
    ; \
  } \

#define TRACE(fmt, ...) \
  LOG_LEVEL(anhthd::cpplibs::logger::lightweight::LogLevel::eTRACE, \
      fmt VA_ARGS(__VA_ARGS__))
#define DEBUG(fmt, ...) \
  LOG_LEVEL(anhthd::cpplibs::logger::lightweight::LogLevel::eDEBUG, \
      fmt VA_ARGS(__VA_ARGS__))
#define INFO(fmt, ...) \
  LOG_LEVEL(anhthd::cpplibs::logger::lightweight::LogLevel::eINFO, \
      fmt VA_ARGS(__VA_ARGS__))
#define WARNING(fmt, ...) \
  LOG_LEVEL(anhthd::cpplibs::logger::lightweight::LogLevel::eWARNING, \
      fmt VA_ARGS(__VA_ARGS__))
#define ERROR(fmt, ...) \
  LOG_LEVEL(anhthd::cpplibs::logger::lightweight::LogLevel::eERROR, \
      fmt VA_ARGS(__VA_ARGS__))
#define FATAL(fmt, ...) \
  LOG_LEVEL(anhthd::cpplibs::logger::lightweight::LogLevel::eFATAL, \
      fmt VA_ARGS(__VA_ARGS__))

static const auto g_start = std::chrono::high_resolution_clock::now();
static const std::unordered_map<LogLevel, const char*> LogLevelStr = {
  {LogLevel::eTRACE,    "TRACE"},
  {LogLevel::eDEBUG,    "DEBUG"},
  {LogLevel::eINFO,     "INFO"},
  {LogLevel::eWARNING,  "WARNING"},
  {LogLevel::eERROR,    "ERROR"},
  {LogLevel::eFATAL,    "FATAL"},
};

class log_formater
{
private:
  static double exect() {
    auto n = high_resolution_clock::now();
    return ((double)duration_cast<microseconds>(n - g_start).count()/1e6);
  }

public:
  static inline std::string
  format(LogLevel lvl_, const char* ctx_, const std::string_view& msg_) {
    auto ss = std::stringstream{};
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    ss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S") << std::setfill(' ');
    //ss << " | " << std::setfill(' ') << std::setw(10) << exect() << "s";
    ss << " | " << std::setfill(' ') << std::setw(07)  << LogLevelStr.at(lvl_);
    ss << " | " << std::setfill(' ') << std::setw(30) << ctx_;
    ss << " | " << msg_;
    return ss.str();
  }

private:
  log_formater() = default;
  ~log_formater() = default;

  log_formater(log_formater&&) = delete;
  log_formater(const log_formater&) = delete;
  log_formater& operator=(log_formater&&) = delete;
  log_formater& operator=(const log_formater&) = delete;
};

#define MAX_MSG_LENGTH 512
class logger {
public:
  using log_stream_p = std::unique_ptr<log_stream>;

  /**
   * Set minimum log level.
   *
   * @param[in] min_lvl_ Specify the minimum log level.
   */
  void set_log_level(LogLevel min_lvl_) {
    _min_lvl = min_lvl_;
  }

  /**
   * Set log file path and its backup size and maximum number of backups.
   *
   * @param[in] lf_ Log file path
   * @param[in] fsize_  Log file size to make backup
   * @param[in] nrt_ Maximum number of rotated files
   */
  void set_log_file(const filesystem::path& lf_,
                    std::uint32_t fsize_,
                    std::uint8_t nrt_) {
    if ((std::int32_t)fsize_ < 0) {
      throw std::invalid_argument("Log file size cannot be negative");
    }
    if ((std::int8_t)nrt_ < 0) {
      throw std::invalid_argument("Number of rotated log files cannot be negative");
    }
    std::lock_guard<std::mutex> lk(_mtx);
    _streams.emplace_back(std::make_unique<file_stream>(lf_, fsize_, nrt_));
  }

  /**
   * Set log trace file.
   *
   * @param[in] tf_ Specify the trace file
   * @param[in] fsize_  Trace file size to make backup
   * @param[in] nrt_ Maximum number of rotated files
   */
  void set_log_trace(const filesystem::path& tf_,
                     std::uint32_t fsize_,
                     std::uint8_t nrt_) {
    if ((std::int32_t)fsize_ < 0) {
      throw std::invalid_argument("Trace file size cannot be negative");
    }
    if ((std::int8_t)nrt_ < 0) {
      throw std::invalid_argument("Number of rotated trace files cannot be negative");
    }
    std::lock_guard<std::mutex> lk(_mtx);
    _streams.emplace_back(std::make_unique<file_stream>(tf_, fsize_, nrt_));
  }

  /**
   * Enable log stream to console.
   */
  void enable_console() {
    std::lock_guard<std::mutex> lk(_mtx);
    _streams.emplace_back(std::make_unique<console_stream>());
  }

  void print_log(LogLevel lvl_, const char* ctx_, const char* fmt_, ...) {
    if (lvl_ < _min_lvl) return;

    std::lock_guard<std::mutex> lk(_mtx);
    memset(_buffer, '\0', MAX_MSG_LENGTH);
    va_list arg_list;
    va_start(arg_list, fmt_);
    vsnprintf(_buffer, MAX_MSG_LENGTH, fmt_, arg_list);
    va_end(arg_list);
    std::string_view msg_(_buffer);

    auto msg = log_formater::format(lvl_, ctx_, msg_);
    for (auto& s : _streams) {
      s->print_log(msg);
    }
  }

  /**
   * Print log message to actual available Log Stream(s)
   *
   * @param[in] msg_ Log message content
   */
  void print_log(LogLevel lvl_, const char* ctx_, const std::string_view& msg_) {
    if (lvl_ < _min_lvl) return;

    auto msg = log_formater::format(lvl_, ctx_, msg_);
    for (auto& s : _streams) {
      s->print_log(msg);
    }
  }

private:
  std::vector<log_stream_p>   _streams{};
  LogLevel                    _min_lvl{LogLevel::eINFO};
  std::mutex                  _mtx;
  char                        _buffer[MAX_MSG_LENGTH]{};;
};

static logger& get_instance() {
  static logger ins;
  return ins;
}
};  // namespace lightweight
};  // namespace logger
};  // namespace cpplibs
};  // namespace anhthd

#endif /* LIGHTWEIGHT_H_ */
