#include <libpq-fe.h>
#include <iostream>
#include "./connection.h"

namespace pqpool {
  connection::connection(std::string host, std::uint16_t port, std::string database, std::string username, std::string password) {
    this->pq_connection.reset(
      PQsetdbLogin(
        host.c_str(),
        std::to_string(port).c_str(),
        nullptr,
        nullptr,
        database.c_str(),
        username.c_str(),
        password.c_str()),
      &PQfinish );

    if (PQstatus(pq_connection.get()) != CONNECTION_OK) {
      throw std::runtime_error(PQerrorMessage(pq_connection.get()));
    }
  };
}
