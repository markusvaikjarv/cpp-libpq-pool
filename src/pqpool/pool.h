#ifndef PQPOOL_POOL_H
#define PQPOOL_POOL_H

#include <cstdint>
#include <libpq-fe.h>
#include <iostream>
#include <queue>
#include "./deleter.h"
#include "./connection.h"

namespace pqpool {
  struct pool_options {
    std::uint8_t pool_size = std::uint8_t(10);
    std::string host;
    std::uint16_t port;
    std::string database;
    std::string username;
    std::string password;
  };

  struct pool {
  public:
    pool(pool_options);
    std::unique_ptr<pg_result, pqpool::deleter> exec(std::string);
  private:
    std::mutex mutex;
    std::condition_variable condition;
    std::queue<std::shared_ptr<connection> > pq_pool;

    void create_pool(pool_options);

    std::shared_ptr<connection> get_connection();

    void free_connection(std::shared_ptr<connection>);
  };
}

#endif