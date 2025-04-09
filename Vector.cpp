#include "Vector.h"
#include <cstring>
#include <cmath>
Vector::Vector(const ValueType* rawArray, const size_t size, float coef)
{
    if (coef <= 1.0f)
    {
        throw std::invalid_argument("Multiplicative coef must be >= 1.0");
    }
    if (size > 0 && rawArray == nullptr)
    {
        throw std::invalid_argument("Pointer to the array is nullptr");
    }
    _size = size;
    _capacity = _size;
    _multiplicativeCoef = coef;
    _data = (_capacity > 0) ? new ValueType[_capacity] : nullptr;
    if (_size > 0)
    {
        std::memcpy(_data, rawArray, size * sizeof(ValueType));
    }
}
Vector::Vector(const Vector& other): _size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef)
{
    _data = new ValueType[_capacity];
    std::memcpy(_data, other._data, _size * sizeof(ValueType));
}
Vector& Vector::operator=(const Vector& other)
{
    if (this == &other)
    {
        return *this;
    }
    delete[] _data;
    _size = other._size;
    _capacity = other._size;
    _multiplicativeCoef = other._multiplicativeCoef;
    _data = new ValueType[_capacity];
    std::memcpy(_data, other._data, _size * sizeof(ValueType));
    return *this;
}
Vector::Vector(Vector&& other) noexcept : _data(other._data), _size(other._size), _capacity(other._capacity), _multiplicativeCoef(other._multiplicativeCoef)
{
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}
Vector& Vector::operator=(Vector&& other) noexcept
{
    if (this == &other)
    {
        return *this;
    }
    delete[] _data;
    _data = other._data;
    _size = other._size;
    _capacity = other._capacity;
    _multiplicativeCoef = other._multiplicativeCoef;
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
    return *this;
}
Vector::~Vector()
{
    delete[] _data;
    _data = nullptr;
    _size = 0;
    _capacity = 0;
}
void Vector::extend()
{
    if (_capacity == 0)
    {
        _capacity = std::ceil(static_cast<size_t>(_multiplicativeCoef));
        _data = new ValueType[_capacity];
    }
    else
    {
        _capacity = static_cast<size_t>(std::ceil(_capacity * _multiplicativeCoef));
        ValueType* newdata = new ValueType[_capacity];
        std::memcpy(newdata, _data, _size * sizeof(ValueType));
        delete[] _data;
        _data = newdata;
        newdata = nullptr;
    }
}
void Vector::pushBack(const ValueType& value)
{
    if (_size == _capacity)
    {
        extend();
    }
    _data[_size] = value;
    _size++;
}
void Vector::pushFront(const ValueType& value)
{
    if (_size == _capacity)
    {
        extend();
    }
    for (size_t i = _size; i > 0; i--)
    {
        _data[i] = _data[i - 1];
    }
    _data[0] = value;
    _size++;
}
void Vector::insert(const ValueType& value, size_t pos)
{
    if (pos == 0)
    {
        pushFront(value);
        return;
    }
    if (pos == _size)
    {
        pushBack(value);
        return;
    }
    if (pos > _size)
    {
        throw std::out_of_range("The index of the inserted element doesn't belong to [0, size]");
    }
    for (size_t i = _size; i > pos; i--)
    {
        _data[i] = _data[i - 1];
    }
    _data[pos] = value;
    _size++;
}
void Vector::insert(const ValueType* values, size_t size, size_t pos)
{
    if (pos > _size)
    {
        throw std::out_of_range("The index of the inserted array doesn't belong to [0, size]");
    }
    while (_capacity <= (_size + size))
    {
        extend();
    }
    for (size_t i = _size; i > pos; i--)
    {
        _data[i + size - 1] = _data[i - 1];
    }
    for (size_t j = 0; j < size; j++)
    {
        _data[pos + j] = values[j];
    }
    _size += size;
}
void Vector::insert(const Vector& vector, size_t pos)
{
    if (pos > _size)
    {
        throw std::out_of_range("The index of the inserted vector doesn't belong to [0, size]");
    }
    while (_capacity <= (_size + vector._size))
    {
        extend();
    }
    for (size_t i = _size; i > pos; i--)
    {
        _data[i + vector._size - 1] = _data[i - 1];
    }
    for (size_t j = 0; j < vector._size; j++)
    {
        _data[pos + j] = vector._data[j];
    }
    _size += vector._size;
}
void Vector::popBack()
{
    if (_size == 0)
    {
        throw std::underflow_error("Can't pop from an empty vector");
    }
    _size--;
}
void Vector::popFront()
{
    if (_size == 0)
    {
        throw std::underflow_error("Can't pop from an empty vector");
    }
    std::memmove(_data, _data + 1, (_size - 1) * sizeof(ValueType));
    _size--;
}
void Vector::erase(size_t pos, size_t count)
{
    if (_size == 0)
    {
        throw std::underflow_error("Can't erase from an empty vector");
    }
    if (pos >= _size)
    {
        throw std::out_of_range("Invalid erasion position");
    }
    if ((pos + count) > _size)
    {
        count = _size - pos;
    }
    std::memmove(_data + pos, _data + pos + count, (_size - pos - count) * sizeof(ValueType));
    _size -= count;
}
void Vector::eraseBetween(size_t beginPos, size_t endPos)
{
    if (_size == 0)
    {
        throw std::underflow_error("Can't erase from an empty vector");
    }
    if (beginPos >= _size || endPos <= beginPos)
    {
        throw std::out_of_range("Invalid erasion position");
    }
    if (endPos >= _size)
    {
        endPos = _size;
    }
    ValueType* newdata = new ValueType[_size - (endPos - beginPos)];
    size_t counter = 0;
    for (size_t i = 0; i < _size; i++)
    {
        if (i < beginPos || i >= endPos)
        {
            newdata[counter] = _data[i];
            counter++;
        }
    }
    delete[] _data;
    _data  = newdata;
    newdata = nullptr;
    _size = _size - (endPos - beginPos);
}
size_t Vector::size() const
{
    return _size;
}
size_t Vector::capacity() const
{
    return _capacity;
}
double Vector::loadFactor() const
{
    return (_capacity == 0) ? 0.0 : static_cast<ValueType>(_size) / _capacity;
}
ValueType& Vector::operator[](size_t idx)
{
    if (idx >= _size)
    {
        throw std::out_of_range("Vector index out of range");
    }
    return _data[idx];
}
const ValueType& Vector::operator[](size_t idx) const
{
    if (idx >= _size) 
    {
        throw std::out_of_range("Vector index out of range");
    }
    return _data[idx];
}
long long Vector::find(const ValueType& value) const
{
    for (size_t i = 0; i < _size; i++)
    {
        if (_data[i] == value)
        {
            return i;
        }
    }
    return -1;
}
void Vector::reserve(size_t capacity)
{
    if (capacity < _size)
    {
        throw std::invalid_argument("New capacity can't be less than current size");
    }
    if (capacity > _capacity)
    {
        ValueType* newdata = new ValueType[capacity];
        std::memcpy(newdata, _data, _size * sizeof(ValueType));
        delete[] _data;
        _data = newdata;
        _capacity = capacity;
        newdata = nullptr;
    }
}
void Vector::shrinkToFit()
{
    if (_size == _capacity)
    {
        return;
    }
    ValueType* newdata = new ValueType[_size];
    std::memcpy(newdata, _data, _size * sizeof(ValueType));
    delete[] _data;
    _data = newdata;
    _capacity = _size;
    newdata = nullptr;
}
Vector::Iterator::Iterator(ValueType* ptr): _ptr(ptr) {}
ValueType& Vector::Iterator::operator*()
{
    return *_ptr;
}
const ValueType& Vector::Iterator::operator*() const
{
    return *_ptr;
}
ValueType* Vector::Iterator::operator->()
{
    return _ptr;
}
const ValueType* Vector::Iterator::operator->() const
{
    return _ptr;
}
Vector::Iterator Vector::Iterator::operator++()
{
    ++_ptr;
    return *this;
}
Vector::Iterator Vector::Iterator::operator++(int)
{
    Iterator copy = *this;
    ++_ptr;
    return copy;
}
bool Vector::Iterator::operator==(const Iterator& other) const
{
    return _ptr == other._ptr;
}
bool Vector::Iterator::operator!=(const Iterator& other) const
{
    return _ptr != other._ptr;
}
Vector::Iterator Vector::begin()
{
    return Iterator(_data);
}
Vector::Iterator Vector::end()
{
    return Iterator(_data + _size);
}
Vector::ReverseIterator::ReverseIterator(ValueType* ptr): _ptr(ptr) {}
ValueType& Vector::ReverseIterator::operator*()
{
    return *_ptr;
}
const ValueType& Vector::ReverseIterator::operator*() const
{
    return *_ptr;
}
ValueType* Vector::ReverseIterator::operator->()
{
    return _ptr;
}
const ValueType* Vector::ReverseIterator::operator->() const
{
    return _ptr;
}
Vector::ReverseIterator Vector::ReverseIterator::operator++()
{
    --_ptr;
    return *this;
}
Vector::ReverseIterator Vector::ReverseIterator::operator++(int)
{
    ReverseIterator copy = *this;
    --_ptr;
    return copy;
}
bool Vector::ReverseIterator::operator==(const ReverseIterator& other) const
{
    return _ptr == other._ptr;
}
bool Vector::ReverseIterator::operator!=(const ReverseIterator& other) const
{
    return _ptr != other._ptr;
}
Vector::ReverseIterator Vector::rbegin()
{
    return ReverseIterator(_data + _size - 1);
}
Vector::ReverseIterator Vector::rend()
{
    return ReverseIterator(_data - 1);
}

