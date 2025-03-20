#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include <queue>

template<typename T>
class priority_queue
{
public:
	priority_queue(std::function<size_t(const T&, const T&)> compare);

	T					peek();
	bool				empty();
	void				push(const T& item);
	T					pop();
	bool				remove(const T& item);
	size_t				size() const;

private:
	std::vector<T>								_data;
	std::function<size_t(const T&, const T&)>	_compare;

	void _up(size_t index);
	void _down(size_t index);
};

template<typename T>
inline priority_queue<T>::priority_queue(std::function<size_t(const T&, const T&)> compare)
{
	_compare = compare;
}

template<typename T>
inline T priority_queue<T>::peek()
{
	if (_data.empty()) {
		throw std::runtime_error("PriorityQueue is empty!");
	}

	return _data[0];
}

template<typename T>
inline bool priority_queue<T>::empty()
{
	return _data.empty();
}

template<typename T>
inline void priority_queue<T>::push(const T& item)
{
	_data.push_back(item); 
	_up(_data.size() - 1);
}

template<typename T>
inline T priority_queue<T>::pop()
{
	if (_data.empty()) {
		throw std::runtime_error("PriorityQueue is empty!");
	}

	T top = _data[0];
	_data[0] = _data.back();
	_data.pop_back();

	if (!_data.empty()) {
		_down(0);
	}

	return top;
}

template<typename T>
inline bool priority_queue<T>::remove(const T& item)
{
	for (size_t i = 0; i < _data.size(); ++i) {
		if (_compare(_data[i], item) == 0) {
			std::swap(_data[i], _data.back());

			_data.pop_back();

			size_t parent_index = (i - 1) >> 1;

			if (parent_index > 0 && _compare(_data[i], _data[parent_index]) < 0) {
				_up(i);
			}
			else
			{
				_down(i);
			}
			return true;
		}
	}
	return false;
}

template<typename T>
inline size_t priority_queue<T>::size() const
{
	return _data.size();
}

template<typename T>
inline void priority_queue<T>::_up(size_t index)
{
	while (index > 0)
	{
		size_t parent_index = (index - 1) >> 1;
		if (_compare(_data[index], _data[parent_index]) > 0) break;
		std::swap(_data[index], _data[parent_index]);
		index = parent_index;
	}
}

template<typename T>
inline void priority_queue<T>::_down(size_t index)
{
	size_t last = _data.size() - 1;
	while (true) {
		size_t left = (index << 1) + 1;
		size_t right = (index << 1) + 2;
		size_t best = index;

		if (left <= last && _compare(_data[left], _data[best]) < 0) {
			best = left;
		}

		if (right <= last && _compare(_data[right], _data[best]) < 0) {
			best = right;
		}

		if (best == index) break;
		std::swap(_data[index], _data[best]);
		index = best;
	}
}
