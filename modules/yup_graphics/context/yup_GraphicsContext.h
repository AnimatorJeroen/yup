/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace yup
{

class LowLevelRenderContext;

//==============================================================================
/**
 * @brief Encapsulates a graphics context that abstracts rendering operations across various APIs.
 *
 * This class serves as a base for implementing specific graphics context functionalities, such as rendering and resource management,
 * across different graphics APIs like OpenGL, Direct3D, Metal, and Dawn. It offers a standardized interface for operations
 * common to all graphics APIs.
 */
class JUCE_API GraphicsContext
{
public:
    //==============================================================================
    /**
     * @brief Configuration options for creating a graphics context.
     */
    struct Options
    {
        /**
         * @brief Default constructor, initializes the options with default values.
         */
        constexpr Options() noexcept = default;

        bool retinaDisplay                 = true; ///< Whether the context supports Retina or high-DPI displays.
        bool readableFramebuffer           = true; ///< Allows the framebuffer to be readable.
        bool synchronousShaderCompilations = false; ///< Controls whether shader compilations are done synchronously.
        bool enableReadPixels              = false; ///< Enables reading pixels directly from the framebuffer.
        bool disableRasterOrdering         = false; ///< Disables specific raster ordering features for performance.
    };

    /**
     * @brief Enumerates supported graphics APIs.
     */
    enum Api
    {
        OpenGL,  ///< Specifies the use of OpenGL for rendering.
        Direct3D,///< Specifies the use of Direct3D for rendering.
        Metal,   ///< Specifies the use of Metal for rendering.
        Dawn     ///< Specifies the use of Dawn, a Vulkan-like API.
    };

    //==============================================================================
    /**
     * @brief Default constructor.
     */
    GraphicsContext() noexcept = default;

    /**
     * @brief Destructor.
     */
    virtual ~GraphicsContext() = default;

    //==============================================================================
    /**
     * @brief Copy and move constructors and assignment operators.
     */
    GraphicsContext (const GraphicsContext& other) noexcept = delete;
    GraphicsContext (GraphicsContext&& other) noexcept = default;
    GraphicsContext& operator=(const GraphicsContext& other) noexcept = delete;
    GraphicsContext& operator=(GraphicsContext&& other) noexcept = default;

    //==============================================================================
    /**
     * @brief Returns the DPI scale associated with a native handle.
     *
     * @param nativeHandle A platform-specific handle to the native window or screen.
     *
     * @return The DPI scale factor.
     */
    virtual float dpiScale (void* nativeHandle) const = 0;

    //==============================================================================
    /**
     * @brief Provides access to the associated factory for resource creation.
     *
     * @return Pointer to a rive::Factory object.
     */
    virtual rive::Factory* factory() = 0;

    /**
     * @brief Gets the PLS render context, if available.
     *
     * @return Pointer to a rive::pls::PLSRenderContext, or nullptr if not available.
     */
    virtual rive::pls::PLSRenderContext* plsContextOrNull() = 0;

    /**
     * @brief Gets the PLS render target, if available.
     *
     * @return Pointer to a rive::pls::PLSRenderTarget, or nullptr if not available.
     */
    virtual rive::pls::PLSRenderTarget* plsRenderTargetOrNull() = 0;

    /**
     * @brief Creates a renderer suitable for the specified dimensions.
     *
     * @param width The width of the render area.
     * @param height The height of the render area.
     *
     * @return A unique pointer to a rive::Renderer object.
     */
    virtual std::unique_ptr<rive::Renderer> makeRenderer (int width, int height) = 0;

    //==============================================================================
    /**
     * @brief Handles changes in the size of the rendering surface.
     *
     * @param nativeHandle A platform-specific handle to the native window or screen.
     * @param width The new width of the surface.
     * @param height The new height of the surface.
     * @param sampleCount The number of samples per pixel, for anti-aliasing.
     */
    virtual void onSizeChanged (void* nativeHandle, int width, int height, uint32_t sampleCount) = 0;

    //==============================================================================
    /**
     * @brief Begins a rendering frame.
     *
     * @param descriptor The frame descriptor that contains frame-specific data.
     */
    virtual void begin (const rive::pls::PLSRenderContext::FrameDescriptor&) = 0;

    /**
     * @brief Ends a rendering frame.
     *
     * @param nativeHandle A platform-specific handle to the native window or screen.
     */
    virtual void end (void* nativeHandle) = 0;

    /**
     * @brief Performs periodic operations, potentially related to animation or state updates.
     */
    virtual void tick() {}

    //==============================================================================
    /**
     * @brief Static factory method to create a graphics context with specific options.
     *
     * @param options Configuration options for the graphics context.
     * @return A unique pointer to a GraphicsContext, configured according to the specified options.
     */
    static std::unique_ptr<GraphicsContext> createContext (Options options);

    /**
     * @brief Static factory method to create a graphics context using a specific graphics API.
     *
     * @param graphicsApi The graphics API to use.
     * @param options Configuration options for the graphics context.
     * @return A unique pointer to a GraphicsContext, using the specified graphics API and configured according to the options.
     */
    static std::unique_ptr<GraphicsContext> createContext (Api graphicsApi, Options options);
};

} // namespace yup
