

// begin post JS

function scan(path) {
  console.log(path);
  FS.readdir(path).forEach(item => {
    if (item !== '.' && item !== '..') {
      try {
        scan(path + (path.endsWith('/') ? '' : '/') + item);
      } catch (e) { }
    }
  });
}
// scan('/');

// end post JS
