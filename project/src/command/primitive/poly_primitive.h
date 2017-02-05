/*******************************************************************************
PANDORAGS project - PS1 GPU driver
------------------------------------------------------------------------
Author  :     Romain Vinders
License :     GPLv2
------------------------------------------------------------------------
Description : polygon primitive (triangle / quad)
*******************************************************************************/
#pragma once

#include "primitive_common.h"
#pragma pack(push, 4)

// quad to triangle
#define POLY0_vertex0 vertex0
#define POLY0_vertex1 vertex1
#define POLY0_vertex2 vertex2
#define POLY1_vertex0 vertex2
#define POLY1_vertex1 vertex1
#define POLY1_vertex2 vertex3


/// @namespace command
/// GPU commands management
namespace command
{
    /// @namespace command.primitive
    /// Drawing primitive management
    namespace primitive
    {
        // -- primitive units - flat polygons -- -------------------------------

        /// @struct poly_f3_t
        /// @brief Flat-shaded triangle
        struct poly_f3_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            rgb24_t color;       ///< Primitive ID (pad) + triangle color (RGB)
            vertex_f1_t vertex0; ///< Vertex coordinates
            vertex_f1_t vertex1; ///< Vertex coordinates
            vertex_f1_t vertex2; ///< Vertex coordinates
            // rendering information
            inline bool isOpaque()  { return ((color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((color.raw & 0x1000000) == 0uL); }

            static inline size_t size() { return 4; } ///< Length (32-bit blocks)
        };


        /// @struct poly_f4_t
        /// @brief Flat-shaded quad
        struct poly_f4_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            rgb24_t color;       ///< Primitive ID (pad) + quad color (RGB)
            vertex_f1_t vertex0; ///< Vertex coordinates (top-left)
            vertex_f1_t vertex1; ///< Vertex coordinates (top-right)
            vertex_f1_t vertex2; ///< Vertex coordinates (bottom-left)
            vertex_f1_t vertex3; ///< Vertex coordinates (bottom-right)
            // rendering information
            inline bool isOpaque()  { return ((color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((color.raw & 0x1000000) == 0uL); }

            static inline size_t size() { return 5; } ///< Length (32-bit blocks)
        };


        /// @struct poly_ft3_t
        /// @brief Flat-shaded texture-mapped triangle
        struct poly_ft3_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            rgb24_t color;        ///< Primitive ID (pad) + triangle color (RGB)
            vertex_ft1_t vertex0; ///< Vertex coordinates/texture (CLUT)
            vertex_ft1_t vertex1; ///< Vertex coordinates/texture (texpage)
            vertex_ft1_t vertex2; ///< Vertex coordinates
            // rendering information
            inline bool isOpaque()  { return ((color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((color.raw & 0x1000000) == 0uL); }
            // texture information
            inline command::cmd_block_t clutX()    { return vertex0.texture.clutX(); }
            inline command::cmd_block_t clutY()    { return vertex0.texture.clutY(); }
            inline command::cmd_block_t texpageX() { return vertex1.texture.texpageX(); }
            inline command::cmd_block_t texpageY() { return vertex1.texture.texpageY(); }

            static inline size_t size() { return 7; } ///< Length (32-bit blocks)
        };


        /// @struct poly_ft4_t
        /// @brief Flat-shaded texture-mapped quad
        struct poly_ft4_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            rgb24_t color;        ///< Primitive ID (pad) + quad color (RGB)
            vertex_ft1_t vertex0; ///< Vertex coordinates/texture (CLUT)
            vertex_ft1_t vertex1; ///< Vertex coordinates/texture (texpage)
            vertex_ft1_t vertex2; ///< Vertex coordinates
            vertex_ft1_t vertex3; ///< Vertex coordinates
            // rendering information
            inline bool isOpaque()  { return ((color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((color.raw & 0x1000000) == 0uL); }
            // texture information
            inline command::cmd_block_t clutX()    { return vertex0.texture.clutX(); }
            inline command::cmd_block_t clutY()    { return vertex0.texture.clutY(); }
            inline command::cmd_block_t texpageX() { return vertex1.texture.texpageX(); }
            inline command::cmd_block_t texpageY() { return vertex1.texture.texpageY(); }

            static inline size_t size() { return 9; } ///< Length (32-bit blocks)
        };


        // -- primitive units - shaded polygons -- -----------------------------

        /// @struct poly_g3_t
        /// @brief Gouraud-shaded triangle
        struct poly_g3_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            vertex_g1_t vertex0; ///< Primitive ID + vertex color/coordinates
            vertex_g1_t vertex1; ///< Vertex color/coordinates
            vertex_g1_t vertex2; ///< Vertex color/coordinates
            // rendering information
            inline bool isOpaque()  { return ((vertex0.color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((vertex0.color.raw & 0x1000000) == 0uL); }

            static inline size_t size() { return 6; } ///< Length (32-bit blocks)
        };


        /// @struct poly_g4_t
        /// @brief Gouraud-shaded quad
        struct poly_g4_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            vertex_g1_t vertex0; ///< Primitive ID + vertex color/coordinates
            vertex_g1_t vertex1; ///< Vertex color/coordinates
            vertex_g1_t vertex2; ///< Vertex color/coordinates
            vertex_g1_t vertex3; ///< Vertex color/coordinates
            // rendering information
            inline bool isOpaque()  { return ((vertex0.color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((vertex0.color.raw & 0x1000000) == 0uL); }

            static inline size_t size() { return 8; } ///< Length (32-bit blocks)
        };


        /// @struct poly_gt3_t
        /// @brief Gouraud-shaded texture-mapped triangle
        struct poly_gt3_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            vertex_gt1_t vertex0; ///< Primitive ID + vertex color/coordinates/texture (CLUT)
            vertex_gt1_t vertex1; ///< Vertex color/coordinates/texture (texpage)
            vertex_gt1_t vertex2; ///< Vertex color/coordinates
            // rendering information
            inline bool isOpaque()  { return ((vertex0.color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((vertex0.color.raw & 0x1000000) == 0uL); }
            // texture information
            inline command::cmd_block_t clutX()    { return vertex0.texture.clutX(); }
            inline command::cmd_block_t clutY()    { return vertex0.texture.clutY(); }
            inline command::cmd_block_t texpageX() { return vertex1.texture.texpageX(); }
            inline command::cmd_block_t texpageY() { return vertex1.texture.texpageY(); }

            static inline size_t size() { return 9; } ///< Length (32-bit blocks)
        };


        /// @struct poly_gt4_t
        /// @brief Gouraud-shaded texture-mapped quad
        struct poly_gt4_t
        {
            /// @brief Process primitive
            /// @param[in] pData  Raw primitive data pointer
            static void process(command::cmd_block_t* pData);

            vertex_gt1_t vertex0; ///< Primitive ID + vertex color/coordinates/texture (CLUT)
            vertex_gt1_t vertex1; ///< Vertex color/coordinates/texture (texpage)
            vertex_gt1_t vertex2; ///< Vertex color/coordinates
            vertex_gt1_t vertex3; ///< Vertex color/coordinates
            // rendering information
            inline bool isOpaque()  { return ((vertex0.color.raw & 0x2000000) == 0uL); }
            inline bool isBlended() { return ((vertex0.color.raw & 0x1000000) == 0uL); }
            // texture information
            inline command::cmd_block_t clutX()    { return vertex0.texture.clutX(); }
            inline command::cmd_block_t clutY()    { return vertex0.texture.clutY(); }
            inline command::cmd_block_t texpageX() { return vertex1.texture.texpageX(); }
            inline command::cmd_block_t texpageY() { return vertex1.texture.texpageY(); }

            static inline size_t size() { return 12; } ///< Length (32-bit blocks)
        };


        /*
        Size Restriction: The maximum distance between two vertices is 1023 horizontally, and 511 vertically. 
          Polygons and lines that are exceeding that dimensions are NOT rendered.
          If portions of the polygon/line/rectangle are located outside of the drawing area, then the hardware renders only the portion that is inside of the drawing area. 

        Caution: For untextured graphics, 8bit RGB values of FFh are brightest.
          However, for texture blending, 8bit values of 80h are brightest (values 81h..FFh make textures brighter (up to twice as bright) as than they were originially stored in memory.
          Of course the results can't exceed the maximum brightness: the 5bit values written to the framebuffer are saturated to max 1Fh.

        Texpage Attribute
          Bits 9 (dithering) and 10 (drawing allowed) must be specified through the texture page command (0xE1 -> primitive_attr.h).

        Clut Attribute (Color Lookup Table)
          Specifies the location of the CLUT data within VRAM (only for 4bit/8bit textures).
        */
    }
}
#pragma pack(pop)
