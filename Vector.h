#ifndef VECTOR_H
#define VECTOR_H
#include<iostream>
#include<stdexcept>
#include<memory>
#include<initializer_list>
#include<algorithm>
template <typename T>
class Vector{
public:

	class IteratorY;
	class ConstIterator;
	class Iterator;
	using value_type = T;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using iterator = Vector<T>::Iterator;
	using const_iterator = Vector<T>::ConstIterator;
  using iteratorY = Vector<T>::IteratorY;

private:
	size_type max_sz;
	size_type sz;
	pointer values;
	
  void allocate(size_type n){ 
		if(n<sz)
			throw std::runtime_error("Verkleinerung unter die Anzahl der gespeicherten Elemente nicht moeglich"); //utest verlangt nur return
		if(n==0) {
			values = nullptr;
			delete[] values;
		}
		else {
			pointer buf = new  value_type[n];
			for(size_type i{0}; i<sz; ++i)
				buf[i] = values[i];
			delete[] values;
			values = buf;
		}
		max_sz=n;
		
	}
public:
	Vector(size_type n=0) : max_sz{n}, sz{0}, values{n==0 ? nullptr : new value_type[n]}
	{
	}
	
	Vector(const Vector& v) : max_sz{v.max_sz}, sz{0}, values{max_sz==0 ? nullptr : new value_type[v.max_sz]} {
		if(v.sz != 0 && values != nullptr)
			for(size_type i{0}; i < v.sz; ++i){
				values[sz++] = v.values[i]; //klammern() weg
			}
	
	}
	
	Vector(std::initializer_list<value_type> l) : Vector(l.size()){
		for (const auto& v : l)
			values[sz++] = v;
	}
	
	~Vector(){
		delete[] values;
	}
	
	bool empty() const {
		return sz==0;
	}
	
	void clear(){
		sz=0;
	}
	
	size_type size()const{
		return sz;
	}
	
	void reserve(size_type n) {
		allocate(n);
	}
	
	void shrink_to_fit(){
		allocate(sz);
	}
	
	size_type capacity() const {
	return max_sz;
	}
	
	reference operator[](size_type index){
		if(size() <= index)
			throw std::runtime_error("[index] out of range");
		
		 return values[index];
	}
	
	const_reference operator[](size_type index) const{
		if(size() <= index)
			throw std::runtime_error("[index] out of range");
		
		 return values[index];
	}
	
	void pop_back(){
	
		if (empty())
			throw std::runtime_error("Vector already emtpy");
			
		pointer buf = new  value_type[sz-1];
			for(size_type i{0}; i<sz-1; ++i)
				(buf[i]) = (values[i]);
			delete[] values;
			values = buf;
			sz--;
	}
	
	void push_back(const_reference value) {
		if(sz >= max_sz) {
			allocate(max_sz*2+1);
		}
	
		values[sz++] = value;
	}
  
 iteratorY begin_y(){
    return iteratorY((values+sz-1), values-1);
  }
  
  iteratorY end_y(){
    return iteratorY(values-1, values-1);
  }
	

	iterator begin() {
		return iterator(values);
	}
	
	iterator end() {
	
		return iterator((values+sz));
	}
	

	
	const_iterator begin() const {
		return const_iterator(values);
	}
	
	const_iterator end() const {
		return const_iterator((values+sz));
	}
	
	std::ostream& print(std::ostream& o) const{
	
		if(values == nullptr)
			throw std::runtime_error("no values!");
			
		else{
			o << "[";
			for (size_type i{0}; i < this->size(); i++){
				if(i==(size()-1))
					o<< values[i] << "]";
				else
					o << values[i] << ", ";
				
				}
		}
		
		return o;
	}


		Vector& operator= (Vector rightop) {
			std::swap(max_sz, rightop.max_sz);
			std::swap(sz, rightop.sz);
			std::swap(values, rightop.values);
			return *this;
		}

	
	bool operator==(const Vector& v){
		return this->values == v.values;
	}

	
	Vector(Vector&& v) : max_sz{v.max_sz}, sz{0}, values{max_sz==0 ? nullptr : new value_type[v.max_sz]} {
		if(v.sz != 0 && values != nullptr)
			for(size_type i{0}; i < v.sz; ++i){
				values[sz++] = v.values[i]; //klammern() weg
			}
		
		v.max_sz=0;
		v.sz=0;
		v.values = nullptr;
	}
	
	
  iterator insert(const_iterator pos, const_reference val) {
		  difference_type diff = pos - begin();
		  if (diff < 0 || static_cast<size_type>(diff) > sz)
			 throw std::runtime_error("Iterator out of bounds");
		  size_type current{static_cast<size_type>(diff)};
		  if (sz >= max_sz)
			 reserve(max_sz * 2 + 10); // Achtung Sonderfall, wenn keine Mindestgroesze definiert ist
		  for (auto i{sz}; i-- > current;)
			 values[i + 1] = values[i];
		  values[current] = val;
		  ++sz;
		  return iterator(values + current);
	}

