/*
 * Templated pop function and _Popper helper struct for class marmot::VM.
 * Implements variadic popping based on type.
 */

template <std::size_t, typename... Ts>
struct _Popper {
  typedef std::tuple<Ts...> type;

  /**
   * Worker method for popping a single element/type.
   */
  template <typename T>
  static std::tuple<T> worker(const VM & vm, const int size, const int index) {
    // Reading from the top of the stack we need to know how many we're
    // popping (size), and then we subtract the index from that to get
    // the value we want.
    const int popIndex = (-size) + index - 1;
    return std::make_tuple(vm.read<T>(popIndex));
  }

  /**
   * Worker method for popping two or more elements/types. Recurses in
   * order to preserve stack order.
   */
  template <typename T1, typename T2, typename... Rest>
  static std::tuple<T1, T2, Rest...> worker(const VM & vm, const int size, const int index) {
    // Reading from the top of the stack we need to know how many we're
    // popping (size), and then we subtract the index from that to get
    // the value we want.
    const int popIndex = (-size) + index - 1;
    std::tuple<T1> head = std::make_tuple(vm.read<T1>(popIndex));
    return std::tuple_cat(head, worker<T2, Rest...>(vm, size, index + 1));
  }

  /**
   * Performs the necessary stack popping, accumulating the popped items
   * into a tuple.
   *
   * @param  vm - the VM instacne to pop from
   * @return a tuple containing the popped elements
   */
  static type apply(VM & vm) {
    auto ret = worker<Ts...>(vm, sizeof...(Ts), 1);
    vm.pop(sizeof...(Ts));
    return ret;
  }
};

template <typename... Ts>
struct _Popper<0, Ts...> {
  using type = void;
  static type apply(VM & vm) {
    // No-op since it's a void
  }
};

// Specialization for singular type
template <typename T>
struct _Popper<1, T> {
  using type = T;
  static type apply(VM & vm) {
    // Read the top element (negative indices count from the top)
    T ret = vm.read<T>(-1);

    // Remove it from the stack
    vm.pop(1);
    return ret;
  }
};

template <typename... T>
typename _Popper<sizeof...(T), T...>::type pop() {
  return _Popper<sizeof...(T), T...>::apply(*this);
}