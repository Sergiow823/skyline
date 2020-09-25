// SPDX-License-Identifier: MPL-2.0
// Copyright © 2020 Skyline Team and Contributors (https://github.com/skyline-emu/)

#pragma once

#include "nvdevice.h"

namespace skyline::service::nvdrv::device {
    /**
     * @brief NvMap (/dev/nvmap) is used to map certain CPU memory as GPU memory (https://switchbrew.org/wiki/NV_services) (https://android.googlesource.com/kernel/tegra/+/refs/heads/android-tegra-flounder-3.10-marshmallow/include/linux/nvmap.h)
     */
    class NvMap : public NvDevice {
      public:
        /**
         * @brief NvMapObject is used to hold the state of held objects
         */
        struct NvMapObject {
            u32 id; //!< The ID of this object
            u32 size; //!< The size of this object
            u64 address{}; //!< The address of the allocation
            u32 flags{}; //!< The flag of the memory (0 = Read Only, 1 = Read-Write)
            u32 align{}; //!< The alignment of the allocation
            u32 heapMask{}; //!< This is set during Alloc and returned during Param
            u8 kind{}; //!< This is same as heapMask

            enum class Status {
                Created, //!< The object has been created but memory has not been allocated
                Allocated //!< The object has been allocated
            } status{Status::Created}; //!< This holds the status of the object

            /**
             * @param handle The ID of this object
             * @param size The size of the object in bytes
             */
            NvMapObject(u32 id, u32 size);
        };

        std::unordered_map<KHandle, std::shared_ptr<NvMapObject>> handleTable; //!< A mapping from a handle to it's corresponding NvMapObject
        KHandle handleIndex{1}; //!< This is used to keep track of the next handle to allocate
        u32 idIndex{1}; //!< This is used to keep track of the next ID to allocate

        NvMap(const DeviceState &state);

        /**
         * @brief This creates an NvMapObject and returns an handle to it (https://switchbrew.org/wiki/NV_services#NVMAP_IOC_CREATE)
         */
        NvStatus Create(IoctlType type, span<u8> buffer, span<u8> inlineBuffer);

        /**
         * @brief This returns the handle of an NvMapObject from it's ID (https://switchbrew.org/wiki/NV_services#NVMAP_IOC_FROM_ID)
         */
        NvStatus FromId(IoctlType type, span<u8> buffer, span<u8> inlineBuffer);

        /**
         * @brief This allocates memory for an NvMapObject (https://switchbrew.org/wiki/NV_services#NVMAP_IOC_ALLOC)
         */
        NvStatus Alloc(IoctlType type, span<u8> buffer, span<u8> inlineBuffer);

        /**
         * @brief This frees previously allocated memory (https://switchbrew.org/wiki/NV_services#NVMAP_IOC_FREE)
         */
        NvStatus Free(IoctlType type, span<u8> buffer, span<u8> inlineBuffer);

        /**
         * @brief This returns a particular parameter from an NvMapObject (https://switchbrew.org/wiki/NV_services#NVMAP_IOC_PARAM)
         */
        NvStatus Param(IoctlType type, span<u8> buffer, span<u8> inlineBuffer);

        /**
         * @brief This returns the ID of an NvMapObject from it's handle (https://switchbrew.org/wiki/NV_services#NVMAP_IOC_GET_ID)
         */
        NvStatus GetId(IoctlType type, span<u8> buffer, span<u8> inlineBuffer);

        NVDEVICE_DECL(
            NVFUNC(0x0101, NvMap, Create),
            NVFUNC(0x0103, NvMap, FromId),
            NVFUNC(0x0104, NvMap, Alloc),
            NVFUNC(0x0105, NvMap, Free),
            NVFUNC(0x0109, NvMap, Param),
            NVFUNC(0x010E, NvMap, GetId)
        )
    };
}
