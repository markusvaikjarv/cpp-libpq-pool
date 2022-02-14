#ifndef PQPOOL_DELETER_H
#define PQPOOL_DELETER_H

#include <libpq-fe.h>
#include <iostream>

namespace pqpool {
  struct deleter {
    public:
    void operator() (pg_result* result);
  };
}

#endif