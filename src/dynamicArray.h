#ifndef _DYNAMIC_ARRAY_H_
#define _DYNAMIC_ARRAY_H_

template <class T>
class Node
{
public:
  T *value;

  Node(T *v)
  {
    this->value = v;
  };

  Node<T> *next = nullptr;
  Node<T> *prev = nullptr;

  ~Node()
  {
    if (value != nullptr) delete value;
  }
};

template <class T>
class DynamicArray
{
private:
  Node<T> *start = nullptr;
  Node<T> *last = nullptr;

  int _size = 0;

  // Получить Узел по индексу
  Node<T> *_nodeFromIndex(unsigned char index)
  {
    if (index >= _size)
      return nullptr;

    unsigned char count = 0;
    Node<T> *currentNode = start;

    while (count < index)
    {
      currentNode = currentNode->next;
      count++;
    }

    return currentNode;
  }

public:
  // Добавить по индексу
  void addTo(unsigned char index, T *value)
  {
    if (index > _size)
      throw "Incorrect index";

    Node<T> *newNode = new Node<T>(value);

    if (_size == 0)
    {
      start = newNode;
      last = newNode;
    }
    else if (index == 0)
    {
      newNode->next = start;
      start->prev = newNode;
      start = newNode;
    }
    else if (index == _size)
    {
      last->next = newNode;
      newNode->prev = last;
      last = newNode;
    }
    else
    {
      Node<T> *movedNode = this->_nodeFromIndex(index);
      Node<T> *prevMovedNode = movedNode->prev;
      newNode->next = movedNode;
      newNode->prev = prevMovedNode;
      movedNode->prev = newNode;
      prevMovedNode->next = newNode;
    }

    _size++;
  };

  // Удалить по индексу
  void deleteFrom(unsigned char index)
  {
    if (index >= _size)
      throw "Incorrect index";

    Node<T> *delNode;

    if (_size == 1)
    {
      delNode = start;
      start = nullptr;
      last = nullptr;
    }
    else if (index == 0)
    {
      delNode = start;
      start = start->next;
      start->prev = nullptr;
    }
    else if (index == _size - 1)
    {
      delNode = last;
      last = last->prev;
      last->next = nullptr;
    }
    else
    {
      delNode = _nodeFromIndex(index);
      delNode->prev->next = delNode->next;
      delNode->next->prev = delNode->prev;
    }
    _size--;
    delete delNode;
  }

  // Добавить в конец
  void addLast(T *value)
  {
    addTo(_size, value);
  };

  // Добавить в начало
  void addFirst(T *value)
  {
    addTo(0, value);
  }

  // Удалить последний
  void deleteLast()
  {
    deleteFrom(_size - 1);
  };

  // Удалить первый
  void deleteFirst()
  {
    deleteFrom(0);
  }

  // Сменить значение в элементе с индексом...
  void change(unsigned char index, T *value)
  {
    if (index > _size) throw "Incorrect Index";
    
    Node<T> *currentNode = _nodeFromIndex(index);
    
    //Если нужного узла нет, записываем в конец
    if (currentNode == nullptr){
      addLast(value);
    } 

    // Если есть меняем его значение
    else {
    delete currentNode->value;
    currentNode->value = value;
    }
  }

  // Очистить весь массив
  void clear()
  {
    Node<T> *delNode = start;

    while (delNode != nullptr)
    {
      start = start->next;
      delete delNode;
      delNode = start;
    }

    last = nullptr;
    _size = 0;

    // while (_size != 0) {
    //   deleteFirst();
    // }
  }

  // Количество элементов в массиве
  int length() { return _size; };

  // Получить элемент по индексу
  T &operator[](unsigned char index)
  {
    Node<T> *node = _nodeFromIndex(index);

    if (node == nullptr)
      throw "Incorrect index";

    return *(node->value);
  };

  ~DynamicArray()
  {
    clear();
  }
};

#endif