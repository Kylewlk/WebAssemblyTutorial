
// Bindings utilities

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant)
    @param {*=} __class__ */
function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_webidl_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_webidl_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_webidl_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = alignMemory(len, 8); // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_webidl_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },
  copy(array, view, offset) {
    offset /= view.BYTES_PER_ELEMENT;
    for (var i = 0; i < array.length; i++) {
      view[offset + i] = array[i];
    }
  },
};

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}

/** @suppress {duplicate} (TODO: avoid emitting this multiple times, it is redundant) */
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}

// Interface: Parent

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Parent(val) {
  if (val && typeof val === 'object') val = val.ptr;
  this.ptr = _emscripten_bind_Parent_Parent_1(val);
  getCache(Parent)[this.ptr] = this;
};

Parent.prototype = Object.create(WrapperObject.prototype);
Parent.prototype.constructor = Parent;
Parent.prototype.__class__ = Parent;
Parent.__cache__ = {};
Module['Parent'] = Parent;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['getVal'] = Parent.prototype.getVal = function() {
  var self = this.ptr;
  return _emscripten_bind_Parent_getVal_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['mulVal'] = Parent.prototype.mulVal = function(mul) {
  var self = this.ptr;
  if (mul && typeof mul === 'object') mul = mul.ptr;
  _emscripten_bind_Parent_mulVal_1(self, mul);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['parentFunc'] = Parent.prototype.parentFunc = function() {
  var self = this.ptr;
  _emscripten_bind_Parent_parentFunc_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['getAsConst'] = Parent.prototype.getAsConst = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_Parent_getAsConst_0(self), Parent);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['voidStar'] = Parent.prototype.voidStar = function(something) {
  var self = this.ptr;
  if (something && typeof something === 'object') something = something.ptr;
  return wrapPointer(_emscripten_bind_Parent_voidStar_1(self, something), VoidPtr);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['getBoolean'] = Parent.prototype.getBoolean = function() {
  var self = this.ptr;
  return !!(_emscripten_bind_Parent_getBoolean_0(self));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['get_attr'] = Parent.prototype.get_attr = function() {
  var self = this.ptr;
  return _emscripten_bind_Parent_get_attr_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['set_attr'] = Parent.prototype.set_attr = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_Parent_set_attr_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(Parent.prototype, 'attr', { get: Parent.prototype.get_attr, set: Parent.prototype.set_attr });
/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['get_immutableAttr'] = Parent.prototype.get_immutableAttr = function() {
  var self = this.ptr;
  return _emscripten_bind_Parent_get_immutableAttr_0(self);
};

/** @suppress {checkTypes} */
Object.defineProperty(Parent.prototype, 'immutableAttr', { get: Parent.prototype.get_immutableAttr });

/** @suppress {undefinedVars, duplicate} @this{Object} */
Parent.prototype['__destroy__'] = Parent.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Parent___destroy___0(self);
};

// Interface: Child2

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Child2() {
  this.ptr = _emscripten_bind_Child2_Child2_0();
  getCache(Child2)[this.ptr] = this;
};

Child2.prototype = Object.create(Parent.prototype);
Child2.prototype.constructor = Child2;
Child2.prototype.__class__ = Child2;
Child2.__cache__ = {};
Module['Child2'] = Child2;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['getValCube'] = Child2.prototype.getValCube = function() {
  var self = this.ptr;
  return _emscripten_bind_Child2_getValCube_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['printStatic'] = Child2.prototype.printStatic = function(arg0) {
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_Child2_printStatic_1(arg0);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['virtualFunc'] = Child2.prototype.virtualFunc = function() {
  var self = this.ptr;
  _emscripten_bind_Child2_virtualFunc_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['virtualFunc2'] = Child2.prototype.virtualFunc2 = function() {
  var self = this.ptr;
  _emscripten_bind_Child2_virtualFunc2_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['virtualFunc3'] = Child2.prototype.virtualFunc3 = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_Child2_virtualFunc3_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['virtualFunc4'] = Child2.prototype.virtualFunc4 = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_Child2_virtualFunc4_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['runVirtualFunc'] = Child2.prototype.runVirtualFunc = function(myself) {
  if (myself && typeof myself === 'object') myself = myself.ptr;
  _emscripten_bind_Child2_runVirtualFunc_1(myself);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['runVirtualFunc3'] = Child2.prototype.runVirtualFunc3 = function(myself, x) {
  if (myself && typeof myself === 'object') myself = myself.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_Child2_runVirtualFunc3_2(myself, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['getVal'] = Child2.prototype.getVal = function() {
  var self = this.ptr;
  return _emscripten_bind_Child2_getVal_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['mulVal'] = Child2.prototype.mulVal = function(mul) {
  var self = this.ptr;
  if (mul && typeof mul === 'object') mul = mul.ptr;
  _emscripten_bind_Child2_mulVal_1(self, mul);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['getAsConst'] = Child2.prototype.getAsConst = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_Child2_getAsConst_0(self), Parent);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['voidStar'] = Child2.prototype.voidStar = function(something) {
  var self = this.ptr;
  if (something && typeof something === 'object') something = something.ptr;
  return wrapPointer(_emscripten_bind_Child2_voidStar_1(self, something), VoidPtr);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['getBoolean'] = Child2.prototype.getBoolean = function() {
  var self = this.ptr;
  return !!(_emscripten_bind_Child2_getBoolean_0(self));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['get_attr'] = Child2.prototype.get_attr = function() {
  var self = this.ptr;
  return _emscripten_bind_Child2_get_attr_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['set_attr'] = Child2.prototype.set_attr = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_Child2_set_attr_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(Child2.prototype, 'attr', { get: Child2.prototype.get_attr, set: Child2.prototype.set_attr });
/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['get_immutableAttr'] = Child2.prototype.get_immutableAttr = function() {
  var self = this.ptr;
  return _emscripten_bind_Child2_get_immutableAttr_0(self);
};

/** @suppress {checkTypes} */
Object.defineProperty(Child2.prototype, 'immutableAttr', { get: Child2.prototype.get_immutableAttr });

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2.prototype['__destroy__'] = Child2.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Child2___destroy___0(self);
};

// Interface: VirtualBase

/** @suppress {undefinedVars, duplicate} @this{Object} */
function VirtualBase() { throw "cannot construct a VirtualBase, no constructor in IDL" }
VirtualBase.prototype = Object.create(WrapperObject.prototype);
VirtualBase.prototype.constructor = VirtualBase;
VirtualBase.prototype.__class__ = VirtualBase;
VirtualBase.__cache__ = {};
Module['VirtualBase'] = VirtualBase;
/** @suppress {undefinedVars, duplicate} @this{Object} */
VirtualBase.prototype['func'] = VirtualBase.prototype.func = function() {
  var self = this.ptr;
  _emscripten_bind_VirtualBase_func_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
VirtualBase.prototype['constFunc'] = VirtualBase.prototype.constFunc = function() {
  var self = this.ptr;
  _emscripten_bind_VirtualBase_constFunc_0(self);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
VirtualBase.prototype['__destroy__'] = VirtualBase.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VirtualBase___destroy___0(self);
};

// Interface: InnerUserBase

/** @suppress {undefinedVars, duplicate} @this{Object} */
function InnerUserBase() { throw "cannot construct a InnerUserBase, no constructor in IDL" }
InnerUserBase.prototype = Object.create(WrapperObject.prototype);
InnerUserBase.prototype.constructor = InnerUserBase;
InnerUserBase.prototype.__class__ = InnerUserBase;
InnerUserBase.__cache__ = {};
Module['InnerUserBase'] = InnerUserBase;

/** @suppress {undefinedVars, duplicate} @this{Object} */
InnerUserBase.prototype['__destroy__'] = InnerUserBase.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_InnerUserBase___destroy___0(self);
};

// Interface: ISmallObject

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ISmallObject() { throw "cannot construct a ISmallObject, no constructor in IDL" }
ISmallObject.prototype = Object.create(WrapperObject.prototype);
ISmallObject.prototype.constructor = ISmallObject;
ISmallObject.prototype.__class__ = ISmallObject;
ISmallObject.__cache__ = {};
Module['ISmallObject'] = ISmallObject;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ISmallObject.prototype['getID'] = ISmallObject.prototype.getID = function(number) {
  var self = this.ptr;
  if (number && typeof number === 'object') number = number.ptr;
  return _emscripten_bind_ISmallObject_getID_1(self, number);
};


// Interface: IObjectProvider

/** @suppress {undefinedVars, duplicate} @this{Object} */
function IObjectProvider() { throw "cannot construct a IObjectProvider, no constructor in IDL" }
IObjectProvider.prototype = Object.create(WrapperObject.prototype);
IObjectProvider.prototype.constructor = IObjectProvider;
IObjectProvider.prototype.__class__ = IObjectProvider;
IObjectProvider.__cache__ = {};
Module['IObjectProvider'] = IObjectProvider;
/** @suppress {undefinedVars, duplicate} @this{Object} */
IObjectProvider.prototype['getObject'] = IObjectProvider.prototype.getObject = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_IObjectProvider_getObject_0(self), ISmallObject);
};


// Interface: ArrayArgumentTest

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ArrayArgumentTest() {
  this.ptr = _emscripten_bind_ArrayArgumentTest_ArrayArgumentTest_0();
  getCache(ArrayArgumentTest)[this.ptr] = this;
};

ArrayArgumentTest.prototype = Object.create(WrapperObject.prototype);
ArrayArgumentTest.prototype.constructor = ArrayArgumentTest;
ArrayArgumentTest.prototype.__class__ = ArrayArgumentTest;
ArrayArgumentTest.__cache__ = {};
Module['ArrayArgumentTest'] = ArrayArgumentTest;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayArgumentTest.prototype['byteArrayTest'] = ArrayArgumentTest.prototype.byteArrayTest = function(arg) {
  var self = this.ptr;
  ensureCache.prepare();
  if (typeof arg == 'object') { arg = ensureInt8(arg); }
  return !!(_emscripten_bind_ArrayArgumentTest_byteArrayTest_1(self, arg));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayArgumentTest.prototype['domStringTest'] = ArrayArgumentTest.prototype.domStringTest = function(arg) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg && typeof arg === 'object') arg = arg.ptr;
  else arg = ensureString(arg);
  return !!(_emscripten_bind_ArrayArgumentTest_domStringTest_1(self, arg));
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayArgumentTest.prototype['__destroy__'] = ArrayArgumentTest.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_ArrayArgumentTest___destroy___0(self);
};

// Interface: VoidPtr

/** @suppress {undefinedVars, duplicate} @this{Object} */
function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};

// Interface: Child1

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Child1(val) {
  if (val && typeof val === 'object') val = val.ptr;
  if (val === undefined) { this.ptr = _emscripten_bind_Child1_Child1_0(); getCache(Child1)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_Child1_Child1_1(val);
  getCache(Child1)[this.ptr] = this;
};

Child1.prototype = Object.create(Parent.prototype);
Child1.prototype.constructor = Child1;
Child1.prototype.__class__ = Child1;
Child1.__cache__ = {};
Module['Child1'] = Child1;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['getValSqr'] = Child1.prototype.getValSqr = function(more) {
  var self = this.ptr;
  if (more && typeof more === 'object') more = more.ptr;
  if (more === undefined) { return _emscripten_bind_Child1_getValSqr_0(self) }
  return _emscripten_bind_Child1_getValSqr_1(self, more);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['getValTimes'] = Child1.prototype.getValTimes = function(times) {
  var self = this.ptr;
  if (times && typeof times === 'object') times = times.ptr;
  if (times === undefined) { return _emscripten_bind_Child1_getValTimes_0(self) }
  return _emscripten_bind_Child1_getValTimes_1(self, times);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['parentFunc'] = Child1.prototype.parentFunc = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_Child1_parentFunc_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['getVal'] = Child1.prototype.getVal = function() {
  var self = this.ptr;
  return _emscripten_bind_Child1_getVal_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['mulVal'] = Child1.prototype.mulVal = function(mul) {
  var self = this.ptr;
  if (mul && typeof mul === 'object') mul = mul.ptr;
  _emscripten_bind_Child1_mulVal_1(self, mul);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['getAsConst'] = Child1.prototype.getAsConst = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_Child1_getAsConst_0(self), Parent);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['voidStar'] = Child1.prototype.voidStar = function(something) {
  var self = this.ptr;
  if (something && typeof something === 'object') something = something.ptr;
  return wrapPointer(_emscripten_bind_Child1_voidStar_1(self, something), VoidPtr);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['getBoolean'] = Child1.prototype.getBoolean = function() {
  var self = this.ptr;
  return !!(_emscripten_bind_Child1_getBoolean_0(self));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['get_attr'] = Child1.prototype.get_attr = function() {
  var self = this.ptr;
  return _emscripten_bind_Child1_get_attr_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['set_attr'] = Child1.prototype.set_attr = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_Child1_set_attr_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(Child1.prototype, 'attr', { get: Child1.prototype.get_attr, set: Child1.prototype.set_attr });
/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['get_immutableAttr'] = Child1.prototype.get_immutableAttr = function() {
  var self = this.ptr;
  return _emscripten_bind_Child1_get_immutableAttr_0(self);
};

/** @suppress {checkTypes} */
Object.defineProperty(Child1.prototype, 'immutableAttr', { get: Child1.prototype.get_immutableAttr });

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child1.prototype['__destroy__'] = Child1.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Child1___destroy___0(self);
};

// Interface: Child2JS

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Child2JS() {
  this.ptr = _emscripten_bind_Child2JS_Child2JS_0();
  getCache(Child2JS)[this.ptr] = this;
};

Child2JS.prototype = Object.create(Child2.prototype);
Child2JS.prototype.constructor = Child2JS;
Child2JS.prototype.__class__ = Child2JS;
Child2JS.__cache__ = {};
Module['Child2JS'] = Child2JS;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2JS.prototype['virtualFunc'] = Child2JS.prototype.virtualFunc = function() {
  var self = this.ptr;
  _emscripten_bind_Child2JS_virtualFunc_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2JS.prototype['virtualFunc2'] = Child2JS.prototype.virtualFunc2 = function() {
  var self = this.ptr;
  _emscripten_bind_Child2JS_virtualFunc2_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2JS.prototype['virtualFunc3'] = Child2JS.prototype.virtualFunc3 = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_Child2JS_virtualFunc3_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2JS.prototype['virtualFunc4'] = Child2JS.prototype.virtualFunc4 = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_Child2JS_virtualFunc4_1(self, x);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
Child2JS.prototype['__destroy__'] = Child2JS.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Child2JS___destroy___0(self);
};

// Interface: ConcreteJS

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ConcreteJS() {
  this.ptr = _emscripten_bind_ConcreteJS_ConcreteJS_0();
  getCache(ConcreteJS)[this.ptr] = this;
};

ConcreteJS.prototype = Object.create(VirtualBase.prototype);
ConcreteJS.prototype.constructor = ConcreteJS;
ConcreteJS.prototype.__class__ = ConcreteJS;
ConcreteJS.__cache__ = {};
Module['ConcreteJS'] = ConcreteJS;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ConcreteJS.prototype['func'] = ConcreteJS.prototype.func = function() {
  var self = this.ptr;
  _emscripten_bind_ConcreteJS_func_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ConcreteJS.prototype['constFunc'] = ConcreteJS.prototype.constFunc = function() {
  var self = this.ptr;
  _emscripten_bind_ConcreteJS_constFunc_0(self);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
ConcreteJS.prototype['__destroy__'] = ConcreteJS.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_ConcreteJS___destroy___0(self);
};

// Interface: StringUser

/** @suppress {undefinedVars, duplicate} @this{Object} */
function StringUser(str, i) {
  ensureCache.prepare();
  if (str && typeof str === 'object') str = str.ptr;
  else str = ensureString(str);
  if (i && typeof i === 'object') i = i.ptr;
  if (str === undefined) { this.ptr = _emscripten_bind_StringUser_StringUser_0(); getCache(StringUser)[this.ptr] = this;return }
  if (i === undefined) { this.ptr = _emscripten_bind_StringUser_StringUser_1(str); getCache(StringUser)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_StringUser_StringUser_2(str, i);
  getCache(StringUser)[this.ptr] = this;
};

StringUser.prototype = Object.create(WrapperObject.prototype);
StringUser.prototype.constructor = StringUser;
StringUser.prototype.__class__ = StringUser;
StringUser.__cache__ = {};
Module['StringUser'] = StringUser;
/** @suppress {undefinedVars, duplicate} @this{Object} */
StringUser.prototype['Print'] = StringUser.prototype.Print = function(anotherInteger, anotherString) {
  var self = this.ptr;
  ensureCache.prepare();
  if (anotherInteger && typeof anotherInteger === 'object') anotherInteger = anotherInteger.ptr;
  if (anotherString && typeof anotherString === 'object') anotherString = anotherString.ptr;
  else anotherString = ensureString(anotherString);
  _emscripten_bind_StringUser_Print_2(self, anotherInteger, anotherString);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StringUser.prototype['PrintFloat'] = StringUser.prototype.PrintFloat = function(f) {
  var self = this.ptr;
  if (f && typeof f === 'object') f = f.ptr;
  _emscripten_bind_StringUser_PrintFloat_1(self, f);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StringUser.prototype['returnAString'] = StringUser.prototype.returnAString = function() {
  var self = this.ptr;
  return UTF8ToString(_emscripten_bind_StringUser_returnAString_0(self));
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
StringUser.prototype['__destroy__'] = StringUser.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_StringUser___destroy___0(self);
};

// Interface: RefUser

/** @suppress {undefinedVars, duplicate} @this{Object} */
function RefUser(value) {
  if (value && typeof value === 'object') value = value.ptr;
  if (value === undefined) { this.ptr = _emscripten_bind_RefUser_RefUser_0(); getCache(RefUser)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_RefUser_RefUser_1(value);
  getCache(RefUser)[this.ptr] = this;
};

RefUser.prototype = Object.create(WrapperObject.prototype);
RefUser.prototype.constructor = RefUser;
RefUser.prototype.__class__ = RefUser;
RefUser.__cache__ = {};
Module['RefUser'] = RefUser;
/** @suppress {undefinedVars, duplicate} @this{Object} */
RefUser.prototype['getValue'] = RefUser.prototype.getValue = function(b) {
  var self = this.ptr;
  if (b && typeof b === 'object') b = b.ptr;
  return _emscripten_bind_RefUser_getValue_1(self, b);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
RefUser.prototype['getMe'] = RefUser.prototype.getMe = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_RefUser_getMe_0(self), RefUser);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
RefUser.prototype['getCopy'] = RefUser.prototype.getCopy = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_RefUser_getCopy_0(self), RefUser);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
RefUser.prototype['getAnother'] = RefUser.prototype.getAnother = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_RefUser_getAnother_0(self), StringUser);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
RefUser.prototype['__destroy__'] = RefUser.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_RefUser___destroy___0(self);
};

// Interface: VoidPointerUser

/** @suppress {undefinedVars, duplicate} @this{Object} */
function VoidPointerUser() {
  this.ptr = _emscripten_bind_VoidPointerUser_VoidPointerUser_0();
  getCache(VoidPointerUser)[this.ptr] = this;
};

VoidPointerUser.prototype = Object.create(WrapperObject.prototype);
VoidPointerUser.prototype.constructor = VoidPointerUser;
VoidPointerUser.prototype.__class__ = VoidPointerUser;
VoidPointerUser.__cache__ = {};
Module['VoidPointerUser'] = VoidPointerUser;
/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPointerUser.prototype['GetVoidPointer'] = VoidPointerUser.prototype.GetVoidPointer = function() {
  var self = this.ptr;
  return _emscripten_bind_VoidPointerUser_GetVoidPointer_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPointerUser.prototype['SetVoidPointer'] = VoidPointerUser.prototype.SetVoidPointer = function(ptr) {
  var self = this.ptr;
  if (ptr && typeof ptr === 'object') ptr = ptr.ptr;
  _emscripten_bind_VoidPointerUser_SetVoidPointer_1(self, ptr);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
VoidPointerUser.prototype['__destroy__'] = VoidPointerUser.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPointerUser___destroy___0(self);
};

// Interface: Inner

/** @suppress {undefinedVars, duplicate} @this{Object} */
function Inner(value) {
  if (value && typeof value === 'object') value = value.ptr;
  if (value === undefined) { this.ptr = _emscripten_bind_Inner_Inner_0(); getCache(Inner)[this.ptr] = this;return }
  this.ptr = _emscripten_bind_Inner_Inner_1(value);
  getCache(Inner)[this.ptr] = this;
};

Inner.prototype = Object.create(WrapperObject.prototype);
Inner.prototype.constructor = Inner;
Inner.prototype.__class__ = Inner;
Inner.__cache__ = {};
Module['Inner'] = Inner;
/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['get'] = Inner.prototype.get = function() {
  var self = this.ptr;
  return _emscripten_bind_Inner_get_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['get_value'] = Inner.prototype.get_value = function() {
  var self = this.ptr;
  return _emscripten_bind_Inner_get_value_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['mul'] = Inner.prototype.mul = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  return wrapPointer(_emscripten_bind_Inner_mul_1(self, x), Inner);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['getAsArray'] = Inner.prototype.getAsArray = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  return _emscripten_bind_Inner_getAsArray_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['incInPlace'] = Inner.prototype.incInPlace = function(i) {
  var self = this.ptr;
  if (i && typeof i === 'object') i = i.ptr;
  _emscripten_bind_Inner_incInPlace_1(self, i);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['add'] = Inner.prototype.add = function(i) {
  var self = this.ptr;
  if (i && typeof i === 'object') i = i.ptr;
  return wrapPointer(_emscripten_bind_Inner_add_1(self, i), Inner);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['mul2'] = Inner.prototype.mul2 = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  return _emscripten_bind_Inner_mul2_1(self, x);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
Inner.prototype['__destroy__'] = Inner.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_Inner___destroy___0(self);
};

// Interface: InnerUser

/** @suppress {undefinedVars, duplicate} @this{Object} */
function InnerUser() {
  this.ptr = _emscripten_bind_InnerUser_InnerUser_0();
  getCache(InnerUser)[this.ptr] = this;
};

InnerUser.prototype = Object.create(InnerUserBase.prototype);
InnerUser.prototype.constructor = InnerUser;
InnerUser.prototype.__class__ = InnerUser;
InnerUser.__cache__ = {};
Module['InnerUser'] = InnerUser;
/** @suppress {undefinedVars, duplicate} @this{Object} */
InnerUser.prototype['Callback'] = InnerUser.prototype.Callback = function(inner) {
  var self = this.ptr;
  if (inner && typeof inner === 'object') inner = inner.ptr;
  _emscripten_bind_InnerUser_Callback_1(self, inner);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
InnerUser.prototype['__destroy__'] = InnerUser.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_InnerUser___destroy___0(self);
};

// Interface: EnumClass

/** @suppress {undefinedVars, duplicate} @this{Object} */
function EnumClass() {
  this.ptr = _emscripten_bind_EnumClass_EnumClass_0();
  getCache(EnumClass)[this.ptr] = this;
};

EnumClass.prototype = Object.create(WrapperObject.prototype);
EnumClass.prototype.constructor = EnumClass;
EnumClass.prototype.__class__ = EnumClass;
EnumClass.__cache__ = {};
Module['EnumClass'] = EnumClass;
/** @suppress {undefinedVars, duplicate} @this{Object} */
EnumClass.prototype['GetEnum'] = EnumClass.prototype.GetEnum = function() {
  var self = this.ptr;
  return _emscripten_bind_EnumClass_GetEnum_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
EnumClass.prototype['GetEnumFromNameSpace'] = EnumClass.prototype.GetEnumFromNameSpace = function() {
  var self = this.ptr;
  return _emscripten_bind_EnumClass_GetEnumFromNameSpace_0(self);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
EnumClass.prototype['__destroy__'] = EnumClass.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_EnumClass___destroy___0(self);
};

// Interface: TypeTestClass

/** @suppress {undefinedVars, duplicate} @this{Object} */
function TypeTestClass() {
  this.ptr = _emscripten_bind_TypeTestClass_TypeTestClass_0();
  getCache(TypeTestClass)[this.ptr] = this;
};

TypeTestClass.prototype = Object.create(WrapperObject.prototype);
TypeTestClass.prototype.constructor = TypeTestClass;
TypeTestClass.prototype.__class__ = TypeTestClass;
TypeTestClass.__cache__ = {};
Module['TypeTestClass'] = TypeTestClass;
/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['ReturnCharMethod'] = TypeTestClass.prototype.ReturnCharMethod = function() {
  var self = this.ptr;
  return _emscripten_bind_TypeTestClass_ReturnCharMethod_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['AcceptCharMethod'] = TypeTestClass.prototype.AcceptCharMethod = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_TypeTestClass_AcceptCharMethod_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['ReturnUnsignedCharMethod'] = TypeTestClass.prototype.ReturnUnsignedCharMethod = function() {
  var self = this.ptr;
  return _emscripten_bind_TypeTestClass_ReturnUnsignedCharMethod_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['AcceptUnsignedCharMethod'] = TypeTestClass.prototype.AcceptUnsignedCharMethod = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_TypeTestClass_AcceptUnsignedCharMethod_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['ReturnUnsignedShortMethod'] = TypeTestClass.prototype.ReturnUnsignedShortMethod = function() {
  var self = this.ptr;
  return _emscripten_bind_TypeTestClass_ReturnUnsignedShortMethod_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['AcceptUnsignedShortMethod'] = TypeTestClass.prototype.AcceptUnsignedShortMethod = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_TypeTestClass_AcceptUnsignedShortMethod_1(self, x);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['ReturnUnsignedLongMethod'] = TypeTestClass.prototype.ReturnUnsignedLongMethod = function() {
  var self = this.ptr;
  return _emscripten_bind_TypeTestClass_ReturnUnsignedLongMethod_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['AcceptUnsignedLongMethod'] = TypeTestClass.prototype.AcceptUnsignedLongMethod = function(x) {
  var self = this.ptr;
  if (x && typeof x === 'object') x = x.ptr;
  _emscripten_bind_TypeTestClass_AcceptUnsignedLongMethod_1(self, x);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
TypeTestClass.prototype['__destroy__'] = TypeTestClass.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_TypeTestClass___destroy___0(self);
};

// Interface: StructInArray

/** @suppress {undefinedVars, duplicate} @this{Object} */
function StructInArray(attr1, attr2) {
  if (attr1 && typeof attr1 === 'object') attr1 = attr1.ptr;
  if (attr2 && typeof attr2 === 'object') attr2 = attr2.ptr;
  this.ptr = _emscripten_bind_StructInArray_StructInArray_2(attr1, attr2);
  getCache(StructInArray)[this.ptr] = this;
};

StructInArray.prototype = Object.create(WrapperObject.prototype);
StructInArray.prototype.constructor = StructInArray;
StructInArray.prototype.__class__ = StructInArray;
StructInArray.__cache__ = {};
Module['StructInArray'] = StructInArray;
/** @suppress {undefinedVars, duplicate} @this{Object} */
StructInArray.prototype['get_attr1'] = StructInArray.prototype.get_attr1 = function() {
  var self = this.ptr;
  return _emscripten_bind_StructInArray_get_attr1_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StructInArray.prototype['set_attr1'] = StructInArray.prototype.set_attr1 = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_StructInArray_set_attr1_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(StructInArray.prototype, 'attr1', { get: StructInArray.prototype.get_attr1, set: StructInArray.prototype.set_attr1 });
/** @suppress {undefinedVars, duplicate} @this{Object} */
StructInArray.prototype['get_attr2'] = StructInArray.prototype.get_attr2 = function() {
  var self = this.ptr;
  return _emscripten_bind_StructInArray_get_attr2_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StructInArray.prototype['set_attr2'] = StructInArray.prototype.set_attr2 = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_StructInArray_set_attr2_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(StructInArray.prototype, 'attr2', { get: StructInArray.prototype.get_attr2, set: StructInArray.prototype.set_attr2 });

/** @suppress {undefinedVars, duplicate} @this{Object} */
StructInArray.prototype['__destroy__'] = StructInArray.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_StructInArray___destroy___0(self);
};

// Interface: ArrayClass

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ArrayClass() {
  this.ptr = _emscripten_bind_ArrayClass_ArrayClass_0();
  getCache(ArrayClass)[this.ptr] = this;
};

ArrayClass.prototype = Object.create(WrapperObject.prototype);
ArrayClass.prototype.constructor = ArrayClass;
ArrayClass.prototype.__class__ = ArrayClass;
ArrayClass.__cache__ = {};
Module['ArrayClass'] = ArrayClass;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['get_int_array'] = ArrayClass.prototype.get_int_array = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return _emscripten_bind_ArrayClass_get_int_array_1(self, arg0);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['set_int_array'] = ArrayClass.prototype.set_int_array = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_ArrayClass_set_int_array_2(self, arg0, arg1);
};

/** @suppress {checkTypes} */
Object.defineProperty(ArrayClass.prototype, 'int_array', { get: ArrayClass.prototype.get_int_array, set: ArrayClass.prototype.set_int_array });
/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['get_struct_array'] = ArrayClass.prototype.get_struct_array = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return wrapPointer(_emscripten_bind_ArrayClass_get_struct_array_1(self, arg0), StructInArray);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['set_struct_array'] = ArrayClass.prototype.set_struct_array = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_ArrayClass_set_struct_array_2(self, arg0, arg1);
};

/** @suppress {checkTypes} */
Object.defineProperty(ArrayClass.prototype, 'struct_array', { get: ArrayClass.prototype.get_struct_array, set: ArrayClass.prototype.set_struct_array });
/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['get_struct_ptr_array'] = ArrayClass.prototype.get_struct_ptr_array = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return wrapPointer(_emscripten_bind_ArrayClass_get_struct_ptr_array_1(self, arg0), StructInArray);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['set_struct_ptr_array'] = ArrayClass.prototype.set_struct_ptr_array = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_ArrayClass_set_struct_ptr_array_2(self, arg0, arg1);
};

/** @suppress {checkTypes} */
Object.defineProperty(ArrayClass.prototype, 'struct_ptr_array', { get: ArrayClass.prototype.get_struct_ptr_array, set: ArrayClass.prototype.set_struct_ptr_array });

/** @suppress {undefinedVars, duplicate} @this{Object} */
ArrayClass.prototype['__destroy__'] = ArrayClass.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_ArrayClass___destroy___0(self);
};

// Interface: ReceiveArrays

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ReceiveArrays() {
  this.ptr = _emscripten_bind_ReceiveArrays_ReceiveArrays_0();
  getCache(ReceiveArrays)[this.ptr] = this;
};

ReceiveArrays.prototype = Object.create(WrapperObject.prototype);
ReceiveArrays.prototype.constructor = ReceiveArrays;
ReceiveArrays.prototype.__class__ = ReceiveArrays;
ReceiveArrays.__cache__ = {};
Module['ReceiveArrays'] = ReceiveArrays;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ReceiveArrays.prototype['giveMeArrays'] = ReceiveArrays.prototype.giveMeArrays = function(vertices, triangles, num) {
  var self = this.ptr;
  ensureCache.prepare();
  if (typeof vertices == 'object') { vertices = ensureFloat32(vertices); }
  if (typeof triangles == 'object') { triangles = ensureInt32(triangles); }
  if (num && typeof num === 'object') num = num.ptr;
  _emscripten_bind_ReceiveArrays_giveMeArrays_3(self, vertices, triangles, num);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
ReceiveArrays.prototype['__destroy__'] = ReceiveArrays.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_ReceiveArrays___destroy___0(self);
};

// Interface: StoreArray

/** @suppress {undefinedVars, duplicate} @this{Object} */
function StoreArray() {
  this.ptr = _emscripten_bind_StoreArray_StoreArray_0();
  getCache(StoreArray)[this.ptr] = this;
};

StoreArray.prototype = Object.create(WrapperObject.prototype);
StoreArray.prototype.constructor = StoreArray;
StoreArray.prototype.__class__ = StoreArray;
StoreArray.__cache__ = {};
Module['StoreArray'] = StoreArray;
/** @suppress {undefinedVars, duplicate} @this{Object} */
StoreArray.prototype['setArray'] = StoreArray.prototype.setArray = function(array) {
  var self = this.ptr;
  ensureCache.prepare();
  if (typeof array == 'object') { array = ensureInt32(array); }
  _emscripten_bind_StoreArray_setArray_1(self, array);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
StoreArray.prototype['getArrayValue'] = StoreArray.prototype.getArrayValue = function(index) {
  var self = this.ptr;
  if (index && typeof index === 'object') index = index.ptr;
  return _emscripten_bind_StoreArray_getArrayValue_1(self, index);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
StoreArray.prototype['__destroy__'] = StoreArray.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_StoreArray___destroy___0(self);
};

// Interface: LongLongTypes

/** @suppress {undefinedVars, duplicate} @this{Object} */
function LongLongTypes() { throw "cannot construct a LongLongTypes, no constructor in IDL" }
LongLongTypes.prototype = Object.create(WrapperObject.prototype);
LongLongTypes.prototype.constructor = LongLongTypes;
LongLongTypes.prototype.__class__ = LongLongTypes;
LongLongTypes.__cache__ = {};
Module['LongLongTypes'] = LongLongTypes;
/** @suppress {undefinedVars, duplicate} @this{Object} */
LongLongTypes.prototype['get_lluArray'] = LongLongTypes.prototype.get_lluArray = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  return _emscripten_bind_LongLongTypes_get_lluArray_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(LongLongTypes.prototype, 'lluArray', { get: LongLongTypes.prototype.get_lluArray });
/** @suppress {undefinedVars, duplicate} @this{Object} */
LongLongTypes.prototype['get_ll'] = LongLongTypes.prototype.get_ll = function() {
  var self = this.ptr;
  return _emscripten_bind_LongLongTypes_get_ll_0(self);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
LongLongTypes.prototype['set_ll'] = LongLongTypes.prototype.set_ll = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_LongLongTypes_set_ll_1(self, arg0);
};

/** @suppress {checkTypes} */
Object.defineProperty(LongLongTypes.prototype, 'll', { get: LongLongTypes.prototype.get_ll, set: LongLongTypes.prototype.set_ll });

/** @suppress {undefinedVars, duplicate} @this{Object} */
LongLongTypes.prototype['__destroy__'] = LongLongTypes.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_LongLongTypes___destroy___0(self);
};

// Interface: JSSmallObject

/** @suppress {undefinedVars, duplicate} @this{Object} */
function JSSmallObject() {
  this.ptr = _emscripten_bind_JSSmallObject_JSSmallObject_0();
  getCache(JSSmallObject)[this.ptr] = this;
};

JSSmallObject.prototype = Object.create(ISmallObject.prototype);
JSSmallObject.prototype.constructor = JSSmallObject;
JSSmallObject.prototype.__class__ = JSSmallObject;
JSSmallObject.__cache__ = {};
Module['JSSmallObject'] = JSSmallObject;
/** @suppress {undefinedVars, duplicate} @this{Object} */
JSSmallObject.prototype['getID'] = JSSmallObject.prototype.getID = function(number) {
  var self = this.ptr;
  if (number && typeof number === 'object') number = number.ptr;
  return _emscripten_bind_JSSmallObject_getID_1(self, number);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
JSSmallObject.prototype['__destroy__'] = JSSmallObject.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_JSSmallObject___destroy___0(self);
};

// Interface: JSObjectProvider

/** @suppress {undefinedVars, duplicate} @this{Object} */
function JSObjectProvider() {
  this.ptr = _emscripten_bind_JSObjectProvider_JSObjectProvider_0();
  getCache(JSObjectProvider)[this.ptr] = this;
};

JSObjectProvider.prototype = Object.create(IObjectProvider.prototype);
JSObjectProvider.prototype.constructor = JSObjectProvider;
JSObjectProvider.prototype.__class__ = JSObjectProvider;
JSObjectProvider.__cache__ = {};
Module['JSObjectProvider'] = JSObjectProvider;
/** @suppress {undefinedVars, duplicate} @this{Object} */
JSObjectProvider.prototype['getObject'] = JSObjectProvider.prototype.getObject = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_JSObjectProvider_getObject_0(self), JSSmallObject);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
JSObjectProvider.prototype['__destroy__'] = JSObjectProvider.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_JSObjectProvider___destroy___0(self);
};

// Interface: ObjectFactory

/** @suppress {undefinedVars, duplicate} @this{Object} */
function ObjectFactory() {
  this.ptr = _emscripten_bind_ObjectFactory_ObjectFactory_0();
  getCache(ObjectFactory)[this.ptr] = this;
};

ObjectFactory.prototype = Object.create(WrapperObject.prototype);
ObjectFactory.prototype.constructor = ObjectFactory;
ObjectFactory.prototype.__class__ = ObjectFactory;
ObjectFactory.__cache__ = {};
Module['ObjectFactory'] = ObjectFactory;
/** @suppress {undefinedVars, duplicate} @this{Object} */
ObjectFactory.prototype['getProvider'] = ObjectFactory.prototype.getProvider = function() {
  var self = this.ptr;
  return wrapPointer(_emscripten_bind_ObjectFactory_getProvider_0(self), IObjectProvider);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
ObjectFactory.prototype['__destroy__'] = ObjectFactory.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_ObjectFactory___destroy___0(self);
};

// Interface: JSArrayArgumentTest

/** @suppress {undefinedVars, duplicate} @this{Object} */
function JSArrayArgumentTest() {
  this.ptr = _emscripten_bind_JSArrayArgumentTest_JSArrayArgumentTest_0();
  getCache(JSArrayArgumentTest)[this.ptr] = this;
};

JSArrayArgumentTest.prototype = Object.create(ArrayArgumentTest.prototype);
JSArrayArgumentTest.prototype.constructor = JSArrayArgumentTest;
JSArrayArgumentTest.prototype.__class__ = JSArrayArgumentTest;
JSArrayArgumentTest.__cache__ = {};
Module['JSArrayArgumentTest'] = JSArrayArgumentTest;
/** @suppress {undefinedVars, duplicate} @this{Object} */
JSArrayArgumentTest.prototype['byteArrayTest'] = JSArrayArgumentTest.prototype.byteArrayTest = function(arg) {
  var self = this.ptr;
  ensureCache.prepare();
  if (typeof arg == 'object') { arg = ensureInt8(arg); }
  return !!(_emscripten_bind_JSArrayArgumentTest_byteArrayTest_1(self, arg));
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
JSArrayArgumentTest.prototype['domStringTest'] = JSArrayArgumentTest.prototype.domStringTest = function(arg) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg && typeof arg === 'object') arg = arg.ptr;
  else arg = ensureString(arg);
  return !!(_emscripten_bind_JSArrayArgumentTest_domStringTest_1(self, arg));
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
JSArrayArgumentTest.prototype['__destroy__'] = JSArrayArgumentTest.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_JSArrayArgumentTest___destroy___0(self);
};

// Interface: BindToTest

/** @suppress {undefinedVars, duplicate} @this{Object} */
function BindToTest() {
  this.ptr = _emscripten_bind_BindToTest_BindToTest_0();
  getCache(BindToTest)[this.ptr] = this;
};

BindToTest.prototype = Object.create(WrapperObject.prototype);
BindToTest.prototype.constructor = BindToTest;
BindToTest.prototype.__class__ = BindToTest;
BindToTest.__cache__ = {};
Module['BindToTest'] = BindToTest;
/** @suppress {undefinedVars, duplicate} @this{Object} */
BindToTest.prototype['testString'] = BindToTest.prototype.testString = function(arg) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg && typeof arg === 'object') arg = arg.ptr;
  else arg = ensureString(arg);
  return _emscripten_bind_BindToTest_testString_1(self, arg);
};

/** @suppress {undefinedVars, duplicate} @this{Object} */
BindToTest.prototype['testInt'] = BindToTest.prototype.testInt = function(arg) {
  var self = this.ptr;
  if (arg && typeof arg === 'object') arg = arg.ptr;
  return _emscripten_bind_BindToTest_testInt_1(self, arg);
};


/** @suppress {undefinedVars, duplicate} @this{Object} */
BindToTest.prototype['__destroy__'] = BindToTest.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_BindToTest___destroy___0(self);
};

(function() {
  function setupEnums() {
    
// $AnEnum

    Module['enum_value1'] = _emscripten_enum_AnEnum_enum_value1();

    Module['enum_value2'] = _emscripten_enum_AnEnum_enum_value2();

    
// $EnumClass_EnumWithinClass

    Module['EnumClass']['e_val'] = _emscripten_enum_EnumClass_EnumWithinClass_e_val();

    
// $EnumNamespace_EnumInNamespace

    Module['e_namespace_val'] = _emscripten_enum_EnumNamespace_EnumInNamespace_e_namespace_val();

  }
  if (runtimeInitialized) setupEnums();
  else addOnInit(setupEnums);
})();
