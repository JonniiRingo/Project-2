
/*In this homework you will make your own smart pointer type. You will write the following class to develop a 

referenced counted smart pointer, you will also implement a few other member functions to resemble the 

functionality of an ordinary raw pointer. Basically, this is a problem of designing a single, non-trivial class 

and overloading a few pointer related operators. You may not use any of the STL, except for stdexcept ("So what 

you're saying is 'exceptions is the only exception'?" - Binh) to do this, i.e., please don't try and use a 

shared_ptr to implement the class. You can start with this code, and you may add other member functions, 

if you want.

*/




#include <iostream>
#include <memory>
#include <exception>
#include <cassert>
using namespace std;

class null_ptr_exception : public runtime_error{
public:
  null_ptr_exception(const char* s) : runtime_error(s){
  }
}; 



template <typename T>
class smart_ptr {

private:
    T* ptr_;               // pointer to the referred object
    int* ref_;             // pointer to a reference count


public:

      // Create a smart_ptr that is initialized to nullptr. The reference count
      // should be initialized to nullptr.
    smart_ptr(){
      ptr_ = nullptr; 
      ref_ = new int(0);  
    }

 
    // Create a smart_ptr that is initialized to raw_ptr. The reference count
    // should be one. No change is made to raw_ptr.
    explicit smart_ptr(T* &raw_ptr) : ptr_(raw_ptr), ref_(new int(1)) {}


      // Create a smart_ptr that is initialized to raw_ptr. The reference count
      // should be one. If the constructor fails raw_ptr is deleted.	
    explicit smart_ptr(T* &&raw_ptr) {
      try {
        ptr_ = raw_ptr; 
        ref_ = new int(1);
      }
      catch(exception e){
        cout << e.what(); 
        delete raw_ptr; 
      }
    }


      // Copy construct a pointer from rhs. The reference count should be 
      // incremented by one.   
    smart_ptr(const smart_ptr& rhs): ptr_(rhs.ptr_), ref_(rhs.ref_){
      ++(*ref_); 
    }

      // Move construct a pointer from rhs.
    smart_ptr(smart_ptr&& rhs) noexcept : ptr_(rhs.ptr_), ref_(rhs.ref_) {
      rhs.ptr_ = nullptr;
      rhs.ref_ = nullptr;
    }


      // *This assignment should make a shallow copy of the right-hand side's
      // pointer data. The reference count should be incremented as appropriate.
    smart_ptr& operator=(const smart_ptr& rhs){
      if (this != &rhs) {
        smart_ptr temp(rhs); // Create a temporary copy of rhs
        std::swap(ptr_, temp.ptr_);
        std::swap(ref_, temp.ref_);
      }
      return *this; 
    }


      // * This move assignment should steal the right-hand side's pointer data.
    smart_ptr& operator=(smart_ptr&& rhs){
      if (this != &rhs) {
        --(*ref_);
        if (*ref_ == 0) {
          delete ptr_;
          delete ref_;
        }
        ptr_ = rhs.ptr_;
        ref_ = rhs.ref_;
        rhs.ptr_ = nullptr;
        rhs.ref_ = nullptr;
      }
      return *this; 
    }



      // If the smart_ptr is either nullptr or has a reference count of one, this
      // function will do nothing and return false. Otherwise, the referred to
      // object's reference count will be decreased and a new deep copy of the
      // object will be created. This new copy will be the object that this
      // smart_ptr points and its reference count will be one.       
    bool clone(){
      if (!ptr_ || *ref_ <= 1) {
        return false; 
      }
      --(*ref_);
      ref_ = new int(1);
      ptr_ = new T(*ptr_);
      return true;
    }



      // Returns the reference count of the pointed to data.      
    int ref_count() const{
      if (ref_){
        return *ref_; 
      } 
      return 0; 
    }

      // The dereference operator shall return a reference to the referred object.
      // Throws null_ptr_exception on invalid access. 
    T& operator*(){
      if(!ptr_){
        throw null_ptr_exception("Sorry bro invalid access");
      }
      return *ptr_; 
    }

    // The arrow operator shall return the pointer ptr_. Throws null_ptr_exception
    // on invalid access.
    T* operator->(){
      if(!ptr_){
        throw null_ptr_exception("SOrry bro invalid access");
      }
      return ptr_;
    }

    ~smart_ptr(){
      --(*ref_);
      if (*ref_ == 0) {
        delete ptr_;
        delete ref_;
      }
    }

};





int main(){

  smart_ptr<int> sahkdb;
  assert(sahkdb.ref_count() == 0);




 smart_ptr<int> sp1; // default constructor
 assert(sp1.ref_count() == 0);

 int* p2 = new int{ 42 };
 smart_ptr<int> sp2(p2); // l-value raw_ptr constructor
 assert(sp2.ref_count() == 1);

 smart_ptr<int> sp3(new int{ 12 }); // r-value raw_ptr constructor
 assert(sp3.ref_count() == 1);

 {
 smart_ptr<int> sp4{ sp2 }; // l-value copy constructor
 assert(sp2.ref_count() == 2 && sp4.ref_count() == 2);
 smart_ptr<int> sp5;
 sp5 = (sp4 = sp2);
 }
 assert(sp2.ref_count() == 1);

 smart_ptr<int> sp6(std::move(sp3)); // r-value copy constructor
 assert(sp6.ref_count() == 1 && sp3.ref_count() == 0);


  return 0; 
}