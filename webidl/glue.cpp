
#include <emscripten.h>
#include <stdlib.h>

EM_JS_DEPS(webidl_binder, "$intArrayFromString,$UTF8ToString,$alignMemory");

class Child2JS : public Child2 {
public:
  void virtualFunc()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['Child2JS'])[$0];
      if (!self.hasOwnProperty('virtualFunc')) throw 'a JSImplementation must implement all functions, you forgot Child2JS::virtualFunc.';
      self['virtualFunc']();
    }, (ptrdiff_t)this);
  }
  void virtualFunc2()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['Child2JS'])[$0];
      if (!self.hasOwnProperty('virtualFunc2')) throw 'a JSImplementation must implement all functions, you forgot Child2JS::virtualFunc2.';
      self['virtualFunc2']();
    }, (ptrdiff_t)this);
  }
  void virtualFunc3(int x)  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['Child2JS'])[$0];
      if (!self.hasOwnProperty('virtualFunc3')) throw 'a JSImplementation must implement all functions, you forgot Child2JS::virtualFunc3.';
      self['virtualFunc3']($1);
    }, (ptrdiff_t)this, x);
  }
  void virtualFunc4(int x)  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['Child2JS'])[$0];
      if (!self.hasOwnProperty('virtualFunc4')) throw 'a JSImplementation must implement all functions, you forgot Child2JS::virtualFunc4.';
      self['virtualFunc4']($1);
    }, (ptrdiff_t)this, x);
  }
  void __destroy__()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['Child2JS'])[$0];
      if (!self.hasOwnProperty('__destroy__')) throw 'a JSImplementation must implement all functions, you forgot Child2JS::__destroy__.';
      self['__destroy__']();
    }, (ptrdiff_t)this);
  }
};

class ConcreteJS : public VirtualBase {
public:
  void func()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['ConcreteJS'])[$0];
      if (!self.hasOwnProperty('func')) throw 'a JSImplementation must implement all functions, you forgot ConcreteJS::func.';
      self['func']();
    }, (ptrdiff_t)this);
  }
  void constFunc() const  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['ConcreteJS'])[$0];
      if (!self.hasOwnProperty('constFunc')) throw 'a JSImplementation must implement all functions, you forgot ConcreteJS::constFunc.';
      self['constFunc']();
    }, (ptrdiff_t)this);
  }
  void __destroy__()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['ConcreteJS'])[$0];
      if (!self.hasOwnProperty('__destroy__')) throw 'a JSImplementation must implement all functions, you forgot ConcreteJS::__destroy__.';
      self['__destroy__']();
    }, (ptrdiff_t)this);
  }
};

class InnerUser : public Space::InnerUserBase {
public:
  void Callback(Space::Inner* inner)  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['InnerUser'])[$0];
      if (!self.hasOwnProperty('Callback')) throw 'a JSImplementation must implement all functions, you forgot InnerUser::Callback.';
      self['Callback']($1);
    }, (ptrdiff_t)this, (ptrdiff_t)inner);
  }
  void __destroy__()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['InnerUser'])[$0];
      if (!self.hasOwnProperty('__destroy__')) throw 'a JSImplementation must implement all functions, you forgot InnerUser::__destroy__.';
      self['__destroy__']();
    }, (ptrdiff_t)this);
  }
};

class JSSmallObject : public ISmallObject {
public:
  int getID(int number)  {
    return  (int) EM_ASM_INT({
      var self = Module['getCache'](Module['JSSmallObject'])[$0];
      if (!self.hasOwnProperty('getID')) throw 'a JSImplementation must implement all functions, you forgot JSSmallObject::getID.';
      return self['getID']($1);
    }, (ptrdiff_t)this, number);
  }
  void __destroy__()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['JSSmallObject'])[$0];
      if (!self.hasOwnProperty('__destroy__')) throw 'a JSImplementation must implement all functions, you forgot JSSmallObject::__destroy__.';
      self['__destroy__']();
    }, (ptrdiff_t)this);
  }
};

