#pragma once
#include <memory>
#include "TeaPacket/Endianness/EndianSwapping.hpp"

namespace TeaPacket::Graphics
{
    struct UniformBufferParameters;
    struct PlatformUniformBuffer;

    /// Represents a block of GPU data bound to a Uniform slot.
    class UniformBuffer
    {
    private:
        size_t size;
    public:

        /// A pointer to the implementation-defined data this buffer uses.
        std::unique_ptr<PlatformUniformBuffer> platformBuffer;

        /// Gets the size, in bytes, of the buffer.
        [[nodiscard]] constexpr decltype(size) GetSize() const { return size; }

        /// Creates a buffer from specified parameters.
        explicit UniformBuffer(const UniformBufferParameters& parameters);
        UniformBuffer(const UniformBuffer& other);
        UniformBuffer(UniformBuffer&& other) noexcept;
        ~UniformBuffer();

        /// @brief Updates the data in this Uniform buffer.
        /// @warning DO NOT UPDATE THE DATA IN THIS BUFFER WHILE A DRAW CALL IS BEING MADE! This may cause issues on certain platforms.
        void SendData(const void* data);
        /// Binds this buffer to a buffer slot.
        /// @param index The index of the buffer slot to be bound to.
        void SetActive(size_t index);

    public:
        /// @brief Whether the data inside a UniformBuffer needs to be Endian Swapped when sending the data over.
        /// @details Certain platforms (Wii U) require the data within a Uniform Buffer to be endian swapped before being sent over.
        /// The user must do this swapping themselves, as UniformBuffers know nothing about the data within them.
        static const bool ShouldUBBeEndianSwapped;

        /// Endian-Swaps a value if UniformBuffers require data to be Endian-Swapped.
        /// Use this instead of ShouldUBBeEndianSwapped where possible.
        template<typename T>
        static void SwapIfNeeded(T& val)
        {
            if (ShouldUBBeEndianSwapped)
            {
                val = Endianness::Swap(val);
            }
        }
    };
}
