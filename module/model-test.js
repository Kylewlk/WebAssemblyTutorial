

Mym().then((MymInstance) => {

    console.log(MymInstance);

    const obj1 = new MymInstance.MyObject(1);
    obj1.printInfo();
    MymInstance.destroy(obj1);

});




