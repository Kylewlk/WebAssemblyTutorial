# Install essdk

安装参考：[install](https://emscripten.org/docs/getting_started/downloads.html)

## export to enveriment

### Mac OS 
把下面代码添加到 `~/.zprofile` 中
``` bash
// 使用 emsdk_env.sh会添加需要的环境变量
source "/Users/programs/emsdk/emsdk_env.sh" > /dev/null 2>&1

// 检查编译器配置
emcc -v

// 检查cmake是否可用
emcmake echo
```
## uninstall
卸载参考：[uninstall](https://emscripten.org/docs/tools_reference/emsdk.html#emsdk-remove-tool-sdk)


# cmake
* Mac OS 上可用使用 `Ninja` 或者  `Unix Makefiles`
* Windows 上可用使用 `Ninja`  或者 `MinGW Makefiles`
* 使用 `Ninja` 需要单独下载 [Ninja](https://github.com/ninja-build/ninja/releases)，配置PATH环境变量

## 使用 emcmake
emcmake 可以自动配置 cmake 变量
```bash

# 使用emcmake 生成 cmake 项目
emcmake cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build

#打印 emcc 使用cmake需要设置的变量 
emcmake echo 

```

## 使用CMakeUserPresets.json
`$env{EMSDK}` 为 emsdk安装目录，需要安装时配置了 `EMSDK` 环境变量
```json
{
  "version": 3,
  "cmakeMinimumRequired": {
    "major": 3,
    "minor": 21,
    "patch": 0
  },
  "configurePresets": [
    {
      "name": "default",
      "displayName": "Emscripten",
      "binaryDir": "cmake-build",
      "generator": "Ninja",
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release"
      },
      "toolchainFile": "$env{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
    }
  ]
}
```

```bash
cmake --preset default
cmake --build cmake-build
```

# 代码提示和错误检查
* 安装[ C/C++ Extension Pack ](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)插件
* 在.vscode文件夹中添加c_cpp_properties.json文件
* Windows上 `compilerPath` 需要使用 emcc.bat
```json
{
    "configurations": [
        {
            "name": "WebAssembly",
            "includePath": [
                "${env:EMSDK}/upstream/emscripten/system/include"
            ],
            "defines": [],
            "compilerPath": "${env:EMSDK}/upstream/emscripten/emcc", 
            "cStandard": "c17",
            "cppStandard": "c++20",
            "intelliSenseMode": "linux-clang-x64"
        }
    ],
    "version": 4
}
```

# 调试
## vscode直接运行调试
* 安装插件[WebAssembly DWARF Debugging](https://marketplace.visualstudio.com/items?itemName=ms-vscode.wasm-dwarf-debugging)
* 在.vscode目录中添加launch.json文件
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug",
            "type": "node",
            "request": "launch",
            "program": "${command:cmake.getLaunchTargetPath}"
        }
    ]
}
```

##  浏览器中运行调试
* vscode 安装 [Live Server](https://marketplace.visualstudio.com/items?itemName=ritwickdey.LiveServer) 插件
* 使用cmake编译debug版本 或者 `emcc` 命令编译时加上 `-g`

### vscode 启动chorme调试运行


* 在.vscode目录中添加launch.json文件,url替换为对应的启动页面，preLaunchTask用于启动LiveServer，如果想手动启动可以不需要这行
```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Launch",
            "type": "chrome",
            "request": "launch",
            "url": "http://127.0.0.1:5500/build/hello.html",
            "preLaunchTask": "StartServer"

        }
    ]
}
```
* 在.vscode目录中添加task.json文件，添加 `StartServer` Task 用于启动LiveServer
```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "StartServer",
            "type": "process",
            "command": "${input:startServer}"
        }
    ],
    "inputs": [
        {
            "id": "startServer",
            "type": "command",
            "command": "livePreview.runServerLoggingTask"
        }
    ]
}
```

### chrome中直接调试
* 安装chrome插件[C/C++ DevTools Support (DWARF)](https://chromewebstore.google.com/detail/cc++-devtools-support-dwa/pdcpmagijalfljmkmjngeonclgbbannb)
* 在Chrome开发者工具工具的 `Source` 中会多一个 `file://` 展开目录会包含C/C++源码，可以直接断点调试 


# Ref
* [WASM Debugging with Emscripten and VSCode](https://floooh.github.io/2023/11/11/emscripten-ide.html)
* [使用现代工具调试 WebAssembly](https://developer.chrome.com/blog/wasm-debugging-2020?hl=zh-cn)
* [https://github.com/floooh/vscode-emscripten-debugging](https://github.com/floooh/vscode-emscripten-debugging)