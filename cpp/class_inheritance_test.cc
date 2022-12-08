#include <iostream>
#include <typeinfo>

class BaseNormal {
 public:
  void print(uint8_t* p) {
    uint8_t* start_address = p; // 记录 Base 类起始地址
    std::cout << "BaseNormal has no virtual functions.\n\n";
    std::cout << "BaseNormal size: " << sizeof(BaseNormal) << std::endl;
    std::cout << "BaseNormal start address: " << reinterpret_cast<void*>(start_address) << std::endl << std::endl;

    std::cout << "BaseNormal data menber: \n";
    std::cout << "  address: " << reinterpret_cast<void*>(p) << " offset: " << p - start_address << " bytes.   value1: " << *reinterpret_cast<char*>(p) << std::endl;
    p += 4;
    std::cout << "  address: " << reinterpret_cast<void*>(p) << " offset: " << p - start_address << " bytes.   value2: " << *reinterpret_cast<int*>(p) << std::endl;
    p += 4;
    std::cout << "  address: " << reinterpret_cast<void*>(p) << " offset: " << p - start_address << " bytes.   value3: " << *reinterpret_cast<int64_t*>(p) << std::endl;
  }

 private:
  char value1 = 'a';
  int value2 = 1024;
  int64_t value3 = 2048;
};

class Base {
 public:
  typedef void(*function)();
  void print(uint8_t* p) {
    uint8_t* start = p;
    std::cout << "Base has virtual functions.\n\n";
    std::cout << "Base size: " << sizeof(Base) << std::endl;
    std::cout << "Base start address: " << reinterpret_cast<void*>(start) << std::endl << std::endl;

    std::cout << "Base data menber: \n";
    std::cout << "  address: " << reinterpret_cast<void*>(p) << "   vptr: " << reinterpret_cast<void*>(*reinterpret_cast<int64_t*>(p)) << std::endl;
    p += 8;
    std::cout << "  address: " << reinterpret_cast<void*>(p) << "   value1: " << *reinterpret_cast<char*>(p) << std::endl;
    p += 4;
    std::cout << "  address: " << reinterpret_cast<void*>(p) << "   value2: " << *reinterpret_cast<int*>(p) << std::endl;
    p += 4;
    std::cout << "  address: " << reinterpret_cast<void*>(p) << "   value3: " << *reinterpret_cast<int64_t*>(p) << std::endl;

    std::cout << std::endl;

    // 虚函数表保存的为函数指针
    uint8_t* vtable_start = reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start));
    std::cout << "vtable start address: " << reinterpret_cast<void*>(vtable_start) << std::endl;

    // 获得函数指针: auto fptr_value =  *reinterpret_cast<int64_t*>(vtable_ptr);
    // 转换为对应函数: auto fp = reinterpret_cast<function>(fptr_value);
    // 调用 fp();
    uint8_t* vtable_ptr = vtable_start;

    auto fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    std::cout << "  function a() address: " << reinterpret_cast<void*>(vtable_ptr) << " offset: " << vtable_ptr - vtable_start << " bytes.  a(): ";
    fp();

    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    std::cout << "  function b() address: " << reinterpret_cast<void*>(vtable_ptr) << " offset: " << vtable_ptr - vtable_start << " bytes.  b(): ";
    fp();

    // typeinfo
    vtable_ptr = vtable_start - 8;
    auto info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_ptr));
    std::cout << " type: " << info->name() << std::endl;

    // 函数指针对于类的起始位置偏移量
    vtable_ptr = vtable_start - 16;
    std::cout << "  vptr offset: " << *reinterpret_cast<int64_t*>(vtable_ptr) << std::endl;

    std::cout << "\n";
  }

  virtual void a() {
    std::cout << "  Base::a()\n";
  }

  virtual void b() {
    std::cout << "  Base::b()\n";
  }

 private:
  char value1 = 'a';
  int value2 = 1024;
  int64_t value3 = 2048;
};

class SingleDeriver : public BaseNormal {

};

int main() {
  std::cout << "-----------------------------------------------------------------------------\n";
  BaseNormal base_normal;
  base_normal.print(reinterpret_cast<uint8_t*>(&base_normal));
  std::cout << "-----------------------------------------------------------------------------\n\n";

  std::cout << "-----------------------------------------------------------------------------\n";
  Base base;
  base.print(reinterpret_cast<uint8_t*>(&base));
  std::cout << "-----------------------------------------------------------------------------\n\n";

  std::cout << typeid(SingleDeriver).name() << std::endl;

  return 0;
}