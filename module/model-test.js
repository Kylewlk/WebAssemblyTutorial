
const mymPromise = Mym();

console.log(mymPromise);

mymPromise.then(async (instance) => {
  /**
   * @type {import('./types').Mym}
   */
  const MymInstance = instance;

  console.log(MymInstance);

  const obj1 = new MymInstance.MyObject(1);
  obj1.printInfo();
  console.log("obj1 n=" + obj1.getN());
  MymInstance.destroy(obj1);

  const obj2 = await MymInstance.MyObject.prototype.createAsync(2);
  console.log(obj2);
  obj2.printInfo();
  MymInstance.destroy(obj2);

  const promise3 = MymInstance.MyObject.prototype.createAsync(-3);
  promise3
    .then((obj3) => {
      console.log(obj3);
      obj3.printInfo();
      MymInstance.destroy(obj3);
    })
    .catch((err) => {
      console.log(err);
    });
});




