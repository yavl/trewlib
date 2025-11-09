#include "TextRenderer.hpp"
#include <trew/Camera.hpp>
#include <trew/Logger.hpp>

// Shaders
#include "testgputext/shaders/shader.vert.spv.h"
#include "testgputext/shaders/shader.frag.spv.h"
#include "testgputext/shaders/shader-sdf.frag.spv.h"
#include "testgputext/shaders/shader.vert.dxil.h"
#include "testgputext/shaders/shader.frag.dxil.h"
#include "testgputext/shaders/shader-sdf.frag.dxil.h"
#include "testgputext/shaders/shader.vert.msl.h"
#include "testgputext/shaders/shader.frag.msl.h"
#include "testgputext/shaders/shader-sdf.frag.msl.h"

using namespace trew;

constexpr auto LOGTAG = "TextRenderer";

SDL_GPUShader* TextRenderer::loadShader(
    SDL_GPUDevice* device,
    TextShader shader,
    Uint32 sampler_count,
    Uint32 uniform_buffer_count,
    Uint32 storage_buffer_count,
    Uint32 storage_texture_count)
{
    SDL_GPUShaderCreateInfo createinfo{
        .num_samplers = sampler_count,
        .num_storage_textures = storage_texture_count,
        .num_storage_buffers = storage_buffer_count,
        .num_uniform_buffers = uniform_buffer_count,
        .props = 0
    };

    SDL_GPUShaderFormat format = SDL_GetGPUShaderFormats(device);
    if (format & SDL_GPU_SHADERFORMAT_DXIL) {
        createinfo.format = SDL_GPU_SHADERFORMAT_DXIL;
        switch (shader) {
        case VertexShader:
            createinfo.code = shader_vert_dxil;
            createinfo.code_size = shader_vert_dxil_len;
            createinfo.entrypoint = "VSMain";
            break;
        case PixelShader:
            createinfo.code = shader_frag_dxil;
            createinfo.code_size = shader_frag_dxil_len;
            createinfo.entrypoint = "PSMain";
            break;
        case PixelShader_SDF:
            createinfo.code = shader_sdf_frag_dxil;
            createinfo.code_size = shader_sdf_frag_dxil_len;
            createinfo.entrypoint = "PSMain";
            break;
        }
    }
    else if (format & SDL_GPU_SHADERFORMAT_MSL) {
        createinfo.format = SDL_GPU_SHADERFORMAT_MSL;
        switch (shader) {
        case VertexShader:
            createinfo.code = shader_vert_msl;
            createinfo.code_size = shader_vert_msl_len;
            createinfo.entrypoint = "main0";
            break;
        case PixelShader:
            createinfo.code = shader_frag_msl;
            createinfo.code_size = shader_frag_msl_len;
            createinfo.entrypoint = "main0";
            break;
        case PixelShader_SDF:
            createinfo.code = shader_sdf_frag_msl;
            createinfo.code_size = shader_sdf_frag_msl_len;
            createinfo.entrypoint = "main0";
            break;
        }
    }
    else {
        createinfo.format = SDL_GPU_SHADERFORMAT_SPIRV;
        switch (shader) {
        case VertexShader:
            createinfo.code = shader_vert_spv;
            createinfo.code_size = shader_vert_spv_len;
            createinfo.entrypoint = "main";
            break;
        case PixelShader:
            createinfo.code = shader_frag_spv;
            createinfo.code_size = shader_frag_spv_len;
            createinfo.entrypoint = "main";
            break;
        case PixelShader_SDF:
            createinfo.code = shader_sdf_frag_spv;
            createinfo.code_size = shader_sdf_frag_spv_len;
            createinfo.entrypoint = "main";
            break;
        }
    }

    if (shader == VertexShader) {
        createinfo.stage = SDL_GPU_SHADERSTAGE_VERTEX;
    }
    else {
        createinfo.stage = SDL_GPU_SHADERSTAGE_FRAGMENT;
    }
    return SDL_CreateGPUShader(device, &createinfo);
}

