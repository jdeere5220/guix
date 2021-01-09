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
/**   Display Management (Display)                                        */
/**                                                                       */
/**************************************************************************/

#define REDVAL(_c)   (GX_UBYTE)(((_c) >> 11) & 0x1f)
#define GREENVAL(_c) (GX_UBYTE)(((_c) >> 5) & 0x3f)
#define BLUEVAL(_c)  (GX_UBYTE)(((_c)) & 0x1f)

#define ASSEMBLECOLOR(_r, _g, _b) \
    ((((_r) & 0x1f) << 11) |      \
     (((_g) & 0x3f) << 5) |       \
     (((_b) & 0x1f)))

#define GX_SOURCE_CODE

/* Include necessary system files.  */

#include "gx_api.h"
#include "gx_display.h"
#include "gx_context.h"
#include "gx_utility.h"
#include "gx_system.h"

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_pixelmap_raw_rotate               */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*      without alpha.                                                    */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*    [gx_display_driver_pixel_blend]       Display driver basic pixel    */
/*                                             blend function             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Kenneth Maxwell          Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_rotated_pixelmap_raw_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                  INT angle, INT cx, INT cy)
{
USHORT       *get;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
INT           alpha;
USHORT        red;
USHORT        green;
USHORT        blue;
INT           idxminx;
INT           idxmaxx;
INT           idxmaxy;
INT          *mx;
INT          *my;
INT           xres;
INT           yres;
INT           x;
INT           y;
INT           xx;
INT           yy;
USHORT        a;
USHORT        b;
USHORT        c;
USHORT        d;
INT           xdiff;
INT           ydiff;
INT           newxpos;
INT           newypos;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    clip = context -> gx_draw_context_clip;
    blend_func = _gx_display_driver_565rgb_pixel_blend;

    mx = _gx_system_scratchpad;
    my = mx + 4;

    mx[0] = mx[3] = -1;
    mx[1] = mx[2] = 1;

    my[0] = my[1] = 1;
    my[2] = my[3] = -1;

    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = pixelmap -> gx_pixelmap_height >> 1;
    srcyres = pixelmap -> gx_pixelmap_width >> 1;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        srcyres = pixelmap -> gx_pixelmap_width - 1 - srcyres;

        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        srcxres = pixelmap -> gx_pixelmap_height - 1 - srcxres;

        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT(mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv);
    yres = GX_FIXED_VAL_TO_INT(my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv);

    /* Calculate the new rotation axis. */

    x = GX_FIXED_VAL_TO_INT((cx - srcxres) * cosv - (cy - srcyres) * sinv);
    y = GX_FIXED_VAL_TO_INT((cy - srcyres) * cosv + (cx - srcxres) * sinv);

    x += xres;
    y += yres;

    newxpos = xpos + cx - x;
    newypos = ypos + cy - y;

    /* Loop through the destination's pixels.  */
    for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
    {
        for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx) + srcxres;
            yy = GX_FIXED_VAL_TO_INT(yy) + srcyres;

            if ((xx >= -1) && (xx < pixelmap -> gx_pixelmap_height) &&
                (yy >= -1) && (yy < pixelmap -> gx_pixelmap_width))
            {
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_height - 1) &&
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_width - 1))
                {
                    get = (USHORT *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_height;
                    get += xx;

                    a = *get;
                    b = *(get + 1);
                    c = *(get + pixelmap -> gx_pixelmap_height);
                    d = *(get + pixelmap -> gx_pixelmap_height + 1);

                    alpha = 0xff;
                }
                else
                {
                    get = (USHORT *)pixelmap -> gx_pixelmap_data;

                    a = 0;
                    b = 0;
                    c = 0;
                    d = 0;
                    alpha = 0;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * pixelmap -> gx_pixelmap_height);
                            alpha += xdiff * (256 - ydiff);
                        }

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height);
                            alpha += xdiff * ydiff;
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha += ydiff * (256 - xdiff);

                        if (xx < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + xx + 1);
                            alpha += xdiff * ydiff;
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_height - 1)
                    {
                        /* handle right edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);
                        alpha += (256 - xdiff) * (256 - ydiff);

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height + xx);
                            alpha += ydiff * (256 - xdiff);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);
                        alpha += (256 - xdiff) * (256 - ydiff);

                        b = *(get + yy * pixelmap -> gx_pixelmap_height + xx + 1);
                        alpha += xdiff * (256 - ydiff);
                    }

                    alpha >>= 8;
                }

                red = (USHORT)((REDVAL(a) * (256 - xdiff) * (256 - ydiff) +
                                REDVAL(b) * xdiff * (256 - ydiff) +
                                REDVAL(c) * ydiff * (256 - xdiff) +
                                REDVAL(d) * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL(a) * (256 - xdiff) * (256 - ydiff) +
                                  GREENVAL(b) * xdiff * (256 - ydiff) +
                                  GREENVAL(c) * ydiff * (256 - xdiff) +
                                  GREENVAL(d) * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL(a) * (256 - xdiff) * (256 - ydiff) +
                                 BLUEVAL(b) * xdiff * (256 - ydiff) +
                                 BLUEVAL(c) * ydiff * (256 - xdiff) +
                                 BLUEVAL(d) * xdiff * ydiff) >> 16);

                if ((alpha > 0) && (alpha < 0xff))
                {
                    red = (USHORT)((red << 8) / alpha);
                    green = (USHORT)((green << 8) / alpha);
                    blue = (USHORT)((blue << 8) / alpha);
                }

                red = red > 31 ? 31 : red;
                green = green > 63 ? 63 : green;
                blue = blue > 31 ? 31 : blue;
                alpha = alpha > 255 ? 255 : alpha;

                blend_func(context, x + newxpos, y + newypos, (GX_COLOR)ASSEMBLECOLOR(red, green, blue), (GX_UBYTE)alpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_pixelmap_alpha_rotate             */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal helper function that rotate an uncompressed pixelmap       */
/*      with alpha.                                                       */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_utility_math_cos                  Compute the cosine value      */
/*    _gx_utility_math_sin                  Compute the sine value        */
/*    [gx_display_driver_pixel_blend]       Display driver basic pixel    */
/*                                             blend function             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Kenneth Maxwell          Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_rotated_pixelmap_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                    INT angle, INT cx, INT cy)
{
USHORT       *get;
GX_UBYTE     *getalpha;
INT           srcxres;
INT           srcyres;
INT           cosv;
INT           sinv;
USHORT        red;
USHORT        green;
USHORT        blue;
INT           idxminx;
INT           idxmaxx;
INT           idxmaxy;
INT          *mx;
INT          *my;
INT           xres;
INT           yres;
INT           x;
INT           y;
INT           xx;
INT           yy;
USHORT        a;
USHORT        b;
USHORT        c;
USHORT        d;
USHORT        alpha[4];
GX_FIXED_VAL  xdiff;
GX_FIXED_VAL  ydiff;
INT           newxpos;
INT           newypos;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    clip = context -> gx_draw_context_clip;
    blend_func = _gx_display_driver_565rgb_pixel_blend;

    mx = _gx_system_scratchpad;
    my = mx + 4;

    mx[0] = mx[3] = -1;
    mx[1] = mx[2] = 1;

    my[0] = my[1] = 1;
    my[2] = my[3] = -1;

    idxminx = (angle / 90) & 0x3;
    idxmaxx = (idxminx + 2) & 0x3;
    idxmaxy = (idxminx + 1) & 0x3;

    /* Calculate the source x and y center. */
    srcxres = pixelmap -> gx_pixelmap_height >> 1;
    srcyres = pixelmap -> gx_pixelmap_width >> 1;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        srcyres = pixelmap -> gx_pixelmap_width - 1 - srcyres;

        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        srcxres = pixelmap -> gx_pixelmap_height - 1 - srcxres;

        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    cosv = _gx_utility_math_cos(GX_FIXED_VAL_MAKE(angle));
    sinv = _gx_utility_math_sin(GX_FIXED_VAL_MAKE(angle));

    xres = GX_FIXED_VAL_TO_INT(mx[idxmaxx] * (srcxres + 2) * cosv - my[idxmaxx] * (srcyres + 2) * sinv);
    yres = GX_FIXED_VAL_TO_INT(my[idxmaxy] * (srcyres + 2) * cosv + mx[idxmaxy] * (srcxres + 2) * sinv);

    x = GX_FIXED_VAL_TO_INT((cx - srcxres) * cosv - (cy - srcyres) * sinv);
    y = GX_FIXED_VAL_TO_INT((cy - srcyres) * cosv + (cx - srcxres) * sinv);

    x += xres;
    y += yres;

    newxpos = xpos + cx - x;
    newypos = ypos + cy - y;

    /* Loop through the source's pixels.  */
    for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
    {
        for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
        {
            xx = (x - xres) * cosv + (y - yres) * sinv;
            yy = (y - yres) * cosv - (x - xres) * sinv;

            xdiff = GX_FIXED_VAL_TO_INT(xx << 8) & 0xff;
            ydiff = GX_FIXED_VAL_TO_INT(yy << 8) & 0xff;

            xx = GX_FIXED_VAL_TO_INT(xx);
            yy = GX_FIXED_VAL_TO_INT(yy);

            xx += srcxres;
            yy += srcyres;

            if ((xx >= -1) && (xx < pixelmap -> gx_pixelmap_height) &&
                (yy >= -1) && (yy < pixelmap -> gx_pixelmap_width))
            {
                if ((xx >= 0) && (xx < pixelmap -> gx_pixelmap_height - 1) && \
                    (yy >= 0) && (yy < pixelmap -> gx_pixelmap_width - 1))
                {
                    get = (USHORT *)pixelmap -> gx_pixelmap_data;
                    get += yy * pixelmap -> gx_pixelmap_height;
                    get += xx;

                    getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                    getalpha += yy * pixelmap -> gx_pixelmap_height;
                    getalpha += xx;

                    a = *get;
                    alpha[0] = *getalpha;

                    b = *(get + 1);
                    alpha[1] = *(getalpha + 1);

                    c = *(get + pixelmap -> gx_pixelmap_height);
                    alpha[2] = *(getalpha + pixelmap -> gx_pixelmap_height);

                    d = *(get + pixelmap -> gx_pixelmap_height + 1);
                    alpha[3] = *(getalpha + pixelmap -> gx_pixelmap_height + 1);
                }
                else
                {
                    get = (USHORT *)pixelmap -> gx_pixelmap_data;
                    getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

                    a = 0;
                    b = 0;
                    c = 0;
                    d = 0;

                    if (xx == -1)
                    {
                        /* handle left edge.  */
                        if (yy >= 0)
                        {
                            b = *(get + yy * pixelmap -> gx_pixelmap_height);
                            alpha[1] = *(getalpha + yy * pixelmap -> gx_pixelmap_height);
                        }

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            d = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height);
                            alpha[3] = *(getalpha + (yy + 1) * pixelmap -> gx_pixelmap_height);
                        }
                    }
                    else if (yy == -1)
                    {
                        /* handle top edge.  */
                        c = *(get + xx);
                        alpha[2] = *(getalpha + xx);

                        if (xx < pixelmap -> gx_pixelmap_height - 1)
                        {
                            d = *(get + xx + 1);
                            alpha[3] = *(getalpha + xx + 1);
                        }
                    }
                    else if (xx == pixelmap -> gx_pixelmap_height - 1)
                    {
                        /* handle right edget. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);
                        alpha[0] = *(getalpha + yy * pixelmap -> gx_pixelmap_height + xx);

                        if (yy < pixelmap -> gx_pixelmap_width - 1)
                        {
                            c = *(get + (yy + 1) * pixelmap -> gx_pixelmap_height + xx);
                            alpha[2] = *(getalpha + (yy + 1) * pixelmap -> gx_pixelmap_height + xx);
                        }
                    }
                    else
                    {
                        /* handle bottom edge. */
                        a = *(get + yy * pixelmap -> gx_pixelmap_height + xx);
                        alpha[0] = *(getalpha + yy * pixelmap -> gx_pixelmap_height + xx);

                        b = *(get + yy * pixelmap -> gx_pixelmap_height + xx + 1);
                        alpha[1] = *(getalpha + yy * pixelmap -> gx_pixelmap_height + xx + 1);
                    }

                    if (!a)
                    {
                        alpha[0] = 0;
                    }

                    if (!b)
                    {
                        alpha[1] = 0;
                    }

                    if (!c)
                    {
                        alpha[2] = 0;
                    }

                    if (!d)
                    {
                        alpha[3] = 0;
                    }
                }

                red = (USHORT)((REDVAL(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                REDVAL(b) * alpha[1] * xdiff * (256 - ydiff) +
                                REDVAL(c) * alpha[2] * ydiff * (256 - xdiff) +
                                REDVAL(d) * alpha[3] * xdiff * ydiff) >> 16);

                green = (USHORT)((GREENVAL(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                  GREENVAL(b) * alpha[1] * xdiff * (256 - ydiff) +
                                  GREENVAL(c) * alpha[2] * ydiff * (256 - xdiff) +
                                  GREENVAL(d) * alpha[3] * xdiff * ydiff) >> 16);

                blue = (USHORT)((BLUEVAL(a) * alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                 BLUEVAL(b) * alpha[1] * xdiff * (256 - ydiff) +
                                 BLUEVAL(c) * alpha[2] * ydiff * (256 - xdiff) +
                                 BLUEVAL(d) * alpha[3] * xdiff * ydiff) >> 16);

                alpha[0] = (USHORT)((alpha[0] * (256 - xdiff) * (256 - ydiff) +
                                     alpha[1] * xdiff * (256 - ydiff) +
                                     alpha[2] * ydiff * (256 - xdiff) +
                                     alpha[3] * xdiff * ydiff) >> 16);

                if (alpha[0])
                {
                    red /= alpha[0];
                    green /= alpha[0];
                    blue /= alpha[0];
                }

                red = red > 31 ? 31 : red;
                green = green > 63 ? 63 : green;
                blue = blue > 31 ? 31 : blue;
                alpha[0] = alpha[0] > 255 ? 255 : alpha[0];

                blend_func(context, x + newxpos, y + newypos, (GX_COLOR)ASSEMBLECOLOR(red, green, blue), (GX_UBYTE)alpha[0]);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_16bpp_rotated_pixelmap_simple_rotate             */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal help function that hangles 90, 180 and 270 degree pixelmap */
/*    rotation.                                                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Kenneth Maxwell          Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_16bpp_rotated_pixelmap_simple_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                    INT angle, INT cx, INT cy)
{
USHORT       *putrow;
USHORT       *put;
USHORT       *get;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;

    clip = context -> gx_draw_context_clip;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        putrow = (USHORT *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (USHORT *)pixelmap -> gx_pixelmap_data;
                get += (width - 1 - x) * height;
                get += y;

                *put++ = *get;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
    else if (angle == 180)
    {

        width = pixelmap -> gx_pixelmap_height;
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - (width - 1 - cx);
        newypos = ypos + cy - (height - 1 - cy);

        putrow = (USHORT *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (USHORT *)pixelmap -> gx_pixelmap_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;

                *put++ = *get;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
    else
    {

        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        putrow = (USHORT *)context -> gx_draw_context_memory;
        putrow += rotated_clip.gx_rectangle_top * context -> gx_draw_context_pitch;
        putrow += rotated_clip.gx_rectangle_left;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            put = putrow;

            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (USHORT *)pixelmap -> gx_pixelmap_data;
                get += x * height;
                get += height - 1 - y;

                *put++ = *get;
            }

            putrow += context -> gx_draw_context_pitch;
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_pixelmap_simple_alpha_rotate      */
/*                                                        PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal help function that hangles 90, 180 and 270 degree pixelmap */
/*    rotation with alpha channel.                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    cx                                    x-coord of rotate center      */
/*    cy                                    y-coord of rotate center      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    [gx_display_driver_pixel_blend]       Display driver basic pixel    */
/*                                             blend function             */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Kenneth Maxwell          Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
static VOID _gx_display_driver_565rgb_rotated_pixelmap_simple_alpha_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                                           INT angle, INT cx, INT cy)
{
USHORT       *get;
GX_UBYTE     *getalpha;
INT           width;
INT           height;
INT           x;
INT           y;
GX_RECTANGLE *clip;
GX_RECTANGLE  rotated_clip;
INT           newxpos;
INT           newypos;
VOID          (*blend_func)(GX_DRAW_CONTEXT *context, INT x, INT y, GX_COLOR color, GX_UBYTE alpha);

    clip = context -> gx_draw_context_clip;
    blend_func = _gx_display_driver_565rgb_pixel_blend;

    get = (USHORT *)pixelmap -> gx_pixelmap_data;
    getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;

    GX_SWAP_VALS(xpos, ypos);
    GX_SWAP_VALS(cx, cy);

    if (context -> gx_draw_context_display -> gx_display_rotation_angle == GX_SCREEN_ROTATION_CW)
    {
        ypos = context -> gx_draw_context_canvas -> gx_canvas_x_resolution - ypos - pixelmap -> gx_pixelmap_width;
        cy = pixelmap -> gx_pixelmap_width - cy - 1;

        rotated_clip.gx_rectangle_left = clip -> gx_rectangle_top;
        rotated_clip.gx_rectangle_right = clip -> gx_rectangle_bottom;
        rotated_clip.gx_rectangle_top = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_right);
        rotated_clip.gx_rectangle_bottom = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_x_resolution - 1 - clip -> gx_rectangle_left);
    }
    else
    {
        xpos = context -> gx_draw_context_canvas -> gx_canvas_y_resolution - xpos - pixelmap -> gx_pixelmap_height;
        cx = pixelmap -> gx_pixelmap_height - cx - 1;

        rotated_clip.gx_rectangle_left = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_bottom);
        rotated_clip.gx_rectangle_right = (GX_VALUE)(context -> gx_draw_context_canvas -> gx_canvas_y_resolution - 1 - clip -> gx_rectangle_top);
        rotated_clip.gx_rectangle_top = clip -> gx_rectangle_left;
        rotated_clip.gx_rectangle_bottom = clip -> gx_rectangle_right;
    }

    if (angle == 90)
    {
        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - (width - 1 - cy);
        newypos = ypos + cy - cx;

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (USHORT *)pixelmap -> gx_pixelmap_data;
                getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                get += (width - 1 - x) * height;
                get += y;
                getalpha += (width - 1 - x) * height;
                getalpha += y;
                blend_func(context, x + newxpos, y + newypos, *get, *getalpha);
            }
        }
    }
    else if (angle == 180)
    {

        width = pixelmap -> gx_pixelmap_height;
        height = pixelmap -> gx_pixelmap_width;

        newxpos = xpos + cx - (width - 1 - cx);
        newypos = ypos + cy - (height - 1 - cy);


        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (USHORT *)pixelmap -> gx_pixelmap_data;
                getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                get += (height - 1 - y) * width;
                get += width - 1 - x;
                getalpha += (height - 1 - y) * width;
                getalpha += width - 1 - x;

                blend_func(context, x + newxpos, y + newypos, *get, *getalpha);
            }
        }
    }
    else
    {

        width = pixelmap -> gx_pixelmap_width;
        height = pixelmap -> gx_pixelmap_height;

        newxpos = xpos + cx - cy;
        newypos = ypos + cx - (height - 1 - cy);

        for (y = rotated_clip.gx_rectangle_top - newypos; y <= rotated_clip.gx_rectangle_bottom - newypos; y++)
        {
            for (x = rotated_clip.gx_rectangle_left - newxpos; x <= rotated_clip.gx_rectangle_right - newxpos; x++)
            {
                get = (USHORT *)pixelmap -> gx_pixelmap_data;
                getalpha = (GX_UBYTE *)pixelmap -> gx_pixelmap_aux_data;
                get += x * height;
                get += height - 1 - y;
                getalpha += x * height;
                getalpha += height - 1 - y;

                blend_func(context, x + newxpos, y + newypos, *get, *getalpha);
            }
        }
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_pixelmap_rotate   PORTABLE C      */
/*                                                           6.x          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This service rotate a pixelmap directly to canvas memory.           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    context                               Drawing context               */
/*    xpos                                  x-coord of top-left draw point*/
/*    ypos                                  y-coord of top-left draw point*/
/*    pixelmap                              Pointer to GX_PIXELMAP struct */
/*    angle                                 The angle to rotate           */
/*    rot_cx                                x-coord of rotating center.   */
/*    rot_cy                                y-coord of rotationg center.  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    status                                Completion status             */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_display_driver_565rgb_rotated_pixelmap_simple_alpha_rotate      */
/*                                          Real rotate functin           */
/*    _gx_display_driver_16bpp_rotated_pixelmap_simple_rotate             */
/*                                          Real rotate functin           */
/*    _gx_display_driver_565rgb_rotated_pixelmap_alpha_rotate             */
/*                                          Real rotate functin           */
/*    _gx_display_driver_565rgb_rotated_pixelmap_raw_rotate               */
/*                                          Real rotate functin           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application Code                                                    */
/*    GUIX Internal Code                                                  */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  xx-xx-xxxx     Kenneth Maxwell          Initial Version 6.x           */
/*                                                                        */
/**************************************************************************/
VOID _gx_display_driver_565rgb_rotated_pixelmap_rotate(GX_DRAW_CONTEXT *context, INT xpos, INT ypos, GX_PIXELMAP *pixelmap,
                                                       INT angle, INT rot_cx, INT rot_cy)
{
    switch (pixelmap -> gx_pixelmap_format)
    {
    case GX_COLOR_FORMAT_565RGB:
        if (angle % 90 == 0)
        {
            /* Simple angle rotate: 90 degree, 180 degree and 270 degree.  */
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                _gx_display_driver_565rgb_rotated_pixelmap_simple_alpha_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            else
            {
                _gx_display_driver_16bpp_rotated_pixelmap_simple_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            break;
        }
        else
        {
            /* General rotation case. */
            if (pixelmap -> gx_pixelmap_flags & GX_PIXELMAP_ALPHA)
            {
                /* alpha, no compression */
                _gx_display_driver_565rgb_rotated_pixelmap_alpha_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            else
            {

                /* no compression or alpha */
                _gx_display_driver_565rgb_rotated_pixelmap_raw_rotate(context, xpos, ypos, pixelmap, angle, rot_cx, rot_cy);
            }
            break;
        }
    }
    return;
}

