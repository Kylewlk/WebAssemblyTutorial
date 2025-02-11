#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <webgpu/webgpu.h>
#include <iostream>
#include <format>
#include <thread>
#include <string>

#define LOGI(...) do{ std::cout << std::format(__VA_ARGS__) << std::endl; }while (false)
// error
#define LOGE(...) do{ std::cerr << std::format(__VA_ARGS__) << std::endl; }while (false)
// warning
#define LOGW(...) do{ std::cerr << std::format(__VA_ARGS__) << std::endl; }while (false)

const std::string canvasID = "canvas"; // <canvas> id
WGPUInstance instance{};
WGPUAdapter adapter{};
WGPUDevice device{};
WGPUQueue queue{};
WGPUSurface surface{};
WGPUTextureFormat preferredFormat = WGPUTextureFormat_RGBA8Unorm;

WGPURenderPipeline pipeline{};
WGPUBindGroupLayout bindGroupLayout{};
WGPUSampler sampler{};
WGPUBuffer vertexBuffer{};

void initWebGpu()
{
    instance = wgpuCreateInstance(nullptr);

    // create a surface
    {
        WGPUSurfaceDescriptorFromCanvasHTMLSelector fromCanvasHTMLSelector;
        fromCanvasHTMLSelector.chain.sType = WGPUSType_SurfaceDescriptorFromCanvasHTMLSelector;
        fromCanvasHTMLSelector.chain.next = NULL;
        fromCanvasHTMLSelector.selector = canvasID.c_str();
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
        emscripten_get_canvas_element_size(canvasID.c_str(), &width, &height);
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

    wgpuTextureViewRelease(renderView);

    // wgpuSurfacePresent(surface); // 浏览器内部会调用present，无需再调用
    return true; // 返回true继续回调
}

constexpr const char *shaderSource = R"(
@vertex fn vs(@location(0) pos : vec4f, @location(1) uv : vec2f) -> @builtin(position) vec4f {
    return pos;
}

@fragment fn fs(@location(0) uv: vec2f) -> @location(0) vec4f {
    return vec4f(uv, 0, 1);
}
)";

WGPUShaderModule createShaderModule(const char *source)
{
    WGPUShaderModuleWGSLDescriptor shaderCodeDescriptor{};
    shaderCodeDescriptor.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    shaderCodeDescriptor.chain.next = nullptr;
    shaderCodeDescriptor.code = source;

    WGPUShaderModuleDescriptor shaderDescriptor{};
    shaderDescriptor.nextInChain = &shaderCodeDescriptor.chain;

    return wgpuDeviceCreateShaderModule(device, &shaderDescriptor);
}


void initDraw()
{
    {
        auto shaderModule = createShaderModule(shaderSource);

        std::array<WGPUVertexAttribute, 2> attributes{};
        auto &position = attributes[0];
        position.format = WGPUVertexFormat_Float32x2;
        position.offset = 0;
        position.shaderLocation = 0;

        auto &uv = attributes[1];
        uv.format = WGPUVertexFormat_Float32x2;
        uv.offset = 2 * sizeof(float);
        uv.shaderLocation = 1;

        WGPUVertexBufferLayout vertexBuffer{};
        vertexBuffer.arrayStride = 4 * sizeof(float);
        vertexBuffer.stepMode = WGPUVertexStepMode_Vertex;
        vertexBuffer.attributeCount = attributes.size();
        vertexBuffer.attributes = attributes.data();

        WGPUBlendState blendState = {};
        blendState.alpha.operation = WGPUBlendOperation_Add;
        blendState.alpha.srcFactor = WGPUBlendFactor_One;
        blendState.alpha.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
        blendState.color.operation = WGPUBlendOperation_Add;
        blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
        blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;

        WGPUColorTargetState colorTargetState{};
        colorTargetState.format = preferredFormat;
        colorTargetState.blend = &blendState;
        colorTargetState.writeMask = WGPUColorWriteMask_All;

        WGPUFragmentState fragmentState{};
        fragmentState.module = shaderModule;
        fragmentState.entryPoint = "vs";
        fragmentState.targetCount = 1;
        fragmentState.targets = &colorTargetState;

        WGPURenderPipelineDescriptor descriptor = {};
        descriptor.label = "DrawPicture";
        descriptor.vertex.module = shaderModule;
        descriptor.vertex.entryPoint = "fs";
        descriptor.vertex.bufferCount = 1;
        descriptor.vertex.buffers = &vertexBuffer;
        descriptor.primitive.topology = WGPUPrimitiveTopology_TriangleList;
        descriptor.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
        descriptor.primitive.frontFace = WGPUFrontFace_CW;
        descriptor.primitive.cullMode = WGPUCullMode_None;
        descriptor.depthStencil = nullptr;
        descriptor.multisample.count = 1;
        descriptor.multisample.mask = ~0u;
        descriptor.multisample.alphaToCoverageEnabled = false;
        descriptor.fragment = &fragmentState;

        pipeline = wgpuDeviceCreateRenderPipeline(device, &descriptor);
    }

    {
        constexpr float vertexData[] = {
            -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 1.0f,

            0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 0.0f,
        };
        WGPUBufferDescriptor descriptor{};
        descriptor.usage = WGPUBufferUsage_Vertex | WGPUBufferUsage_CopyDst;
        descriptor.size = sizeof(vertexData);
        vertexBuffer = wgpuDeviceCreateBuffer(device, &descriptor);
        wgpuQueueWriteBuffer(queue, vertexBuffer, 0, vertexData, sizeof(vertexData));
    }

    {

    }

}

int main()
{
    initWebGpu();

    emscripten_request_animation_frame_loop(render, nullptr);
    return 0;
}





