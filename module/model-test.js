
const htmlPath = window.location.pathname;
const index = htmlPath.lastIndexOf('/');
const basePath = htmlPath.substring(0, index) +'/module.js';
console.log(basePath);

async function dynamicImportExample() {
  try {
    // 动态 import 支持字符串表达式
    const module = await import(basePath);
    // 使用导入的模块
    console.log(module);
    return module;
  } catch (error) {
    console.error('模块加载失败:', error);
  }
}

const mymPromise =dynamicImportExample().then((module) => {
  const Mym = module.default;
  return Mym();
});

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




