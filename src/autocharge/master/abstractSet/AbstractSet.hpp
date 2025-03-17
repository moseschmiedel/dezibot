#pragma once

template <typename T, typename ID> class AbstractSet {
public:
  virtual void insert(ID id, T& item) = 0;
  virtual T* pick() = 0;
  virtual T* get(ID id) = 0;
  virtual bool isEmpty() const = 0;
  virtual ~AbstractSet() = default;
};
