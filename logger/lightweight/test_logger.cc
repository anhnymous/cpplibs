#include <string>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>

#include "logger.hh"

using namespace std;
using namespace anhthd::cpplibs;
using namespace anhthd::cpplibs::logger::lightweight;

int main(int argc, char** argv)
{
  LOGGER.set_log_level(LogLevel::eTRACE);
  LOGGER.set_log_file(filesystem::path("./log.log"), 10000, 5);
  LOGGER.set_log_trace(filesystem::path("./trace.trace"), 1000, 2);
  LOGGER.enable_console();

  TRACE("This is log TRACE");
  DEBUG("This is log DEBUG");
  INFO("This is log INFO");
  WARNING("This is log WARNING");
  ERROR("This is log ERROR");
  FATAL("This is log FATAL");

  return 0;
}
