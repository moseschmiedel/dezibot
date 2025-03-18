#pragma once

#include "AbstractSet.hpp"
#include <queue>

template <typename T>
class Fifo final : public AbstractSet<T> {
private:
  std::queue<T> queue;

public:
  void insert(T &item) override {
      queue.push(item);
  }

  T *pick() override {
    if (queue.empty()) {
      return nullptr;
    }
    T &item = queue.front();
    queue.pop();

    return &item;
  }

  bool isEmpty() const override { return queue.empty(); }
};
