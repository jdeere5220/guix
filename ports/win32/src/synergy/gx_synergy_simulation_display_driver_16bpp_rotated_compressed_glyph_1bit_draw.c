/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Synergy Simulation Display Management (Display)                     */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE


/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_system.h"
#include "gx_utility.h"
#include "gx_display.h"
#include "gx_synergy_simulation_display_driver.h"

#define DRAW_PIXEL if (alpha & mask) \
    {                                \
        *put = text_color;           \
    }                                \
    put++;                           \
    mask = (unsigned char)(mask << 1);

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_synergy_simulation_display_driver_16bpp_rotated_compressed_     */
/*                                                        glyph_1bit_draw */
/*                                                                        */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This function draws compressed monochrome font on 16bpp canvas,     */
/*    clipped to one viewport.                                            */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Draw context                  */
/*    draw_area                             The region bound by the       */
/*                                            rectangle where the glyph   */
/*                                            is drawn                    */
/*    map_offset                            X,Y offset into the glyph map */
/*    glyph                                 Pointer to the glyph          */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX internal code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Kenneth Maxwell          Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _gx_synergy_simulation_display_driver_16bpp_rotated_compressed_glyph_1bit_draw(GX_DRAW_CONTEXT *context, GX_RECTANGLE *draw_area,
                                                                                    GX_POINT *map_offset, GX_CONST GX_GLYPH *glyph)
{
GX_UBYTE            *glyph_data;
INT                  row;
INT                  col;
INT                  start_x;
USHORT               text_color;
GX_UBYTE             alpha_value;
GX_UBYTE             alpha;
GX_UBYTE             mask;
GX_UBYTE             count;
GX_UBYTE             index;
GX_COMPRESSED_GLYPH *compressed_glyph;
GX_RECTANGLE         rotated_draw_area;
GX_POINT             rotated_map_offset;

    compressed_glyph = (GX_COMPRESSED_GLYPH *)glyph;

    if (compressed_glyph -> gx_glyph_map_size & 0x8000)
    {
        if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
        {
            rotated_draw_area.gx_rectangle_left = draw_area -> gx_rectangle_top;
            rotated_draw_area.gx_rectangle_right = draw_area -> gx_rectangle_bottom;
            rotated_draw_area.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_right - 1);
            rotated_draw_area.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - draw_area -> gx_rectangle_left - 1);

            rotated_map_offset.gx_point_x = map_offset -> gx_point_y;
            rotated_map_offset.gx_point_y = (GX_VALUE)(glyph -> gx_glyph_width - map_offset -> gx_point_x -
                                                       (GX_VALUE)(draw_area -> gx_rectangle_right - draw_area -> gx_rectangle_left + 1));
        }
        else
        {
            rotated_draw_area.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_bottom - 1);
            rotated_draw_area.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - draw_area -> gx_rectangle_top - 1);
            rotated_draw_area.gx_rectangle_top = draw_area -> gx_rectangle_left;
            rotated_draw_area.gx_rectangle_bottom = draw_area -> gx_rectangle_right;

            rotated_map_offset.gx_point_x = (GX_VALUE)(glyph -> gx_glyph_height - map_offset -> gx_point_y -
                                                       (GX_VALUE)(draw_area -> gx_rectangle_bottom - draw_area -> gx_rectangle_top + 1));
            rotated_map_offset.gx_point_y = map_offset -> gx_point_x;
        }

        text_color = (USHORT)context -> gx_draw_context_brush.gx_brush_line_color;

        glyph_data = (GX_UBYTE *)glyph -> gx_glyph_map;

        /* First, skip to the starting row.  */
        for (row = 0; row < rotated_map_offset.gx_point_y; row++)
        {
            col = 0;
            while (col < glyph -> gx_glyph_height)
            {
                count = *glyph_data++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    glyph_data++;
                }
                else
                {
                    count++;
                    glyph_data += count;
                }
                col += count * 8;
            }
        }

        row = rotated_draw_area.gx_rectangle_top;

        /* Now we are on the first visible row, copy pixels until we get
           to the enf of the last visible row.  */
        while (row < rotated_draw_area.gx_rectangle_bottom + 1)
        {
            start_x = rotated_draw_area.gx_rectangle_left - rotated_map_offset.gx_point_x;
            col = start_x;
            while (col < start_x + glyph -> gx_glyph_height)
            {
                count = *glyph_data++;
                if (count & 0x80)
                {
                    count = (GX_UBYTE)((count & 0x7f) + 1);
                    alpha_value = *glyph_data++;
                    while (count--)
                    {
                        mask = 0x01;
                        if (alpha_value != 0)
                        {
                            for (index = 0; index < 8; index++)
                            {
                                alpha = alpha_value & mask;
                                mask = (GX_UBYTE)(mask << 1);
                                if (alpha != 0)
                                {
                                    if ((col <= rotated_draw_area.gx_rectangle_right) &&
                                        (col >= rotated_draw_area.gx_rectangle_left))
                                    {
                                        _gx_display_driver_16bpp_pixel_write(context, col, row, text_color);
                                    }
                                }
                                col++;
                            }
                        }
                        else
                        {
                            col += 8;
                        }
                    }
                }
                else
                {
                    count++;
                    while (count--)
                    {
                        alpha_value = *glyph_data++;
                        mask = 0x01;
                        if (alpha_value != 0)
                        {
                            for (index = 0; index < 8; index++)
                            {
                                alpha = alpha_value & mask;
                                mask = (GX_UBYTE)(mask << 1);
                                if (alpha != 0)
                                {
                                    if ((col <= rotated_draw_area.gx_rectangle_right) &&
                                        (col >= rotated_draw_area.gx_rectangle_left))
                                    {
                                        _gx_display_driver_16bpp_pixel_write(context, col, row, text_color);
                                    }
                                }
                                col++;
                            }
                        }
                        else
                        {
                            col += 8;
                        }
                    }
                }
            }
            row++;
        }
    }
    else
    {
        _gx_synergy_simulation_display_driver_16bpp_rotated_glyph_1bit_draw(context, draw_area, map_offset, glyph);
    }
}