void TextRenderer::queue_text_sequence(GeometryData* geometry_data, TTF_GPUAtlasDrawSequence* sequence, SDL_FColor* colour)
{
    for (int i = 0; i < sequence->num_vertices; i++) {
        Vertex vert;
        const SDL_FPoint pos = sequence->xy[i];
        vert.pos = { pos.x, pos.y, 0.0f };
        vert.colour = *colour;
        vert.uv = sequence->uv[i];

        geometry_data->vertices[geometry_data->vertex_count + i] = vert;
    }

    SDL_memcpy(geometry_data->indices + geometry_data->index_count, sequence->indices, sequence->num_indices * sizeof(int));

    geometry_data->vertex_count += sequence->num_vertices;
    geometry_data->index_count += sequence->num_indices;
}

void TextRenderer::queue_text(GeometryData* geometry_data, TTF_GPUAtlasDrawSequence* sequence, SDL_FColor* colour)
{
    for (; sequence; sequence = sequence->next) {
        queue_text_sequence(geometry_data, sequence, colour);
    }
}

void TextRenderer::set_geometry_data(TextContext* context, GeometryData* geometry_data)
{
    Vertex* transfer_data = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(context->device, context->transfer_buffer, false));

    SDL_memcpy(transfer_data, geometry_data->vertices, sizeof(Vertex) * geometry_data->vertex_count);
    SDL_memcpy(transfer_data + MAX_VERTEX_COUNT, geometry_data->indices, sizeof(int) * geometry_data->index_count);

    SDL_UnmapGPUTransferBuffer(context->device, context->transfer_buffer);
}

void TextRenderer::transfer_data(TextContext* context, GeometryData* geometry_data)
{
    SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(context->commandBuffer);

    // Store the temporary objects in named variables
    SDL_GPUTransferBufferLocation transfer_source1{ context->transfer_buffer, 0 };
    SDL_GPUBufferRegion buffer_region_vertex{
        context->vertex_buffer,
        0,
        static_cast<unsigned int>(sizeof(Vertex) * geometry_data->vertex_count)
    };

    SDL_UploadToGPUBuffer(copy_pass, &transfer_source1, &buffer_region_vertex, false);

    // Create a new set of variables for the second call
    SDL_GPUTransferBufferLocation transfer_source2{
        context->transfer_buffer,
        static_cast<unsigned int>(sizeof(Vertex) * MAX_VERTEX_COUNT)
    };
    SDL_GPUBufferRegion buffer_region_index{
        context->index_buffer,
        0,
        static_cast<unsigned int>(sizeof(int) * geometry_data->index_count)
    };

    SDL_UploadToGPUBuffer(copy_pass, &transfer_source2, &buffer_region_index, false);

    SDL_EndGPUCopyPass(copy_pass);
}

void TextRenderer::draw(TextContext* context, glm::mat4 matrices[], int num_matrices, TTF_GPUAtlasDrawSequence* draw_sequence)
{
    if (context->swapchainTexture != NULL) {
        // Initialize the struct using uniform initialization
        SDL_GPUColorTargetInfo colour_target_info{};
        colour_target_info.texture = context->swapchainTexture;

        SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(context->commandBuffer, &colour_target_info, 1, NULL);

        SDL_BindGPUGraphicsPipeline(render_pass, context->pipeline);

        // Use named variable for the first buffer binding
        SDL_GPUBufferBinding vertex_binding{ context->vertex_buffer, 0 };
        SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_binding, 1);

        // Use named variable for the index buffer binding
        SDL_GPUBufferBinding index_binding{ context->index_buffer, 0 };
        SDL_BindGPUIndexBuffer(render_pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

        SDL_PushGPUVertexUniformData(context->commandBuffer, 0, matrices, sizeof(glm::mat4) * num_matrices);

        int index_offset = 0, vertex_offset = 0;
        for (TTF_GPUAtlasDrawSequence* seq = draw_sequence; seq != NULL; seq = seq->next) {
            // Use named variable for the fragment sampler binding
            SDL_GPUTextureSamplerBinding sampler_binding{ seq->atlas_texture, context->sampler };
            SDL_BindGPUFragmentSamplers(render_pass, 0, &sampler_binding, 1);

            SDL_DrawGPUIndexedPrimitives(render_pass, seq->num_indices, 1, index_offset, vertex_offset, 0);

            index_offset += seq->num_indices;
            vertex_offset += seq->num_vertices;
        }
        SDL_EndGPURenderPass(render_pass);
    }
}

