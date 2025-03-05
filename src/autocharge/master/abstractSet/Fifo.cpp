#include "AbstractSet.hpp"
#include <optional>
#include <queue>

template <typename T> class Fifo final : public AbstractSet<T> {
private:
  std::queue<T> queue;

public:
  void insert(T item) override { queue.push(item); }

  std::optional<T> pick() override {
    if (queue.empty()) {
      return std::nullopt;
    }
    T item = queue.front();
    queue.pop();
    return item;
  }

  bool isEmpty() const override { return queue.empty(); }
};
