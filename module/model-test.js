

Mym().then((MymInstance) => {

    console.log(MymInstance);

    const obj1 = new MymInstance.MyObject(1);
    obj1.printInfo();
    MymInstance.destroy(obj1);

    let obj2 = null;
    const cb = (obj) => {
        obj2 = obj;

        console.log(obj2);
        obj2.printInfo();
        MymInstance.destroy(obj2);
    }

    const callback = (obj) =>{
        const wobj = obj !== 0 ? MymInstance.wrapPointer(obj, MymInstance.MyObject) : null;
        cb(wobj);
    }

    const callbackPoint = MymInstance.addFunction(callback, 'vi'); // 低版本 emsdk 用 i 代替 p
    MymInstance.MyObject.prototype.createAsync(2, callbackPoint);
    // console.log(obj2);
    // obj2.printInfo();
    // MymInstance.destroy(obj2);

});




