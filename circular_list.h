/// cpp-circular-list - A barebones, header-only C++ implementation of
/// `circular_list` with STL-like iterator support.
///
/// Author: Gurtej Kanwar (https://github.com/gkanwar)
///
/// To the extent possible under law, the author(s) have dedicated all copyright
/// and related and neighboring rights to this software to the public domain
/// worldwide. This software is distributed without any warranty.  You should
/// have received a copy of the CC0 Public Domain Dedication along with this
/// software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.

#ifndef CIRCULAR_LIST
#define CIRCULAR_LIST

#include <cstddef>
#include <iterator>
#include <vector>

namespace ext_std {

template<typename T>
struct circular_list {

  struct node {
    int fwd;
    int bwd;
    T value;
    bool free;
    bool is_head;
  };

  template<bool Const = false>
  struct generic_iterator {
    using iterator_category = std::bidirectional_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = typename std::conditional<Const, T const*, T *>::type;
    using reference = typename std::conditional<Const, T const&, T &>::type;

    using list_pointer = typename std::conditional<Const, circular_list const*, circular_list *>::type;
    using node_pointer = typename std::conditional<Const, node const*, node *>::type;
    
    list_pointer list;
    int node;
    int branch;

    generic_iterator(list_pointer list, int node)
        : list(list), node(node), branch(0) {}
    generic_iterator(list_pointer list, int node, int branch)
        : list(list), node(node), branch(branch) {}

    template<bool _Const = Const>
    typename std::enable_if<_Const, node_pointer>::type
    _get_node() const {
      return &list->all_nodes[node];
    }
    template<bool _Const = Const>
    typename std::enable_if<!_Const, node_pointer>::type
    _get_node() {
      return &list->all_nodes[node];
    }

    template<bool _Const = Const>
    typename std::enable_if<_Const, reference>::type
    operator*() const {
      assert(!_get_node()->free);
      return _get_node()->value;
    }
    template<bool _Const = Const>
    typename std::enable_if<!_Const, reference>::type
    operator*() {
      assert(!_get_node()->free);
      return _get_node()->value;
    }
 
    template<bool _Const = Const>
    typename std::enable_if<_Const, pointer>::type
    operator->() const {
      assert(!_get_node()->free);
      return &_get_node()->value;
    }
    template<bool _Const = Const>
    typename std::enable_if<!_Const, pointer>::type
    operator->() {
      assert(!_get_node()->free);
      return &_get_node()->value;
    }

    generic_iterator& operator++() {
      assert(!_get_node()->free);
      assert(!list->all_nodes[_get_node()->fwd].free);
      node = _get_node()->fwd;
      if (_get_node()->is_head) {
        branch++;
      }
      return *this;
    }
    generic_iterator operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
    generic_iterator& operator--() {
      assert(!_get_node()->free);
      assert(!list->all_nodes[_get_node()->bwd].free);
      if (_get_node()->is_head) {
        branch--;
      }
      node = _get_node()->bwd;
      return *this;
    }
    generic_iterator operator--(int) {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    generic_iterator operator+(difference_type n) const {
      return std::next(*this, n);
    }
    generic_iterator operator-(difference_type n) const {
      return std::prev(*this, n);
    }

    friend bool operator==(const generic_iterator& l, const generic_iterator& r) {
      // -1 (empty iterator) does not distinguish branches
      return l.list == r.list && l.node == r.node && (l.node < 0 || l.branch == r.branch);
    }
    friend bool operator!=(const generic_iterator& l, const generic_iterator& r) {
      return !(l == r);
    }
  };

  using iterator = generic_iterator<false>;
  using const_iterator = generic_iterator<true>;

  int head;
  std::vector<node> all_nodes;

  circular_list() : head(-1), all_nodes() {}
  circular_list(const std::vector<T>& values) : all_nodes(values.size()) {
    size_t n_values = values.size();
    for (int i = 0; i < n_values; ++i) {
      all_nodes[i].value = values[i];
      all_nodes[i].fwd = (i+1) % values.size();
      all_nodes[i].bwd = (i-1+values.size()) % values.size();
      all_nodes[i].free = false;
      all_nodes[i].is_head = (i == 0);
    }
    head = 0;
  }

