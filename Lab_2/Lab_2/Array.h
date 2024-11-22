#pragma once

#include <iostream>
#include <cassert>
#include <cstdlib> 
#include <utility> 

using namespace std;

template<typename T>
class Array {
private:
	T* data_; 
	int size_; 
	int capacity_; 

	void resize(int new_capacity) { 
		T* new_data = static_cast<T*>(malloc(new_capacity * sizeof(T))); 
		for (int i = 0; i < size_; ++i) {
			new (new_data + i) T(move(data_[i])); 
		}
		free(data_); 
		data_ = new_data; 
		capacity_ = new_capacity; 
	}

public:
	Array() : data_(nullptr), size_(0), capacity_(16) 
	{
		data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
	}

	explicit Array(int capacity) : data_(nullptr), size_(0), capacity_(capacity) 
	{
		data_ = static_cast<T*>(malloc(capacity_ * sizeof(T))); 
	}

	Array(const Array& other) : data_(nullptr), size_(other.size_), capacity_(other.capacity_)
	{
		data_ = static_cast<T*>(malloc(capacity_ * sizeof(T)));
		for (int i = 0; i < size_; ++i)
		{
			new (data_ + i) T(other.data_[i]);
		}
	}

	~Array()
	{
		for (int i = 0; i < size_; ++i) {
			data_[i].~T();
		}
		free(data_);
	}


	Array& operator=(const Array& other)
	{
		if (this != &other)
		{
			Array temp(other);
			swap(data_, temp.data_);
			swap(size_, temp.size_);
			swap(capacity_, temp.capacity_);
		}
		return *this;
	}

	Array(Array&& other) noexcept : data_(nullptr), size_(0), capacity_(0)
	{
		swap(data_, other.data_);
		swap(size_, other.size_);
		swap(capacity_, other.capacity_);
	}

	Array& operator=(Array&& other) noexcept {
		if (this != &other) {
			for (int i = 0; i < size_; ++i) {
				data_[i].~T();
			}
			free(data_);

			data_ = other.data_;
			size_ = other.size_;
			capacity_ = other.capacity_;

			other.data_ = nullptr;
			other.size_ = 0;
			other.capacity_ = 0;
		}
		return *this;
	}

	int insert(const T& value) {
		if (size_ == capacity_) {
			resize(capacity_ * 2);
		}
		new (data_ + size_) T(value);
		return size_++;
	}

	int insert(int index, const T& value) {
		assert(index >= 0 && index <= size_);
		if (size_ == capacity_) {
			resize(capacity_ * 2);
		}
		for (int i = size_; i > index; --i) {
			new (data_ + i) T(move(data_[i - 1]));
			data_[i - 1].~T();
		}
		new (data_ + index) T(value);
		++size_;
		return index;
	}

	void remove(int index) {
		assert(index >= 0 && index < size_);
		data_[index].~T();
		for (int i = index; i < size_ - 1; ++i) {
			new (data_ + i) T(move(data_[i + 1]));
			data_[i + 1].~T();
		}
		--size_;
	}

	const T& operator[](int index) const {
		assert(index >= 0 && index < size_);
		return data_[index];
	}

	T& operator[](int index) {
		assert(index >= 0 && index < size_);
		return data_[index];
	}

	int size() const {
		return size_;
	}

	class Iterator {
	private:
		T* current_;
		T* end_;
		int direction_;

	public:
		Iterator(T* start, T* end) : current_(start), end_(end), direction_(1) {}

		Iterator(T* start, T* end, int direction_) : direction_(direction_)
		{
			current_ = direction_ == 1 ? start : end - 1;
			end_ = direction_ == 1 ? end : start - 1;
		}

		const T& get() const {
			return *current_;
		}

		void set(const T& value) {
			*current_ = value;
		}

		void next() {
			current_ += direction_;
		}


		bool hasNext() const {
			return direction_ == 1 ? current_ < end_ : current_ > end_;
		}

		const T& operator*() const {
			return *current_;
		}

		T& operator*() {
			return *current_;
		}

		Iterator& operator++() {
			current_ += direction_;
			return *this;
		}

		Iterator operator++(int) {
			Iterator temp = *this;
			current_ += direction_;
			return temp;
		}

		bool operator!=(const Iterator& other) const {
			return current_ != other.current_;
		}
	};

	Iterator iterator() {
		return Iterator(data_, data_ + size_);
	}

	Iterator reverseIterator() {
		return Iterator(data_, data_ + size_, -1);
	}

	Iterator begin() {
		return Iterator(data_, data_ + size_);
	}

	Iterator end() {
		return Iterator(data_ + size_, data_ + size_);
	}


	class ConstIterator {
	private:
		const T* current_;
		const T* end_;
		int direction_;

	public:
		ConstIterator(const T* start, const T* end) : current_(start), end_(end), direction_(1) {}

		ConstIterator(const T* start, const T* end, int direction) : direction_(direction) {
			current_ = direction_ == 1 ? start : end - 1;
			end_ = direction_ == 1 ? end : start - 1;
		}

		const T& get() const {
			return *current_;
		}

		void next() {
			current_ += direction_;
		}

		bool hasNext() const {
			return direction_ == 1 ? current_ < end_ : current_ > end_;
		}

		const T& operator*() const {
			return *current_;
		}


		ConstIterator& operator++() {
			current_ += direction_;
			return *this;
		}

		ConstIterator operator++(int) {
			Iterator temp = *this;
			current_ += direction_;
			return temp;
		}

		bool operator!=(const ConstIterator& other) const {
			return current_ != other.current_;
		}
	};

	ConstIterator iterator() const {
		return ConstIterator(data_, data_ + size_);
	}

	ConstIterator reverseIterator() const {
		return ConstIterator(data_, data_ + size_, -1);
	}

	const ConstIterator begin() const {
		return ConstIterator(data_, data_ + size_);
	}

	const ConstIterator end() const {
		return ConstIterator(data_ + size_, data_ + size_);
	}

	const ConstIterator cbegin() const {
		return ConstIterator(data_, data_ + size_);
	}

	const ConstIterator cend() const {
		return ConstIterator(data_ + size_, data_ + size_);
	}
};