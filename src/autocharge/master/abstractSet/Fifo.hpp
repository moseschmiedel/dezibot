#pragma once

#include "AbstractSet.hpp"
#include <queue>
#include <unordered_map>

template <typename T, typename ID>
class Fifo final : public AbstractSet<T, ID> {
private:
  std::queue<T> queue;
  std::unordered_map<ID, T *> idMap;

public:
  void insert(ID id, T &item) override {
    if (idMap.find(id) == idMap.end()) {
      queue.push(item);
      idMap[id] = &queue.back();
    }
  }

  T *pick() override {
    if (queue.empty()) {
      return nullptr;
    }
    T &item = queue.front();
    queue.pop();
    idMap.erase(item.getId());
    return &item;
  }

  T *get(ID id) override {
    if (idMap.find(id) != idMap.end()) {
      return idMap[id];
    }
    return nullptr;
  }

  bool isEmpty() const override { return queue.empty(); }
};
