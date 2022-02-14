#include <_types/_uint64_t.h>
#include <_types/_uint8_t.h>
#include <cstdint>
#include <thread>
#include <memory>
#include <string>
#include <libpq-fe.h>
#include <iostream>
#include "./pqpool/pool.h"
#include "./pqpool/connection.h"
#include "./pqpool/deleter.h"


void test(std::shared_ptr<pqpool::pool> pool)
{
  std::cout << "Sending SQL request" << std::endl;
  auto test_res = pool->exec(std::string("select pg_sleep(5)")); // Let the database wait for 5 seconds
  // test_res is a smart pointer to pg_result - https://www.postgresql.org/docs/9.3/libpq-exec.html
  std::cout << "Received response" << std::endl;
}


int main() {
  auto options = pqpool::pool_options {
    .pool_size = std::uint8_t{2},
    .host = std::string("localhost"),
    .port = std::uint16_t{5432},
    .database = std::string(""),
    .username = std::string(""),
    .password = std::string("")
  };

  auto pool = std::make_shared<pqpool::pool>(options);
  std::vector<std::shared_ptr<std::thread> > thread_vec;

  // Create 10 threads and send a query to the database from each thread at the same time.
  // The threads should resolve in chunks of 2 (or whatever is the size of the connection pool) after every 5 seconds because only 2 requests are being processed by the database at a time.
  for (size_t i = 0; i < 10 ; ++i){
    thread_vec.push_back(std::make_shared<std::thread>(std::thread(test, pool)));
  }

  for(auto &i: thread_vec) {
    i.get()->join();
  }

  return 0;
}