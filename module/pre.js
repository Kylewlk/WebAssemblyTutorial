// var Module = {
//   'print': function(text) { console.log('[info]', text)  },
//   'printErr': function(text) { console.log('[error]', text)  },
// };


if (!Module) {
  Module = {};
}

if (!Module['disableLogInfo']) {
  Module['print'] = function(text) { console.log('[info]', text)  };
} else {
  Module['print'] = function(text) { };
}

if (!Module['disableLogError']) {
  Module['printErr'] = function(text) { console.error('[error]', text)  };
} else {
  Module['printErr'] = function(text) { };
}

// Object.assign(Module, {
//   'print': function(text) { console.log('[info]', text)  },
//   'printErr': function(text) { console.log('[error]', text)  },
// });