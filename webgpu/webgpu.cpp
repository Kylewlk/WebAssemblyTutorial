#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/fetch.h>
#include <webgpu/webgpu.h>
#include <iostream>
#include <format>
#include <thread>
#include <string>

#include "webgpu.h"

#include "webgpu_glue.cpp"

#define LOGI(...)                                           \
    do                                                      \
    {                                                       \
        std::cout << std::format(__VA_ARGS__) << std::endl; \
    } while (false)
// error
#define LOGE(...)                                           \
    do                                                      \
    {                                                       \
        std::cerr << std::format(__VA_ARGS__) << std::endl; \
    } while (false)
// warning
#define LOGW(...)                                           \
    do                                                      \
    {                                                       \
        std::cerr << std::format(__VA_ARGS__) << std::endl; \
    } while (false)

Context* Context::getInstance()
{
    static Context context;
    return &context;
}

const std::string canvasID = "canvas"; // <canvas> id
WGPUInstance instance{};
WGPUAdapter adapter{};
WGPUDevice device{};
WGPUQueue queue{};
WGPUSurface surface{};
WGPUTextureFormat preferredFormat = WGPUTextureFormat_RGBA8Unorm;

WGPURenderPipeline pipeline{};
WGPUBindGroupLayout bindGroupLayout{};
WGPUBindGroup bindGroup{};
WGPUSampler sampler{};
WGPUBuffer vertexBuffer{};
WGPUTexture texture{};
WGPUTextureView textureView{};

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
        },
        nullptr);
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
        },
        nullptr);
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
            std::cerr << "Device Uncaptured Error: " << errorType << ", " << message << std::endl; }, nullptr);

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

constexpr const char *shaderSource = R"(

struct VertexOutput
{
	@builtin(position) position: vec4f,
	@location(0) uv: vec2f,
}

@group(0) @binding(0) var mySampler: sampler;
@group(0) @binding(1) var myTexture: texture_2d<f32>;

@vertex fn vs(@location(0) pos : vec4f,
              @location(1) uv : vec2f
            ) -> VertexOutput {
    var output : VertexOutput;
    output.position = pos;
    output.uv = uv;
    return output;
}

