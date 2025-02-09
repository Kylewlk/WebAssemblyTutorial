# Install essdk

安装参考：[install](https://emscripten.org/docs/getting_started/downloads.html)

## export to enveriment

### Mac OS 
``` bash
# Download and install the latest SDK tools.
./emsdk install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
./emsdk activate latest

# 把下面代码添加到 ~/.zprofile 中, emsdk_env.sh会添加需要的环境变量
source "/Users/programs/emsdk/emsdk_env.sh" > /dev/null 2>&1

# 检查编译器配置
emcc -v

# 检查cmake是否可用
emcmake echo
```

### Windows
```powershell
# Download and install the latest SDK tools.
.\emsdk.bat install latest

# Make the "latest" SDK "active" for the current user. (writes .emscripten file)
.\emsdk.bat activate latest 

# 将环境变量信息写入用户环境变量中，如果vscode识别不到环境变量，可以注销或重启一下Windows系统
.\emsdk.bat activate --permanent

# 检查编译器配置
emcc.bat -v

# 检查cmake是否可用，需要先下载Ninja，并配置环境变量
emcmake.bat echo

```

## uninstall
卸载参考：[uninstall](https://emscripten.org/docs/tools_reference/emsdk.html#emsdk-remove-tool-sdk)



# 编译链接
## 编译选项
[编译选项参考](https://emscripten.org/docs/tools_reference/emcc.html)
* `-g` 生成调试信息
* `-sEXPORTED_FUNCTIONS=foo,bar` 设置导出给js使用的函数
* `-gseparate-dwarf[=FILENAME]` 将调试信息输出到单独的文件

## 编译输出
* 编译生成 .js `./emcc test/hello.c`, 生成的 js 可以使用node执行,从main函数开始执行，没有main函数，则不会执行。
* 编译输出 .html `./emcc test/hello_world.c -o hello.html` 打开html会执行main函数 
* 使用 `--shell-file=../shell.html` 将输出嵌套到指定html文件中


## 链接系统库
### 使用port
* 使用`--use-port=sdl2`链接指定的库
* `emcc --show-ports` 查询可用的库，
### 使用编译选项链接系统库
* 使用`-sUSE_SDL=1` 或 `-sFULL_ES3` 链接指定的库
* 查询支持的库，[Emscripten Compiler Settings](https://emscripten.org/docs/tools_reference/settings_reference.html) 

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

## cmake输出
* 使用`CMAKE_EXECUTABLE_SUFFIX` 指定输出 js 或者 html
```
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX .html)
    或者
    set(CMAKE_EXECUTABLE_SUFFIX .js)
endif()
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

# 其他
[视频教程 WebAssembly Tutorial](https://www.youtube.com/watch?v=_8T9T6MQ1fU&list=PLysLvOneEETPM_YbEyZcJ35_3pSdrj33O)

# Ref
* [WASM Debugging with Emscripten and VSCode](https://floooh.github.io/2023/11/11/emscripten-ide.html)
* [使用现代工具调试 WebAssembly](https://developer.chrome.com/blog/wasm-debugging-2020?hl=zh-cn)
* [https://github.com/floooh/vscode-emscripten-debugging](https://github.com/floooh/vscode-emscripten-debugging)