class JSObjectProvider : public IObjectProvider {
public:
  JSSmallObject* getObject()  {
    return  (JSSmallObject*) EM_ASM_PTR({
      var self = Module['getCache'](Module['JSObjectProvider'])[$0];
      if (!self.hasOwnProperty('getObject')) throw 'a JSImplementation must implement all functions, you forgot JSObjectProvider::getObject.';
      return self['getObject']();
    }, (ptrdiff_t)this);
  }
  void __destroy__()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['JSObjectProvider'])[$0];
      if (!self.hasOwnProperty('__destroy__')) throw 'a JSImplementation must implement all functions, you forgot JSObjectProvider::__destroy__.';
      self['__destroy__']();
    }, (ptrdiff_t)this);
  }
};

class JSArrayArgumentTest : public ArrayArgumentTest {
public:
  bool byteArrayTest(const char* arg)  {
    return  (bool) EM_ASM_INT({
      var self = Module['getCache'](Module['JSArrayArgumentTest'])[$0];
      if (!self.hasOwnProperty('byteArrayTest')) throw 'a JSImplementation must implement all functions, you forgot JSArrayArgumentTest::byteArrayTest.';
      return self['byteArrayTest']($1);
    }, (ptrdiff_t)this, arg);
  }
  bool domStringTest(const char* arg)  {
    return  (bool) EM_ASM_INT({
      var self = Module['getCache'](Module['JSArrayArgumentTest'])[$0];
      if (!self.hasOwnProperty('domStringTest')) throw 'a JSImplementation must implement all functions, you forgot JSArrayArgumentTest::domStringTest.';
      return self['domStringTest']($1);
    }, (ptrdiff_t)this, arg);
  }
  void __destroy__()  {
     (void) EM_ASM_INT({
      var self = Module['getCache'](Module['JSArrayArgumentTest'])[$0];
      if (!self.hasOwnProperty('__destroy__')) throw 'a JSImplementation must implement all functions, you forgot JSArrayArgumentTest::__destroy__.';
      self['__destroy__']();
    }, (ptrdiff_t)this);
  }
};

