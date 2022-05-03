#ifndef LIST_H
#define LIST_H

template<typename...>
struct List;

template <typename T, typename... TT>
struct List<T, TT...>
{
    using head = T;
    using next = List<TT...>;
    static constexpr int size = sizeof...(TT) + 1;
};

template<>
struct List<>
{
    using head = void;
    using next = void;
    static constexpr int size = 0;
};



template <typename...>
struct PrependList;

template <typename T, template <typename...> class List, typename... TT>
struct PrependList<T, List<TT...>>
{
    using list = List<T, TT...>;
};



template <int N, typename...>
struct GetAtIndex;

template <int N, template <typename...> class List, typename T, typename... TT>
struct GetAtIndex<N, List<T, TT...>>
{
    using value = typename GetAtIndex<N-1, List<TT...>>::value;
};

template <template <typename...> class List, typename T, typename... TT>
struct GetAtIndex<0, List<T, TT...>>
{
    using value = T;
};



template <int N, typename...>
struct SetAtIndex;

template <int N, template <typename...> class List, typename NT, typename T, typename... TT>
struct SetAtIndex<N, NT, List<T, TT...>>
{
    using list = typename PrependList<T, typename SetAtIndex<N - 1, NT, List<TT...>>::list>::list;
};

template <template <typename...> class List, typename NT, typename T, typename... TT>
struct SetAtIndex<0, NT, List<T, TT...>>
{
    using list = typename PrependList<NT, typename List<T, TT...>::next>::list;
};


#endif