	iterator erase(const_iterator pos) {
	  difference_type diff = pos - begin();
	  if (diff < 0 || static_cast<size_type>(diff) >= sz)
		 throw std::runtime_error("Iterator out of bounds");
	  size_type current{static_cast<size_type>(diff)};
	  for (auto i{current}; i < sz - 1; ++i)
		 values[i] = values[i + 1];
	  --sz;
	  return iterator(values + current);
}
  
  
  
      
  
	friend difference_type operator-(const ConstIterator& lop, const ConstIterator& rop);
	
	//ITERATOREN
	class Iterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::reference;
      using pointer = Vector::pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
    	pointer ptr;
      
    public:
      // Methoden
    	Iterator( pointer p = nullptr) : ptr{p}{	}
    	
    	
    	Iterator& operator= (Iterator rightop){
			std::swap(ptr, rightop.ptr);
			
			return *this;
		}
    	
    	
    	
    	
    	

    reference operator*(){
     return *ptr;
      }
      
      
      
      pointer operator->(){
      return ptr;
      	
      }
      
   
      iterator& operator++(){
        if (ptr == nullptr) {
		throw std::runtime_error("Iterator points to nullptr");
	}
      ptr++;
        return *this;
      }
      
    
      
      iterator operator++(int){
        if (ptr == nullptr) {
		throw std::runtime_error("Iterator points to nullptr");
	}
        iterator nptr = Iterator(ptr);
        ptr++;
        return nptr;
      }
      
 
      pointer get_ptr()const {
        return ptr;
      }
      
    bool operator==(const const_iterator& obj) const{
            return ConstIterator(ptr) == obj;
        }
    bool operator!=(const const_iterator& obj) const{
            return ConstIterator(ptr) != obj;
        }
    
    bool operator==(const iterator& obj) const{
            ptr == obj.get_ptr();
        }
    bool operator!=(const iterator& obj) const{
            return ptr != obj.get_ptr();
        }
    
    

   
  };
  class ConstIterator {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::const_reference;
      using pointer = Vector::const_pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
    	const_pointer ptr;
      
    public:
      // Methoden
      ConstIterator( const_pointer p = nullptr) : ptr{p}{}
      

      reference operator*() const{
      return *ptr;
      }
      
  
      
      pointer operator->() const{
        return ptr;
      }
      
  
      
      const_iterator& operator++() {
        if (ptr == nullptr) {
		throw std::runtime_error("Iterator points to nullptr");
	}
        ptr++;

        return *this;
      }
      

   
      //postfix
      const_iterator operator++(int) {
        if (ptr == nullptr) {
		throw std::runtime_error("Iterator points to nullptr");
	}
    const_iterator old = *this;
            ptr++;
            return old;
      }
      
      const_pointer get_ptr() const {
      	return ptr;
      }
      
      
  bool operator==(const const_iterator& obj) const{
            return ptr == obj.ptr;
        }
  bool operator!=(const const_iterator& obj) const{
            return !(obj == *this);
        }
 friend difference_type operator-(const ConstIterator& lop, const ConstIterator& rop) {
            return lop.ptr - rop.ptr ;
        } 
  
  };
  
  	//ITERATOR_Y
	class IteratorY {
    public:
      using value_type = Vector::value_type;
      using reference = Vector::reference;
      using pointer = Vector::pointer;
      using difference_type = Vector::difference_type;
      using iterator_category = std::forward_iterator_tag;
    private:
    pointer ptr1;
    pointer ptr2;
    
    public:

    IteratorY( pointer p1 = nullptr, pointer p2 = nullptr) : ptr1{p1}, ptr2{p2}{	}
    	
    	
    	
    	IteratorY& operator= (Iterator rightop){
			std::swap(ptr1, rightop.ptr);
			
			return *this;
		}
    
    IteratorY& operator= (IteratorY rightop){
			std::swap(ptr1, rightop.ptr);
			ptr2=nullptr;
			return *this;
		}
    	


    reference operator*(){
     return *ptr1;
      }
      
      
      
      pointer operator->(){
      return ptr1;
      	
      }
    
		
   
      iteratorY& operator++(){
        
        
        
       if (ptr1 == ptr2){
         return *this;
       }
        
        ptr1--;
        return *this;
      }
      
      
    
      
      iteratorY operator++(int){
			 if (ptr1 == ptr2){
         return *this;
       }
         iteratorY nptr = ptr1;
        ptr1++;
       
        
        return nptr;
      }
      
 
      pointer get_ptr() const {
        return ptr1;
      }
      
    bool operator==(const iteratorY& obj) const{
            return ptr1 == obj.get_ptr();
        }
    bool operator!=(const iteratorY& obj) const{
            return ptr1 != obj.get_ptr();
        }

   
  };
  
  
};
template <typename T>
	std::ostream& operator<<(std::ostream& o, const Vector<T>& v) {

		return v.print(o);
	};



#endif
