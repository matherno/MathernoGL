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

  void remove(uint id)
    {
    if (contains(id))
      items.erase(itemIters[id]);
    }

  int count() const
    {
    return (int)itemIters.size();
    }

  bool contains(uint id) const
    {
    return itemIters.count(id) > 0;
    }

  T get(uint id)
    {
    if (contains(id))
      return *itemIters[id];
    else
      return nullptr;
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
  };


  }