  iterator begin() {
    return iterator(this, head, 0);
  }
  const_iterator begin() const {
    return const_iterator(this, head, 0);
  }
  iterator end() {
    return iterator(this, head, 1);
  }
  const_iterator end() const {
    return const_iterator(this, head, 1);
  }
  const_iterator cbegin() const {
    return const_iterator(this, head, 0);
  }
  const_iterator cend() const {
    return const_iterator(this, head, 1);
  }

  bool empty() const {
    return head < 0;
  }

  size_t size() const {
    return std::distance(cbegin(), cend());
  }

  bool _list_is_consistent() const {
    for (auto it = cbegin(); it != cend(); ++it) {
      if (it == cbegin() && !it._get_node()->is_head ||
          it != cbegin() && it._get_node()->is_head) {
        return false;
      }
      if (it._get_node()->free) {
        return false;
      }
      if (it._get_node()->fwd != std::next(it).node) {
        return false;
      }
      if (it._get_node()->bwd != std::prev(it).node) {
        return false;
      }
    }
    if (!empty() && head != cbegin().node) {
      return false;
    }
    if (empty() && head >= 0) {
      return false;
    }
    return true;
  }

  iterator erase(iterator first_it, iterator last_it) {
    assert(last_it.branch == first_it.branch || last_it.branch == first_it.branch + 1);
    int before_first = all_nodes[first_it.node].bwd;
    // free nodes in [first, last)
    int cur_branch = first_it.branch;
    bool beheaded = false;
    for (int cur = first_it.node;
         cur != last_it.node || cur_branch != last_it.branch;
         cur = all_nodes[cur].fwd) {
      node& cur_node = all_nodes[cur];
      if (cur == head) {
        beheaded = true;
      }
      cur_node.free = true;
      cur_node.is_head = false;
      if (cur_node.fwd == head) {
        cur_branch++;
      }
    }
    // close the loop again (we may be connecting free'd nodes, but that is ok)
    all_nodes[last_it.node].bwd = before_first;
    all_nodes[before_first].fwd = last_it.node;

    if (beheaded) {
      if (all_nodes[last_it.node].free) {
        head = -1;
      }
      else {
        head = last_it.node;
        all_nodes[last_it.node].is_head = true;
      }
    }
    assert(_list_is_consistent());
    if (!all_nodes[last_it.node].free) {
      return last_it;
    }
    else {
      return end();
    }
  }

  // insert values BEFORE pos
  void insert(iterator pos, const std::vector<T>& values) {
    int last = all_nodes[pos.node].bwd;
    int next_free = 0;
    for (int i = 0; i < values.size(); ++i) {
      while(!all_nodes[next_free].free) {
        ++next_free;
        if (next_free == all_nodes.size()) {
          all_nodes.resize(
              all_nodes.size() + values.size() - i,
              node{ .free = true, .is_head = false });
        }
      }
      node& new_node = all_nodes[next_free];
      new_node.free = false;
      new_node.value = values[i];
      new_node.bwd = last;
      all_nodes[last].fwd = next_free;
      last = next_free;
      // special case: inserting before head
      if (all_nodes[pos.node].is_head && pos.branch == 0) {
        all_nodes[pos.node].is_head = false;
        new_node.is_head = true;
        head = next_free;
      }
    }
    all_nodes[last].fwd = pos.node;
    all_nodes[pos.node].bwd = last;
    assert(_list_is_consistent());
  }

  const T& operator[](size_t ind) const {
    return *std::next(begin(), ind);
  }
  T& operator[](size_t ind) {
    return *std::next(begin(), ind);
  }
};

template<typename T>
bool operator==(const circular_list<T>& l, const circular_list<T>& r) {
  auto l_it = l.cbegin();
  auto r_it = r.cbegin();
  for (; l_it != l.cend() && r_it != r.cend(); ++l_it, ++r_it) {
    if (*l_it != *r_it) return false;
  }
  return l_it == l.cend() && r_it == r.cend();
}

} // namespace ext_std

#endif