extern "C" {

// Define custom allocator functions that we can force export using
// EMSCRIPTEN_KEEPALIVE.  This avoids all webidl users having to add
// malloc/free to -sEXPORTED_FUNCTIONS.
EMSCRIPTEN_KEEPALIVE void webidl_free(void* p) { free(p); }
EMSCRIPTEN_KEEPALIVE void* webidl_malloc(size_t len) { return malloc(len); }


// Interface: Parent


Parent* EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_Parent_1(int val) {
  return new Parent(val);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_getVal_0(Parent* self) {
  return self->getVal();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_mulVal_1(Parent* self, int mul) {
  self->mulVal(mul);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_parentFunc_0(Parent* self) {
  self->parentFunc();
}

const Parent* EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_getAsConst_0(Parent* self) {
  return self->getAsConst();
}

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_voidStar_1(Parent* self, void* something) {
  return self->voidStar(something);
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_getBoolean_0(Parent* self) {
  return self->getBoolean();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_get_attr_0(Parent* self) {
  return self->attr;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_set_attr_1(Parent* self, int arg0) {
  self->attr = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent_get_immutableAttr_0(Parent* self) {
  return self->immutableAttr;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Parent___destroy___0(Parent* self) {
  delete self;
}

// Interface: Child2


Child2* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_Child2_0() {
  return new Child2();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_getValCube_0(Child2* self) {
  return self->getValCube();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_printStatic_1(int arg0) {
  Child2::printStatic(arg0);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_virtualFunc_0(Child2* self) {
  self->virtualFunc();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_virtualFunc2_0(Child2* self) {
  self->virtualFunc2();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_virtualFunc3_1(Child2* self, int x) {
  self->virtualFunc3(x);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_virtualFunc4_1(Child2* self, int x) {
  self->virtualFunc4(x);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_runVirtualFunc_1(Child2* myself) {
  Child2::runVirtualFunc(myself);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_runVirtualFunc3_2(Child2* myself, int x) {
  Child2::runVirtualFunc3(myself, x);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_getVal_0(Child2* self) {
  return self->getVal();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_mulVal_1(Child2* self, int mul) {
  self->mulVal(mul);
}

const Parent* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_getAsConst_0(Child2* self) {
  return self->getAsConst();
}

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_voidStar_1(Child2* self, void* something) {
  return self->voidStar(something);
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_getBoolean_0(Child2* self) {
  return self->getBoolean();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_get_attr_0(Child2* self) {
  return self->attr;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_set_attr_1(Child2* self, int arg0) {
  self->attr = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2_get_immutableAttr_0(Child2* self) {
  return self->immutableAttr;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2___destroy___0(Child2* self) {
  delete self;
}

// Interface: VirtualBase


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VirtualBase_func_0(VirtualBase* self) {
  self->func();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VirtualBase_constFunc_0(VirtualBase* self) {
  self->constFunc();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VirtualBase___destroy___0(VirtualBase* self) {
  delete self;
}

// Interface: InnerUserBase


void EMSCRIPTEN_KEEPALIVE emscripten_bind_InnerUserBase___destroy___0(Space::InnerUserBase* self) {
  delete self;
}

// Interface: ISmallObject


int EMSCRIPTEN_KEEPALIVE emscripten_bind_ISmallObject_getID_1(ISmallObject* self, int number) {
  return self->getID(number);
}

// Interface: IObjectProvider


ISmallObject* EMSCRIPTEN_KEEPALIVE emscripten_bind_IObjectProvider_getObject_0(IObjectProvider* self) {
  return self->getObject();
}

// Interface: ArrayArgumentTest


ArrayArgumentTest* EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayArgumentTest_ArrayArgumentTest_0() {
  return new ArrayArgumentTest();
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayArgumentTest_byteArrayTest_1(ArrayArgumentTest* self, const char* arg) {
  return self->byteArrayTest(arg);
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayArgumentTest_domStringTest_1(ArrayArgumentTest* self, const char* arg) {
  return self->domStringTest(arg);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayArgumentTest___destroy___0(ArrayArgumentTest* self) {
  delete self;
}

// Interface: VoidPtr


void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// Interface: Child1


Child1* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_Child1_0() {
  return new Child1();
}

Child1* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_Child1_1(int val) {
  return new Child1(val);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getValSqr_0(Child1* self) {
  return self->getValSqr();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getValSqr_1(Child1* self, int more) {
  return self->getValSqr(more);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getValTimes_0(Child1* self) {
  return self->getValTimes();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getValTimes_1(Child1* self, int times) {
  return self->getValTimes(times);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_parentFunc_1(Child1* self, int x) {
  self->parentFunc(x);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getVal_0(Child1* self) {
  return self->getVal();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_mulVal_1(Child1* self, int mul) {
  self->mulVal(mul);
}

const Parent* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getAsConst_0(Child1* self) {
  return self->getAsConst();
}

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_voidStar_1(Child1* self, void* something) {
  return self->voidStar(something);
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_getBoolean_0(Child1* self) {
  return self->getBoolean();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_get_attr_0(Child1* self) {
  return self->attr;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_set_attr_1(Child1* self, int arg0) {
  self->attr = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1_get_immutableAttr_0(Child1* self) {
  return self->immutableAttr;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child1___destroy___0(Child1* self) {
  delete self;
}

// Interface: Child2JS


Child2JS* EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2JS_Child2JS_0() {
  return new Child2JS();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2JS_virtualFunc_0(Child2JS* self) {
  self->virtualFunc();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2JS_virtualFunc2_0(Child2JS* self) {
  self->virtualFunc2();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2JS_virtualFunc3_1(Child2JS* self, int x) {
  self->virtualFunc3(x);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2JS_virtualFunc4_1(Child2JS* self, int x) {
  self->virtualFunc4(x);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Child2JS___destroy___0(Child2JS* self) {
  delete self;
}

// Interface: ConcreteJS


ConcreteJS* EMSCRIPTEN_KEEPALIVE emscripten_bind_ConcreteJS_ConcreteJS_0() {
  return new ConcreteJS();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ConcreteJS_func_0(ConcreteJS* self) {
  self->func();
}

const void EMSCRIPTEN_KEEPALIVE emscripten_bind_ConcreteJS_constFunc_0(ConcreteJS* self) {
  self->constFunc();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ConcreteJS___destroy___0(ConcreteJS* self) {
  delete self;
}

// Interface: StringUser


StringUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_StringUser_StringUser_0() {
  return new StringUser();
}

StringUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_StringUser_StringUser_2(char* str, int i) {
  return new StringUser(str, i);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StringUser_Print_2(StringUser* self, int anotherInteger, char* anotherString) {
  self->Print(anotherInteger, anotherString);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StringUser_PrintFloat_1(StringUser* self, float f) {
  self->PrintFloat(f);
}

const char* EMSCRIPTEN_KEEPALIVE emscripten_bind_StringUser_returnAString_0(StringUser* self) {
  return self->returnAString();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StringUser___destroy___0(StringUser* self) {
  delete self;
}

// Interface: RefUser


RefUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser_RefUser_0() {
  return new RefUser();
}

RefUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser_RefUser_1(int value) {
  return new RefUser(value);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser_getValue_1(RefUser* self, RefUser* b) {
  return self->getValue(*b);
}

RefUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser_getMe_0(RefUser* self) {
  return &self->getMe();
}

RefUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser_getCopy_0(RefUser* self) {
  static thread_local RefUser temp;
  return (temp = self->getCopy(), &temp);
}

StringUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser_getAnother_0(RefUser* self) {
  static thread_local StringUser temp;
  return (temp = self->getAnother(), &temp);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_RefUser___destroy___0(RefUser* self) {
  delete self;
}

// Interface: VoidPointerUser


VoidPointerUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPointerUser_VoidPointerUser_0() {
  return new VoidPointerUser();
}

void* EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPointerUser_GetVoidPointer_0(VoidPointerUser* self) {
  return self->GetVoidPointer();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPointerUser_SetVoidPointer_1(VoidPointerUser* self, void* ptr) {
  self->SetVoidPointer(ptr);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPointerUser___destroy___0(VoidPointerUser* self) {
  delete self;
}

// Interface: Inner


Space::Inner* EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_Inner_0() {
  return new Space::Inner();
}

Space::Inner* EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_Inner_1(int value) {
  return new Space::Inner(value);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_get_0(Space::Inner* self) {
  return self->get();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_get_value_0(Space::Inner* self) {
  return self->get_value();
}

Space::Inner* EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_mul_1(Space::Inner* self, float x) {
  return &(*self *= x);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_getAsArray_1(Space::Inner* self, int x) {
  return ((*self)[x]);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_incInPlace_1(Space::Inner* self, const Space::Inner* i) {
  (*self += *i);
}

Space::Inner* EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_add_1(Space::Inner* self, const Space::Inner* i) {
  static thread_local Space::Inner temp;
  return (temp = (*self + *i), &temp);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner_mul2_1(Space::Inner* self, int x) {
  return (*self * x);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_Inner___destroy___0(Space::Inner* self) {
  delete self;
}

// Interface: InnerUser


InnerUser* EMSCRIPTEN_KEEPALIVE emscripten_bind_InnerUser_InnerUser_0() {
  return new InnerUser();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_InnerUser_Callback_1(InnerUser* self, Space::Inner* inner) {
  self->Callback(inner);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_InnerUser___destroy___0(InnerUser* self) {
  delete self;
}

// Interface: EnumClass


EnumClass* EMSCRIPTEN_KEEPALIVE emscripten_bind_EnumClass_EnumClass_0() {
  return new EnumClass();
}

EnumClass_EnumWithinClass EMSCRIPTEN_KEEPALIVE emscripten_bind_EnumClass_GetEnum_0(EnumClass* self) {
  return self->GetEnum();
}

EnumNamespace_EnumInNamespace EMSCRIPTEN_KEEPALIVE emscripten_bind_EnumClass_GetEnumFromNameSpace_0(EnumClass* self) {
  return self->GetEnumFromNameSpace();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_EnumClass___destroy___0(EnumClass* self) {
  delete self;
}

// Interface: TypeTestClass


TypeTestClass* EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_TypeTestClass_0() {
  return new TypeTestClass();
}

char EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_ReturnCharMethod_0(TypeTestClass* self) {
  return self->ReturnCharMethod();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_AcceptCharMethod_1(TypeTestClass* self, char x) {
  self->AcceptCharMethod(x);
}

unsigned char EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_ReturnUnsignedCharMethod_0(TypeTestClass* self) {
  return self->ReturnUnsignedCharMethod();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_AcceptUnsignedCharMethod_1(TypeTestClass* self, unsigned char x) {
  self->AcceptUnsignedCharMethod(x);
}

unsigned short EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_ReturnUnsignedShortMethod_0(TypeTestClass* self) {
  return self->ReturnUnsignedShortMethod();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_AcceptUnsignedShortMethod_1(TypeTestClass* self, unsigned short x) {
  self->AcceptUnsignedShortMethod(x);
}

unsigned int EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_ReturnUnsignedLongMethod_0(TypeTestClass* self) {
  return self->ReturnUnsignedLongMethod();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass_AcceptUnsignedLongMethod_1(TypeTestClass* self, unsigned int x) {
  self->AcceptUnsignedLongMethod(x);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_TypeTestClass___destroy___0(TypeTestClass* self) {
  delete self;
}

// Interface: StructInArray


StructInArray* EMSCRIPTEN_KEEPALIVE emscripten_bind_StructInArray_StructInArray_2(int attr1, int attr2) {
  return new StructInArray(attr1, attr2);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_StructInArray_get_attr1_0(StructInArray* self) {
  return self->attr1;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StructInArray_set_attr1_1(StructInArray* self, int arg0) {
  self->attr1 = arg0;
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_StructInArray_get_attr2_0(StructInArray* self) {
  return self->attr2;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StructInArray_set_attr2_1(StructInArray* self, int arg0) {
  self->attr2 = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StructInArray___destroy___0(StructInArray* self) {
  delete self;
}

// Interface: ArrayClass


ArrayClass* EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_ArrayClass_0() {
  return new ArrayClass();
}

EM_JS(void, array_bounds_check_error, (size_t idx, size_t size), {
  throw 'Array index ' + idx + ' out of bounds: [0,' + size + ')';
});

static void array_bounds_check(size_t array_size, size_t array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    array_bounds_check_error(array_idx, array_size);
  }
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_get_int_array_1(ArrayClass* self, int arg0) {
  return (array_bounds_check(sizeof(self->int_array) / sizeof(self->int_array[0]), arg0), self->int_array[arg0]);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_set_int_array_2(ArrayClass* self, int arg0, int arg1) {
  (array_bounds_check(sizeof(self->int_array) / sizeof(self->int_array[0]), arg0), self->int_array[arg0] = arg1);
}

StructInArray* EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_get_struct_array_1(ArrayClass* self, int arg0) {
  return &self->struct_array[arg0];
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_set_struct_array_2(ArrayClass* self, int arg0, StructInArray* arg1) {
  self->struct_array[arg0] = *arg1;
}

StructInArray* EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_get_struct_ptr_array_1(ArrayClass* self, int arg0) {
  return self->struct_ptr_array[arg0];
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass_set_struct_ptr_array_2(ArrayClass* self, int arg0, StructInArray* arg1) {
  self->struct_ptr_array[arg0] = arg1;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ArrayClass___destroy___0(ArrayClass* self) {
  delete self;
}

// Interface: ReceiveArrays


ReceiveArrays* EMSCRIPTEN_KEEPALIVE emscripten_bind_ReceiveArrays_ReceiveArrays_0() {
  return new ReceiveArrays();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ReceiveArrays_giveMeArrays_3(ReceiveArrays* self, float* vertices, int* triangles, int num) {
  self->giveMeArrays(vertices, triangles, num);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ReceiveArrays___destroy___0(ReceiveArrays* self) {
  delete self;
}

// Interface: StoreArray


StoreArray* EMSCRIPTEN_KEEPALIVE emscripten_bind_StoreArray_StoreArray_0() {
  return new StoreArray();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StoreArray_setArray_1(StoreArray* self, const int* array) {
  self->setArray(array);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_StoreArray_getArrayValue_1(StoreArray* self, int index) {
  return self->getArrayValue(index);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_StoreArray___destroy___0(StoreArray* self) {
  delete self;
}

// Interface: LongLongTypes


unsigned long long EMSCRIPTEN_KEEPALIVE emscripten_bind_LongLongTypes_get_lluArray_1(LongLongTypes* self, int arg0) {
  return self->lluArray[arg0];
}

long long EMSCRIPTEN_KEEPALIVE emscripten_bind_LongLongTypes_get_ll_0(LongLongTypes* self) {
  return self->ll;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_LongLongTypes_set_ll_1(LongLongTypes* self, long long arg0) {
  self->ll = arg0;
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_LongLongTypes___destroy___0(LongLongTypes* self) {
  delete self;
}

// Interface: JSSmallObject


JSSmallObject* EMSCRIPTEN_KEEPALIVE emscripten_bind_JSSmallObject_JSSmallObject_0() {
  return new JSSmallObject();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_JSSmallObject_getID_1(JSSmallObject* self, int number) {
  return self->getID(number);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_JSSmallObject___destroy___0(JSSmallObject* self) {
  delete self;
}

// Interface: JSObjectProvider


JSObjectProvider* EMSCRIPTEN_KEEPALIVE emscripten_bind_JSObjectProvider_JSObjectProvider_0() {
  return new JSObjectProvider();
}

JSSmallObject* EMSCRIPTEN_KEEPALIVE emscripten_bind_JSObjectProvider_getObject_0(JSObjectProvider* self) {
  return self->getObject();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_JSObjectProvider___destroy___0(JSObjectProvider* self) {
  delete self;
}

// Interface: ObjectFactory


ObjectFactory* EMSCRIPTEN_KEEPALIVE emscripten_bind_ObjectFactory_ObjectFactory_0() {
  return new ObjectFactory();
}

IObjectProvider* EMSCRIPTEN_KEEPALIVE emscripten_bind_ObjectFactory_getProvider_0(ObjectFactory* self) {
  return self->getProvider();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_ObjectFactory___destroy___0(ObjectFactory* self) {
  delete self;
}

// Interface: JSArrayArgumentTest


JSArrayArgumentTest* EMSCRIPTEN_KEEPALIVE emscripten_bind_JSArrayArgumentTest_JSArrayArgumentTest_0() {
  return new JSArrayArgumentTest();
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_JSArrayArgumentTest_byteArrayTest_1(JSArrayArgumentTest* self, const char* arg) {
  return self->byteArrayTest(arg);
}

bool EMSCRIPTEN_KEEPALIVE emscripten_bind_JSArrayArgumentTest_domStringTest_1(JSArrayArgumentTest* self, const char* arg) {
  return self->domStringTest(arg);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_JSArrayArgumentTest___destroy___0(JSArrayArgumentTest* self) {
  delete self;
}

// Interface: BindToTest


BindToTest* EMSCRIPTEN_KEEPALIVE emscripten_bind_BindToTest_BindToTest_0() {
  return new BindToTest();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_BindToTest_testString_1(BindToTest* self, const char* arg) {
  return self->test(arg);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_BindToTest_testInt_1(BindToTest* self, int arg) {
  return self->test(arg);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_BindToTest___destroy___0(BindToTest* self) {
  delete self;
}

// $AnEnum
AnEnum EMSCRIPTEN_KEEPALIVE emscripten_enum_AnEnum_enum_value1() {
  return enum_value1;
}
AnEnum EMSCRIPTEN_KEEPALIVE emscripten_enum_AnEnum_enum_value2() {
  return enum_value2;
}

// $EnumClass_EnumWithinClass
EnumClass_EnumWithinClass EMSCRIPTEN_KEEPALIVE emscripten_enum_EnumClass_EnumWithinClass_e_val() {
  return EnumClass::e_val;
}

// $EnumNamespace_EnumInNamespace
EnumNamespace_EnumInNamespace EMSCRIPTEN_KEEPALIVE emscripten_enum_EnumNamespace_EnumInNamespace_e_namespace_val() {
  return EnumNamespace::e_namespace_val;
}

}

