# Install essdk

安装参考：[install](https://emscripten.org/docs/getting_started/downloads.html)

## export to enveriment

Mac OS 中把下面代码添加到 `~/.zprofile` 中
``` bash
source "/Users/kyle/programs/emsdk/emsdk_env.sh"
```
## uninstall
卸载参考：[uninstall](https://emscripten.org/docs/tools_reference/emsdk.html#emsdk-remove-tool-sdk)


# cmake
[WASM CMake](https://floooh.github.io/2023/11/11/emscripten-ide.html)


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
`$env{EMSDK}` 为 emsdk安装目录，安装时会自动配置 `EMSDK` 环境变量
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
      "generator": "Unix Makefiles",
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