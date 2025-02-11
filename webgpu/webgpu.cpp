#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu.h>
#include <iostream>
#include <format>
#include <thread>

#define LOGI(...) do{ std::cout << std::format(__VA_ARGS__) << std::endl; }while (false)
// error
#define LOGE(...) do{ std::cerr << std::format(__VA_ARGS__) << std::endl; }while (false)
// warning
#define LOGW(...) do{ std::cerr << std::format(__VA_ARGS__) << std::endl; }while (false)


WGPUInstance instance{};
WGPUAdapter adapter{};
WGPUDevice device{};
WGPUQueue queue{};
WGPUSurface surface{};
WGPUTextureFormat preferredFormat = WGPUTextureFormat_RGBA8Unorm;

void initWebGpu()
{
    instance = wgpuCreateInstance(nullptr);

    // create a surface
    {
        WGPUSurfaceDescriptorFromCanvasHTMLSelector fromCanvasHTMLSelector;
        fromCanvasHTMLSelector.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
        fromCanvasHTMLSelector.chain.next = NULL;
        fromCanvasHTMLSelector.selector = "canvas"; // <canvas> id
        WGPUSurfaceDescriptor surfaceDescriptor;
        surfaceDescriptor.nextInChain = &fromCanvasHTMLSelector.chain;
        surfaceDescriptor.label = NULL;
        surface = wgpuInstanceCreateSurface(instance, &surfaceDescriptor);
    }
    WGPURequestAdapterOptions options = {};
    options.backendType = WGPUBackendType_WebGPU;
    options.compatibleSurface = surface;
    options.powerPreference = WGPUPowerPreference_HighPerformance;

    static std::atomic<bool> done = false;
    wgpuInstanceRequestAdapter(
        instance, &options,
        [](WGPURequestAdapterStatus status, WGPUAdapter ad, char const *message, void *userdata)
        {
            if (status != WGPURequestAdapterStatus_Success)
            {
                LOGE("wgpuInstanceRequestAdapter error: {}", (int)status);
            }
            adapter = ad;
            done = true;
        }, nullptr
    );
    while (!done)
    {
        emscripten_sleep(10);
    }
    
    WGPUAdapterProperties adapterProperties = {};
    wgpuAdapterGetProperties(adapter, &adapterProperties);

    preferredFormat = wgpuSurfaceGetPreferredFormat(surface, adapter);
    if (preferredFormat == WGPUTextureFormat_Undefined)
    {
        preferredFormat = WGPUTextureFormat_RGBA8Unorm;
    }

    WGPUDeviceDescriptor deviceDescriptor = {};
    done = false;
    wgpuAdapterRequestDevice(
        adapter, &deviceDescriptor,
        [](WGPURequestDeviceStatus status, WGPUDevice de, char const *message, void *userdata)
        {
            if (status != WGPURequestDeviceStatus_Success)
            {
                LOGE("wgpuAdapterRequestDevice error: {}", (int)status);
            }
            device = de;
            done = true;
        }, nullptr
    );
    while (!done)
    {
        emscripten_sleep(10);
    }
    queue = wgpuDeviceGetQueue(device);

    wgpuDeviceSetUncapturedErrorCallback(
        device, [](WGPUErrorType type, char const *message, void *userdata)
        {
            // auto ctx = static_cast<WGContext*>(userdata);
            const char *errorType = "Unknown Error";
            switch (type)
            {
            case WGPUErrorType_NoError:
                errorType = "No error";
                break;
            case WGPUErrorType_Validation:
                errorType = "Validation failed";
                break;
            case WGPUErrorType_OutOfMemory:
                errorType = "Out of memory";
                break;
            case WGPUErrorType_Internal:
                errorType = "Internal error";
                break;
            case WGPUErrorType_DeviceLost:
                errorType = "Device lost";
                break;
            case WGPUErrorType_Force32:
                errorType = "Force 32bit access";
                break;
            case WGPUErrorType_Unknown:
            default:
                break;
            }
            std::cerr << "Device Uncaptured Error: " << errorType << ", " << message << std::endl;
        },nullptr
    );

    // config surface
    {
        int width{}, height{};
        emscripten_get_canvas_element_size("canvas", &width, &height);
        WGPUSurfaceConfiguration surfaceConfig = {};
        surfaceConfig.device = device;
        surfaceConfig.width = width;
        surfaceConfig.height = height;
        surfaceConfig.usage = WGPUTextureUsage_RenderAttachment;
        surfaceConfig.format = preferredFormat;
        surfaceConfig.presentMode = WGPUPresentMode_Fifo;

        wgpuSurfaceConfigure(surface, &surfaceConfig);
    }
}

bool render(double time, void *userData)
{
    WGPUSurfaceTexture surfaceTexture{};
    wgpuSurfaceGetCurrentTexture(surface, &surfaceTexture);
    if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
    {
        return true;
    }
    WGPUTextureView renderView = wgpuTextureCreateView(surfaceTexture.texture, nullptr);
    wgpuTextureRelease(surfaceTexture.texture);

    auto commandEncoder = wgpuDeviceCreateCommandEncoder(device, nullptr);
    wgpuCommandEncoderSetLabel(commandEncoder, "RenderCommand");

    WGPURenderPassDescriptor descriptor = {};

    WGPURenderPassColorAttachment colorAttachment{};
    colorAttachment.view = renderView;
    colorAttachment.loadOp = WGPULoadOp_Clear;
    colorAttachment.storeOp = WGPUStoreOp_Store;
    colorAttachment.clearValue = {0.0f, 0.6f, 0.8f, 1.0f};
#ifndef WEBGPU_BACKEND_WGPU
    colorAttachment.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
#endif
    descriptor.colorAttachmentCount = 1;
    descriptor.colorAttachments = &colorAttachment;
    auto renderPass = wgpuCommandEncoderBeginRenderPass(commandEncoder, &descriptor);

    wgpuRenderPassEncoderEnd(renderPass);

    auto command = wgpuCommandEncoderFinish(commandEncoder, nullptr);
    wgpuQueueSubmit(queue, 1, &command);
    wgpuCommandBufferRelease(command);
    wgpuCommandEncoderRelease(commandEncoder);

    return true; // 返回true继续回调
}

int main()
{
    initWebGpu();

    emscripten_request_animation_frame_loop(render, nullptr);
    return 0;
}





