#include <libpq-fe.h>
#include <iostream>
#include <queue>
#include "./deleter.h"
#include "./connection.h"
#include "./pool.h"

namespace pqpool {
  pool::pool(pool_options options) {
    this->create_pool(options);
  };

  std::unique_ptr<pg_result, pqpool::deleter> pool::exec(std::string query) {
    auto conn = this->get_connection();
    auto res = PQexec(conn->pq_connection.get(), query.c_str());
    this->free_connection(conn);

    return std::unique_ptr<pg_result, pqpool::deleter>{res};
  }

  void pool::create_pool(pool_options options) {
    std::lock_guard<std::mutex> lock(mutex);

    for (auto i = 0; i < options.pool_size; ++i){
      pq_pool.emplace(std::make_shared<connection>(options.host, options.port, options.database, options.username, options.password));
    }
  }

  std::shared_ptr<connection> pool::get_connection() {
    std::unique_lock<std::mutex> lock(mutex);

    while (pq_pool.empty()){
      condition.wait(lock);
    }

    auto free_connection = pq_pool.front();
    pq_pool.pop();

    return free_connection;
  }

  void pool::free_connection(std::shared_ptr<connection> connection) {
    std::unique_lock<std::mutex> lock(mutex);
    pq_pool.push(connection);
    lock.unlock();
    condition.notify_one();
  }
}