@fragment fn fs(@location(0) uv: vec2f,) -> @location(0) vec4f {
    return textureSample(myTexture, mySampler, uv);
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
        WGPUTextureDescriptor textureDesc{};
        textureDesc.dimension = WGPUTextureDimension_2D;
        textureDesc.size = {4, 4, 1};
        textureDesc.mipLevelCount = 1;
        textureDesc.sampleCount = 1;
        textureDesc.format = WGPUTextureFormat_RGBA8Unorm;
        textureDesc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;
        textureDesc.viewFormatCount = 0;
        textureDesc.viewFormats = nullptr;
        texture = wgpuDeviceCreateTexture(device, &textureDesc);

        WGPUImageCopyTexture destination{};
        destination.texture = texture;
        destination.mipLevel = 0;
        destination.origin = {0, 0, 0};
        destination.aspect = WGPUTextureAspect_All;

        WGPUTextureDataLayout source;
        source.offset = 0;
        source.bytesPerRow = 4 * 4;
        source.rowsPerImage = 4;

        std::array<uint8_t, 4 * 4 * 4> imageData = {};
        imageData.fill(255);
        wgpuQueueWriteTexture( queue, &destination, imageData.data(), imageData.size(), &source, &textureDesc.size);
        textureView = wgpuTextureCreateView(texture, nullptr);

        WGPUSamplerDescriptor samplerDescriptor{};
        samplerDescriptor.addressModeU = WGPUAddressMode_Repeat; 
        samplerDescriptor.addressModeV = WGPUAddressMode_Repeat;
        samplerDescriptor.addressModeW = WGPUAddressMode_Repeat;
        samplerDescriptor.magFilter = WGPUFilterMode_Nearest;
        samplerDescriptor.minFilter = WGPUFilterMode_Nearest;
        samplerDescriptor.mipmapFilter = WGPUMipmapFilterMode_Nearest;
        samplerDescriptor.lodMinClamp = 0.0f;
        samplerDescriptor.lodMaxClamp = 1.0f;
        samplerDescriptor.compare = WGPUCompareFunction_Undefined;
        samplerDescriptor.maxAnisotropy = 1;
        sampler = wgpuDeviceCreateSampler(device, &samplerDescriptor);

        // group layout
        std::array<WGPUBindGroupLayoutEntry, 2> bindLayoutEntries{};
        auto &samplerLayoutEntry = bindLayoutEntries[0];
        samplerLayoutEntry.binding = 0;
        samplerLayoutEntry.visibility = WGPUShaderStage_Fragment;
        samplerLayoutEntry.sampler.type = WGPUSamplerBindingType_Filtering;
        auto &imageLayoutEntry = bindLayoutEntries[1];
        imageLayoutEntry.binding = 1;
        imageLayoutEntry.visibility = WGPUShaderStage_Fragment;
        imageLayoutEntry.texture.sampleType = WGPUTextureSampleType_Float;
        imageLayoutEntry.texture.viewDimension = WGPUTextureViewDimension_2D;
        WGPUBindGroupLayoutDescriptor bgLayoutDescriptor{};
        bgLayoutDescriptor.entryCount = bindLayoutEntries.size();
        bgLayoutDescriptor.entries = bindLayoutEntries.data();
        bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bgLayoutDescriptor);

        // Binding Group
        std::array<WGPUBindGroupEntry, 2> bindGroupEntries{};
        auto &samplerEntry = bindGroupEntries[0];
        samplerEntry.binding = 0;
        samplerEntry.sampler = sampler;
        auto &imageEntry = bindGroupEntries[1];
        imageEntry.binding = 1;
        imageEntry.textureView = textureView;

        WGPUBindGroupDescriptor bgDescriptor{};
        bgDescriptor.entryCount = bindGroupEntries.size();
        bgDescriptor.entries = bindGroupEntries.data();
        bgDescriptor.layout = bindGroupLayout;
        bindGroup = wgpuDeviceCreateBindGroup(device, &bgDescriptor);
    }

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
        fragmentState.entryPoint = "fs";
        fragmentState.targetCount = 1;
        fragmentState.targets = &colorTargetState;

        WGPURenderPipelineDescriptor descriptor = {};
        descriptor.label = "DrawPicture";
        descriptor.vertex.module = shaderModule;
        descriptor.vertex.entryPoint = "vs";
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

        WGPUPipelineLayoutDescriptor layoutDesc{};
        layoutDesc.bindGroupLayoutCount = 1;
        layoutDesc.bindGroupLayouts = &bindGroupLayout;
        descriptor.layout = wgpuDeviceCreatePipelineLayout(device, &layoutDesc);

        pipeline = wgpuDeviceCreateRenderPipeline(device, &descriptor);

        wgpuShaderModuleRelease(shaderModule);
        wgpuPipelineLayoutRelease(descriptor.layout);
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

    wgpuRenderPassEncoderSetPipeline(renderPass, pipeline);
    wgpuRenderPassEncoderSetBindGroup(renderPass, 0, bindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, vertexBuffer, 0, wgpuBufferGetSize(vertexBuffer));
    wgpuRenderPassEncoderDraw(renderPass, 6, 1, 0, 0);

    wgpuRenderPassEncoderEnd(renderPass);

    auto command = wgpuCommandEncoderFinish(commandEncoder, nullptr);
    wgpuQueueSubmit(queue, 1, &command);
    wgpuCommandBufferRelease(command);
    wgpuCommandEncoderRelease(commandEncoder);

    wgpuTextureViewRelease(renderView);

    // wgpuSurfacePresent(surface); // 浏览器内部会调用present，无需再调用
    return true; // 返回true继续回调
}

void downloadSucceeded(emscripten_fetch_t *fetch)
{
    printf("Finished downloading %llu bytes from URL %s.\n", fetch->numBytes, fetch->url);
    // The data is now available at fetch->data[0] through fetch->data[fetch->numBytes-1];
    emscripten_fetch_close(fetch); // Free data associated with the fetch.
}

void downloadFailed(emscripten_fetch_t *fetch)
{
    printf("Downloading %s failed, HTTP failure status code: %d.\n", fetch->url, fetch->status);
    emscripten_fetch_close(fetch); // Also free data on failure.
}

int main()
{
    initWebGpu();
    emscripten_sleep(10);
    initDraw();

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = downloadSucceeded;
    attr.onerror = downloadFailed;
    auto url = Context::getInstance()->getUrlString() + "/tasks.json";
    emscripten_fetch(&attr, url.c_str());

    emscripten_request_animation_frame_loop(render, nullptr);
    return 0;
}
