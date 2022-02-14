#ifndef PQPOOL_CONNECTION_H
#define PQPOOL_CONNECTION_H

#include <libpq-fe.h>
#include <iostream>

namespace pqpool {
  struct connection {
    public:
    std::shared_ptr<PGconn> pq_connection;

    connection(std::string, std::uint16_t, std::string, std::string, std::string);
  };
}

#endif