TTF_Font* TextRenderer::createFont(int size, bool useSDF) {
    auto path = "assets/fonts/Ubuntu-Regular.ttf";
    TTF_Font* font = TTF_OpenFont(path, size);
    logDebug(LOGTAG, fmt::format("SDF {} for {} with size {}", useSDF ? "enabled" : "disabled", path, size));
    TTF_SetFontSDF(font, useSDF);
    TTF_SetFontWrapAlignment(font, TTF_HORIZONTAL_ALIGN_CENTER);
    return font;
}

TTF_Font* TextRenderer::fontOfSize(int size) const {
    TTF_Font* font = nullptr;
    if (auto findit = fonts.find(size); findit != fonts.end()) {
        font = findit->second;
    }
    assert(font != nullptr && "wrong font size");
    if (font == nullptr) {
        font = fontDefault;
    }
    return font;
}

void free_context(TextContext* context)
{
}

TextRenderer::TextRenderer(SDL_GPUDevice* device, SDL_Window* window, Camera* cam): cam(cam) {
    bool useSDF = true;

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    context.window = window;
    context.device = device;

    SDL_GPUShader* vertex_shader = loadShader(context.device, VertexShader, 0, 1, 0, 0);
    SDL_GPUShader* fragment_shader = loadShader(context.device, useSDF ? PixelShader_SDF : PixelShader, 1, 0, 0, 0);

    // Create the nested structures as named variables first
    SDL_GPUColorTargetBlendState blend_state = {
        .src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
        .color_blend_op = SDL_GPU_BLENDOP_ADD,
        .src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
        .dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_DST_ALPHA,
        .alpha_blend_op = SDL_GPU_BLENDOP_ADD,
        .color_write_mask = 0xF,
        .enable_blend = true
    };

    SDL_GPUColorTargetDescription color_target_descriptions[] = { {
        SDL_GetGPUSwapchainTextureFormat(context.device, context.window),
        blend_state
    } };

    SDL_GPUVertexAttribute vertex_attributes[] = {
        {
            0, // location
            0, // buffer_slot
            SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
            0  // offset
        },
        {
            1, // location
            0, // buffer_slot
            SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4,
            static_cast<unsigned int>(sizeof(float) * 3) // offset
        },
        {
            2, // location
            0, // buffer_slot
            SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
            static_cast<unsigned int>(sizeof(float) * 7) // offset
        }
    };

    SDL_GPUVertexBufferDescription vertex_buffer_descriptions[] = { {
        0, // slot
        sizeof(Vertex),
        SDL_GPU_VERTEXINPUTRATE_VERTEX,
        0 // instance_step_rate
    } };

    // Correct the nested SDL_GPUVertexInputState aggregate initialization.
    SDL_GPUVertexInputState vertex_input_state = {
        // member order: vertex_buffer_descriptions, num_vertex_buffers, vertex_attributes, num_vertex_attributes
        vertex_buffer_descriptions,
        1,
        vertex_attributes,
        3
    };

    // Assuming default values for other states if not specified.
    SDL_GPURasterizerState rasterizer_state = {};
    SDL_GPUMultisampleState multisample_state = {};
    SDL_GPUDepthStencilState depth_stencil_state = {};

    // Correct the nested SDL_GPUGraphicsPipelineTargetInfo aggregate initialization.
    SDL_GPUGraphicsPipelineTargetInfo target_info = {
        // member order: color_target_descriptions, num_color_targets, depth_stencil_format, has_depth_stencil_target
        color_target_descriptions,
        1,
        SDL_GPU_TEXTUREFORMAT_INVALID,
        false
    };

    // Now initialize the main struct in the correct order using the named variables.
    SDL_GPUGraphicsPipelineCreateInfo pipeline_create_info = {
        vertex_shader,
        fragment_shader,
        vertex_input_state,
        SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
        rasterizer_state,
        multisample_state,
        depth_stencil_state,
        target_info
    };

    context.pipeline = SDL_CreateGPUGraphicsPipeline(context.device, &pipeline_create_info);

    SDL_ReleaseGPUShader(context.device, vertex_shader);
    SDL_ReleaseGPUShader(context.device, fragment_shader);

    SDL_GPUBufferCreateInfo vbf_info = {
        .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
        .size = sizeof(Vertex) * MAX_VERTEX_COUNT
    };
    context.vertex_buffer = SDL_CreateGPUBuffer(context.device, &vbf_info);

    SDL_GPUBufferCreateInfo ibf_info = {
        .usage = SDL_GPU_BUFFERUSAGE_INDEX,
        .size = sizeof(int) * MAX_INDEX_COUNT
    };
    context.index_buffer = SDL_CreateGPUBuffer(context.device, &ibf_info);

    SDL_GPUTransferBufferCreateInfo tbf_info = {
        .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
        .size = (sizeof(Vertex) * MAX_VERTEX_COUNT) + (sizeof(int) * MAX_INDEX_COUNT)
    };
    context.transfer_buffer = SDL_CreateGPUTransferBuffer(context.device, &tbf_info);

    SDL_GPUSamplerCreateInfo sampler_info = {
        .min_filter = SDL_GPU_FILTER_LINEAR,
        .mag_filter = SDL_GPU_FILTER_LINEAR,
        .mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
        .address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
        .address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE
    };
    context.sampler = SDL_CreateGPUSampler(context.device, &sampler_info);

    geometry_data.vertices = static_cast<Vertex*>(SDL_calloc(MAX_VERTEX_COUNT, sizeof(Vertex)));
    geometry_data.indices = static_cast<int*>(SDL_calloc(MAX_INDEX_COUNT, sizeof(int)));

    TTF_Init();

    fonts[50] = createFont(50, useSDF);
    fonts[100] = createFont(100, useSDF);

    fontDefault = fonts[50];

    engine = TTF_CreateGPUTextEngine(context.device);
    text = TTF_CreateText(engine, fontDefault, "example", 0);
}

