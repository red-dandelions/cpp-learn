#include <cstdio>
#include <typeinfo>

class BaseNormal {
 public:
  void print(uint8_t* p) {
    uint8_t* start_address = p;  // 记录 Base 类起始地址
    printf("BaseNormal has no virtual functions.\n\n");
    printf("BaseNormal size: %ld\n", sizeof(BaseNormal));
    printf("BaseNormal start address: %p\n\n", reinterpret_cast<void*>(start_address));

    // print data member
    printf("BaseNormal data menber: \n");
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
  }

 private:
  char value1 = 'a';
  int value2 = 1024;
  int64_t value3 = 2048;
};

class Base {
 public:
  virtual void a() { printf("  Base::a()\n"); }

  virtual void b() { printf("  Base::b()\n"); }

  typedef void (*function)();
  void print(uint8_t* p) {
    uint8_t* start_address = p;  // 记录 Base 类起始地址
    printf("Base has virtual functions.\n\n");
    printf("Base size: %ld\n", sizeof(Base));
    printf("Base start address: %p\n\n", reinterpret_cast<void*>(start_address));

    printf("Base data menber: \n");
    printf("  address: %p offset: %2d bytes.   vptr: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));

    // 虚函数表保存的为函数指针
    uint8_t* vtable_start = reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address));

    auto info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));

    // 获得函数指针: auto fptr_value =  *reinterpret_cast<int64_t*>(vtable_ptr);
    // 转换为对应函数: auto fp = reinterpret_cast<function>(fptr_value);
    // 调用 fp();
    uint8_t* vtable_ptr = vtable_start;

    auto fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. a(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. b(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
  }

 private:
  char value1 = 'a';
  int value2 = 1024;
  int64_t value3 = 2048;
};

class SingleDeriver : public Base {
 public:
  void print(uint8_t* p) {
    uint8_t* start_address = p;
    printf("SingleDeriver has no virtual functions.\n\n");
    printf("SingleDeriver size: %ld\n", sizeof(SingleDeriver));
    printf("SingleDeriver start address: %p\n\n", reinterpret_cast<void*>(start_address));

    printf("SingleDeriver data menber: \n");
    printf("  address: %p offset: %2d bytes.   vptr: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value4: %d\n\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));

    // 虚函数表保存的为函数指针
    uint8_t* vtable_start = reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address));

