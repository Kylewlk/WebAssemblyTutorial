

// begin post JS

const MyObject_createAsync = MyObject.prototype["createAsync"];
MyObject.prototype["createAsync"] = (n) => {
  let promiseResolve;
  let promiseReject;
  const promise = new Promise((resolve, reject) => {
    promiseResolve = resolve;
    promiseReject = reject;
  });

  const callback = (obj) => {
    if (obj !== 0) {
      promiseResolve(wrapPointer(obj, MyObject));
    } else {
      promiseReject("create obj is null");
    }
  };
  const callbackPoint = addFunction(callback, "vi"); // 低版本 emsdk 用 i 代替 p
  MyObject_createAsync(n, callbackPoint);
  return promise;
};


// end post JS
