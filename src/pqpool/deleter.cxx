#include <libpq-fe.h>
#include <iostream>
#include "./deleter.h"

namespace pqpool {
  void deleter::operator() (pg_result *result) {
    PQclear(result);
  }
}