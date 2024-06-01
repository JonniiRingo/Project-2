#include <iostream>
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
    T* ptr_;
    int* ref_;

public:
    smart_ptr() : ptr_(nullptr), ref_(new int(0)) {}

    explicit smart_ptr(T* &raw_ptr) : ptr_(raw_ptr), ref_(new int(1)) {}

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

    smart_ptr(const smart_ptr& rhs) : ptr_(rhs.ptr_), ref_(rhs.ref_) {
      ++(*ref_); 
    }

    smart_ptr(smart_ptr&& rhs) noexcept : ptr_(rhs.ptr_), ref_(rhs.ref_) {
      rhs.ptr_ = nullptr;
      rhs.ref_ = nullptr;
    }

smart_ptr& operator=(const smart_ptr& rhs){
  if (this != &rhs) {
    smart_ptr temp(rhs); // Create a temporary copy of rhs
    std::swap(ptr_, temp.ptr_);
    std::swap(ref_, temp.ref_);
  }
  return *this; 
}


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


    bool clone(){
      if (!ptr_ || *ref_ <= 1) {
        return false; 
      }
      --(*ref_);
      ref_ = new int(1);
      ptr_ = new T(*ptr_);
      return true;
    }

    int ref_count() const {
      return *ref_; 
    }

    T& operator*(){
      if(!ptr_){
        throw null_ptr_exception("Invalid access");
      }
      return *ptr_; 
    }

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
    smart_ptr<int> sp1;
    assert(sp1.ref_count() == 0);

    int* p2 = new int{ 42 };
    smart_ptr<int> sp2(p2);
    assert(sp2.ref_count() == 1);

    smart_ptr<int> sp3(new int{ 12 });
    assert(sp3.ref_count() == 1);

    {
        smart_ptr<int> sp4{ sp2 };
        assert(sp2.ref_count() == 2 && sp4.ref_count() == 2);
        smart_ptr<int> sp5;
        sp5 = (sp4 = sp2);
    }
    assert(sp2.ref_count() == 1);

    smart_ptr<int> sp6(std::move(sp3));
    assert(sp6.ref_count() == 1 && sp3.ref_count() == 0);



  return 0; 
}