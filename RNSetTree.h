#ifndef RN_SET_TREE_H
#define RN_SET_TREE_H

#include <set>
#include <list>
#include <cstddef>

template <typename T = int>
class RNSetTree {
public:
  using size_type = std::size_t;

  RNSetTree() = default;

  bool insert(const T& value) {
    return data_.insert(value).second;
  }
  bool erase(const T& value) {
    return data_.erase(value) > 0;
  }
  size_type size() const noexcept { return data_.size(); }
  bool empty() const noexcept { return data_.empty(); }

  // Exporta los elementos en orden (inorden) a una lista
  void inorder(std::list<T>& out) const {
    out.clear();
    for (const auto& v : data_) out.push_back(v);
  }

private:
  std::set<T> data_; // std::set (árbol RN de la STL)
};

#endif // RN_SET_TREE_H