    auto info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));

    // 获得函数指针: auto fptr_value =  *reinterpret_cast<int64_t*>(vtable_ptr);
    // 转换为对应函数: auto fp = reinterpret_cast<function>(fptr_value);
    // 调用 fp();

    uint8_t* vtable_ptr = vtable_start;

    auto fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. a(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function b() address: %p  offset: %2d bytes. b(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function c() address: %p  offset: %2d bytes. c(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
  }

  virtual void a() override { printf("  SingleDeriver::a()\n"); }

  virtual void c() { printf("  SingleDeriver::c()\n"); }

 private:
  int value4 = 512;
};

class Foo {
 public:
  virtual void c() { printf("  Foo::c()\n"); }

  virtual void d() { printf("  Foo::d()\n"); }

 private:
  char value4 = 'b';
  int64_t value5 = 128;
};

class MultiDeriver : public Base, public Foo {
 public:
  virtual void a() override { printf("  MultiDeriver::a()\n"); }

  virtual void d() override { printf("  MultiDeriver::d()\n"); }

  virtual void e() { printf("  MultiDeriver::e()\n"); }

  typedef void (*function)();
  void print(uint8_t* p) {
    uint8_t* start_address = p;  // 记录 Base 类起始地址
    size_t vptr2_offset;
    printf("MultiDeriver has virtual functions.\n\n");
    printf("MultiDeriver size: %ld\n", sizeof(MultiDeriver));
    printf("MultiDeriver start address: %p\n\n", reinterpret_cast<void*>(start_address));

    printf("MultiDeriver data menber: \n");
    printf("  address: %p offset: %2d bytes.  vptr1: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
    p += 8;
    vptr2_offset = p - start_address;
    printf("  address: %p offset: %2d bytes.  vptr2: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value4: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value5: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value6: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 2;
    printf("  address: %p offset: %2d bytes. value7: %d\n\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<short*>(p));

    // 虚函数表保存的为函数指针
    uint8_t* vtable_start = reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address));

    auto info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("MultiDeriver vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));

    // 获得函数指针: auto fptr_value =  *reinterpret_cast<int64_t*>(vtable_ptr);
    // 转换为对应函数: auto fp = reinterpret_cast<function>(fptr_value);
    // 调用 fp();
    uint8_t* vtable_ptr = vtable_start;
    // Base & MultiDeriver vtable
    auto fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. a(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function b() address: %p  offset: %2d bytes. b(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function d() address: %p  offset: %2d bytes. d(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function e() address: %p  offset: %2d bytes. e(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    // Foo & MultiDeriver vtable
    vtable_start =
        reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address + vptr2_offset));

    info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));
    vtable_ptr = vtable_start;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function c() address: %p  offset: %2d bytes. c(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function d() address: %p  offset: %2d bytes. d(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
  }

 private:
  char value6 = 'c';
  short value7 = 64;
};

class Bar : Base {
 public:
  virtual void d() { printf("  Bar::d()\n"); }

 private:
  int64_t value5 = 128;
};

class DiamondDeriver : public SingleDeriver, public Bar {
 public:
  virtual void e() { printf("  DiamondDeriver::e()\n"); }

  typedef void (*function)();
  void print(uint8_t* p) {
    uint8_t* start_address = p;  // 记录 Base 类起始地址
    size_t vptr2_offset;
    printf("DiamondDeriver has virtual functions.\n\n");
    printf("DiamondDeriver size: %ld\n", sizeof(DiamondDeriver));
    printf("DiamondDeriver start address: %p\n\n", reinterpret_cast<void*>(start_address));

    printf("DiamondDeriver data menber: \n");
    printf("  address: %p offset: %2d bytes.  vptr1: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value4: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 8;
    vptr2_offset = p - start_address;
    printf("  address: %p offset: %2d bytes.  vptr2: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value5: %lld\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value6: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 2;
    printf("  address: %p offset: %2d bytes. value7: %d\n\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<short*>(p));

    // 虚函数表保存的为函数指针
    uint8_t* vtable_start = reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address));

    auto info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("DiamondDeriver vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));

    // 获得函数指针: auto fptr_value =  *reinterpret_cast<int64_t*>(vtable_ptr);
    // 转换为对应函数: auto fp = reinterpret_cast<function>(fptr_value);
    // 调用 fp();
    uint8_t* vtable_ptr = vtable_start;
    // SingleDeriver & DiamondDeriver vtable
    auto fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. a(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function b() address: %p  offset: %2d bytes. b(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function c() address: %p  offset: %2d bytes. c(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function e() address: %p  offset: %2d bytes. e(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    // Bar & DiamondDeriver vtable
    vtable_start =
        reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address + vptr2_offset));

    info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));
    vtable_ptr = vtable_start;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. a(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function b() address: %p  offset: %2d bytes. b(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function d() address: %p  offset: %2d bytes. d(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
  }

 private:
  char value6 = 'd';
  short value7 = 32;
};

class SingleDeriverVirtual : public virtual Base {
 public:
  virtual void c() { printf("  SingleDeriverVirtual::c()\n"); }

 private:
  int value4 = 512;
};

class BarVirtual : public virtual Base {
 public:
  virtual void a() override { printf("  BarVirtual::a()\n"); }

  virtual void d() { printf("  BarVirtual::d()\n"); }

 private:
  int value5 = 258;
};

class DiamondDeriverVirtual : public SingleDeriverVirtual, public BarVirtual {
 public:
  virtual void e() { printf("  DiamondDeriverVirtual::e()\n"); }

  typedef void (*function)();
  void print(uint8_t* p) {
    uint8_t* start_address = p;  // 记录 Base 类起始地址
    size_t vptr2_offset, vptr3_offset;
    printf("DiamondDeriverVirtual has virtual functions.\n\n");
    printf("DiamondDeriverVirtual size: %ld\n", sizeof(DiamondDeriverVirtual));
    printf("DiamondDeriverVirtual start address: %p\n\n", reinterpret_cast<void*>(start_address));

    printf("DiamondDeriverVirtual data menber: \n");
    printf("  address: %p offset: %2d bytes.  vptr1: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value4: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 8;
    vptr2_offset = p - start_address;
    printf("  address: %p offset: %2d bytes.  vptr2: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    printf("  address: %p offset: %2d bytes. value5: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value6: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 2;
    printf("  address: %p offset: %2d bytes. value7: %d\n\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<short*>(p));
    p += 2;
    vptr3_offset = p - start_address;
    printf("  address: %p offset: %2d bytes.  vptr3: %p\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), reinterpret_cast<void*>(p));
    p += 8;
    // Base
    printf("  address: %p offset: %2d bytes. value1: %c\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<char*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value2: %d\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int*>(p));
    p += 4;
    printf("  address: %p offset: %2d bytes. value3: %lld\n\n", reinterpret_cast<void*>(p),
           static_cast<int>(p - start_address), *reinterpret_cast<int64_t*>(p));

    // 虚函数表保存的为函数指针
    uint8_t* vtable_start = reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address));

    auto info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("DiamondDeriverVirtual vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));

    // 获得函数指针: auto fptr_value =  *reinterpret_cast<int64_t*>(vtable_ptr);
    // 转换为对应函数: auto fp = reinterpret_cast<function>(fptr_value);
    // 调用 fp();
    uint8_t* vtable_ptr = vtable_start;
    // SingleDeriverVirtual & DiamondDeriverVirtual vtable
    auto fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function c() address: %p  offset: %2d bytes. c(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function e() address: %p  offset: %2d bytes. e(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    // BarVirtual vtable
    vtable_start =
        reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address + vptr2_offset));

    info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("\nBarVirtual vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));
    vtable_ptr = vtable_start;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function a() address: %p  offset: %2d bytes. a(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
    vtable_ptr += 8;
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function d() address: %p  offset: %2d bytes. d(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();

    // Base vtable
    vtable_start =
        reinterpret_cast<uint8_t*>(*reinterpret_cast<int64_t*>(start_address + vptr3_offset));
    info = reinterpret_cast<std::type_info*>(*reinterpret_cast<int64_t*>(vtable_start - 8));
    printf("\nBase vtable start address: %p  | type: %s  | offset: %2lld bytes.\n",
           reinterpret_cast<void*>(vtable_start), info->name(),
           *reinterpret_cast<int64_t*>(vtable_start - 16));
    vtable_ptr = vtable_start;
    // 这里是因为在 Base 的虚函数表中，a 被 BarVirtual 类重写了，所以在 Base 类的虚函数表中没有
    // a()，访问就是错误的. b() 的偏移量为8字节，偏移量对应 Base 类的虚函数表。 如果 b() 在 a()
    // 前面，则不用加偏移量，b() 就在虚函数表的表头。
    vtable_ptr += 8;
    printf("%p", reinterpret_cast<void*>(vtable_ptr));
    fp = reinterpret_cast<function>(*reinterpret_cast<int64_t*>(vtable_ptr));
    printf("  function b() address: %p  offset: %2d bytes. b(): ",
           reinterpret_cast<void*>(vtable_ptr), static_cast<int>(vtable_ptr - vtable_start));
    fp();
  }

 private:
  char value6 = 'd';
  short value7 = 32;
};

int main() {
  printf("-----------------------------------------------------------------------------\n");
  BaseNormal base_normal;
  base_normal.print(reinterpret_cast<uint8_t*>(&base_normal));
  printf("-----------------------------------------------------------------------------\n\n");

  printf("-----------------------------------------------------------------------------\n");
  Base base;
  base.print(reinterpret_cast<uint8_t*>(&base));
  printf("-----------------------------------------------------------------------------\n\n");

  printf("-----------------------------------------------------------------------------\n");
  SingleDeriver single_deriver;
  single_deriver.print(reinterpret_cast<uint8_t*>(&single_deriver));
  printf("-----------------------------------------------------------------------------\n\n");

  printf("-----------------------------------------------------------------------------\n");
  MultiDeriver multi_deriver;
  multi_deriver.print(reinterpret_cast<uint8_t*>(&multi_deriver));
  printf("-----------------------------------------------------------------------------\n\n");

  printf("-----------------------------------------------------------------------------\n");
  DiamondDeriver diamond_deriver;
  diamond_deriver.print(reinterpret_cast<uint8_t*>(&diamond_deriver));
  printf("-----------------------------------------------------------------------------\n\n");

  printf("-----------------------------------------------------------------------------\n");
  DiamondDeriverVirtual diamond_deriver_virtual;
  diamond_deriver_virtual.print(reinterpret_cast<uint8_t*>(&diamond_deriver_virtual));
  printf("-----------------------------------------------------------------------------\n\n");

  return 0;
}