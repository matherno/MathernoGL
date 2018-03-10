#pragma once

#include <mathernogl/Types.h>
#include <list>
#include <map>

namespace mathernogl
  {


template <class T>
class MappedList
  {
private:
  std::list<T> items;
  std::map<uint, typename std::list<T>::iterator> itemIters;

public:
  void add(T item, uint id)
    {
    remove(id);
    items.push_back(item);
    itemIters[id] = --items.end();
    }

  void addFront(T item, uint id)
    {
    remove(id);
    items.push_front(item);
    itemIters[id] = items.begin();
    }

  void remove(uint id)
    {
    if (contains(id))
      {
      items.erase(itemIters[id]);
      itemIters.erase(id);
      }
    }

  int count() const
    {
    return (int)items.size();
    }

  bool contains(uint id) const
    {
    return itemIters.count(id) > 0;
    }

  T get(uint id)
    {
    return *itemIters[id];
    }

  T* getPtr(uint id)
    {
    return &(*itemIters[id]);
    }

  const T get(uint id) const
    {
    return *itemIters.at(id);
    }

  const T* getPtr(uint id) const
    {
    return &(*itemIters[id]);
    }

  void clear()
    {
    itemIters.clear();
    items.clear();
    }

  std::list<T>* getList()
    {
    return &items;
    }

  const std::list<T>* getList() const
    {
    return &items;
    }
  };


  }