TextRenderer::~TextRenderer() {

    SDL_free(geometry_data.vertices);
    SDL_free(geometry_data.indices);
    TTF_DestroyText(text);
    TTF_DestroyGPUTextEngine(engine);
    for (const auto& pair : fonts) {
        TTF_CloseFont(pair.second);
    }
    TTF_Quit();
    free_context(&context);

    SDL_ReleaseGPUTransferBuffer(context.device, context.transfer_buffer);
    SDL_ReleaseGPUSampler(context.device, context.sampler);
    SDL_ReleaseGPUBuffer(context.device, context.vertex_buffer);
    SDL_ReleaseGPUBuffer(context.device, context.index_buffer);
    SDL_ReleaseGPUGraphicsPipeline(context.device, context.pipeline);
    SDL_ReleaseWindowFromGPUDevice(context.device, context.window);
    SDL_DestroyGPUDevice(context.device);
    SDL_DestroyWindow(context.window);
}

void TextRenderer::drawText(const char* str, float x, float y, FontSize fontSize, float rotation) {
    TTF_SetTextFont(text, fontOfSize(static_cast<int>(fontSize)));
    TTF_SetTextString(text, str, 0);

    int textWidth, textHeight;
    TTF_GetTextSize(text, &textWidth, &textHeight);

    auto matrix = glm::translate(glm::mat4(1.f), glm::vec3(x / 2.f, y / 2.f, 0.f));
    matrix = glm::rotate(matrix, glm::radians(rotation / 2.f), glm::vec3(0.f, 0.f, -1.f));
    matrix = glm::translate(matrix, { -textWidth / 4.f, textHeight / 4.f, 0.0f });

    glm::mat4 matrices[2] = {
        cam->projection * cam->view * matrix,
        glm::mat4(1.f)
    };
    matrices[1] = matrix;

    // Get the text data and queue the text in a buffer for drawing later
    TTF_GPUAtlasDrawSequence* sequence = TTF_GetGPUTextDrawData(text);
    SDL_FColor textColor = { 1.0f, 1.0f, 0.0f, 1.0f };
    queue_text(&geometry_data, sequence, &textColor);

    set_geometry_data(&context, &geometry_data);

    transfer_data(&context, &geometry_data);
    draw(&context, matrices, 2, sequence);

    geometry_data.vertex_count = 0;
    geometry_data.index_count = 0;
}