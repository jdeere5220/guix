/*******************************************************************************/
/*  This file is auto-generated by Azure RTOS GUIX Studio. Do not edit this    */
/*  file by hand. Modifications to this file should only be made by running    */
/*  the Azure RTOS GUIX Studio application and re-generating the application   */
/*  specification file(s). For more information please refer to the Azure RTOS */
/*  GUIX Studio User Guide, or visit our web site at azure.com/rtos            */
/*                                                                             */
/*  GUIX Studio Revision 6.1.0.0                                               */
/*  Date (dd.mm.yyyy):  9.11.2020   Time (hh:mm): 18:07                        */
/*******************************************************************************/


#ifndef _GUIX_WIDGET_TYPES_SPECIFICATIONS_H_
#define _GUIX_WIDGET_TYPES_SPECIFICATIONS_H_

#include "gx_api.h"

/* Determine if C++ compiler is being used, if so use standard C.              */
#ifdef __cplusplus
extern   "C" {
#endif

/* Define widget ids                                                           */

#define ID_MENU_SCREEN 1
#define ID_SCROLL_WHEEL_SCREEN 2
#define ID_SCROLL_WHEEL_DAY 3
#define ID_SCROLL_WHEEL_YEAR 4
#define ID_SCROLL_WHEEL_MONTH 5
#define IDB_NEXT 6
#define ID_GAUGE_SCREEN 7
#define ID_GAUGE_ANIMATED 8
#define ID_PROMPT_ANIMATED 9
#define ID_SLIDER_ANIMATED 10
#define ID_SLIDER_NOT_ANIMATED 11
#define ID_GAUGE_NOT_ANIMATED 12
#define ID_PROMPT_NOT_ANIMATED 13
#define ID_WINDOW_SCREEN 14
#define ID_NESTED_WINDOW_BLUE 15
#define ID_VERTICAL_LIST 16
#define ID_HORIZONTAL_LIST 17
#define ICON_BUTTON_DISC 18
#define ICON_BUTTON_USER 19
#define ICON_BUTTON_PEN 20
#define ICON_BUTTON_TRASH 21
#define ICON_BUTTON_COG 22
#define ID_DROP_LIST 23
#define ID_SLIDER_SCREEN 24
#define ID_PIXELMAP_SLIDER_H 25
#define ID_PIXELMAP_SLIDER_V 26
#define ID_PIXELMAP_SLIDER_THIN_H 27
#define ID_PROGRESS_BAR 28
#define ID_PROGRESS_BAR_PROMPT 29
#define ID_TEXT_SCREEN 30
#define ID_BUTTON_SCREEN 31
#define IDB_CHECKBOX 32


/* Define animation ids                                                        */

#define GX_NEXT_ANIMATION_ID 1


/* Define user event ids                                                       */

#define GX_NEXT_USER_EVENT_ID GX_FIRST_USER_EVENT

#define GX_ACTION_FLAG_DYNAMIC_TARGET 0x01
#define GX_ACTION_FLAG_DYNAMIC_PARENT 0x02
#define GX_ACTION_FLAG_POP_TARGET     0x04
#define GX_ACTION_FLAG_POP_PARENT     0x08

typedef struct GX_STUDIO_ACTION_STRUCT
{
    GX_UBYTE opcode;
    GX_UBYTE flags;
    GX_CONST VOID *parent;
    GX_CONST VOID *target;
    GX_CONST GX_ANIMATION_INFO  *animation;
} GX_STUDIO_ACTION;

typedef struct GX_STUDIO_EVENT_ENTRY_STRUCT
{
    ULONG event_type;
    USHORT event_sender;
    GX_CONST GX_STUDIO_ACTION *action_list;
} GX_STUDIO_EVENT_ENTRY;

typedef struct GX_STUDIO_EVENT_PROCESS_STRUCT 
{
    GX_CONST GX_STUDIO_EVENT_ENTRY *event_table;
    UINT (*chain_event_handler)(GX_WIDGET *, GX_EVENT *);
} GX_STUDIO_EVENT_PROCESS;

/* Declare properties structures for each utilized widget type                 */

typedef struct GX_STUDIO_WIDGET_STRUCT
{
   GX_CHAR *widget_name;
   USHORT  widget_type;
   USHORT  widget_id;
   #if defined(GX_WIDGET_USER_DATA)
   INT   user_data;
   #endif
   ULONG style;
   ULONG status;
   ULONG control_block_size;
   GX_RESOURCE_ID normal_fill_color_id;
   GX_RESOURCE_ID selected_fill_color_id;
   GX_RESOURCE_ID disabled_fill_color_id;
   UINT (*create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);
   void (*draw_function) (GX_WIDGET *);
   UINT (*event_function) (GX_WIDGET *, GX_EVENT *);
   GX_RECTANGLE size;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *next_widget;
   GX_CONST struct GX_STUDIO_WIDGET_STRUCT *child_widget;
   ULONG control_block_offset;
   GX_CONST void *properties;
} GX_STUDIO_WIDGET;

typedef struct
{
    GX_CONST GX_STUDIO_WIDGET *widget_information;
    GX_WIDGET        *widget;
} GX_STUDIO_WIDGET_ENTRY;

typedef struct
{
    GX_RESOURCE_ID string_id; 
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
} GX_TEXT_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id; 
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
} GX_ML_TEXT_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id; 
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID unchecked_pixelmap_id;
    GX_RESOURCE_ID checked_pixelmap_id;
    GX_RESOURCE_ID unchecked_disabled_pixelmap_id;
    GX_RESOURCE_ID checked_disabled_pixelmap_id;
} GX_CHECKBOX_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id; 
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID off_pixelmap_id;
    GX_RESOURCE_ID on_pixelmap_id;
    GX_RESOURCE_ID off_disabled_pixelmap_id;
    GX_RESOURCE_ID on_disabled_pixelmap_id;
} GX_RADIO_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID pixelmap_id;
} GX_ICON_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
    GX_RESOURCE_ID disabled_pixelmap_id;
} GX_PIXELMAP_BUTTON_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
} GX_ICON_PROPERTIES;

typedef struct
{
    int min_val;
    int max_val;
    int current_val;
    int increment;
    GX_VALUE min_travel;
    GX_VALUE max_travel;
    GX_VALUE needle_width;
    GX_VALUE needle_height;
    GX_VALUE needle_inset;
    GX_VALUE needle_hotspot;
    GX_RESOURCE_ID lower_pixelmap;
    GX_RESOURCE_ID upper_pixelmap;
    GX_RESOURCE_ID needle_pixelmap;
} GX_PIXELMAP_SLIDER_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
} GX_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    VOID (*format_func)(GX_NUMERIC_PROMPT *, INT);
    INT            numeric_prompt_value;
} GX_NUMERIC_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID left_map_id;
    GX_RESOURCE_ID fill_map_id;
    GX_RESOURCE_ID right_map_id;
    GX_RESOURCE_ID selected_left_map_id;
    GX_RESOURCE_ID selected_fill_map_id;
    GX_RESOURCE_ID selected_right_map_id;
} GX_PIXELMAP_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID left_map_id;
    GX_RESOURCE_ID fill_map_id;
    GX_RESOURCE_ID right_map_id;
    GX_RESOURCE_ID selected_left_map_id;
    GX_RESOURCE_ID selected_fill_map_id;
    GX_RESOURCE_ID selected_right_map_id;
    VOID (*format_func)(GX_NUMERIC_PIXELMAP_PROMPT *, INT);
    INT            numeric_prompt_value;
} GX_NUMERIC_PIXELMAP_PROMPT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID wallpaper_id;
} GX_WINDOW_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID wallpaper_id;
    VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT);
    int total_rows;
} GX_VERTICAL_LIST_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID wallpaper_id;
    VOID (*callback)(GX_HORIZONTAL_LIST *, GX_WIDGET *, INT);
    int total_rows;
} GX_HORIZONTAL_LIST_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID pixelmap_id;
    GX_RESOURCE_ID wallpaper_id;
    VOID (*callback)(GX_VERTICAL_LIST *, GX_WIDGET *, INT);
    int total_rows;
    int open_height;
} GX_DROP_LIST_PROPERTIES;

typedef struct
{
    int                 total_rows;
    int                 selected_row;
    GX_VALUE            row_height;
    GX_UBYTE            start_alpha;
    GX_UBYTE            end_alpha;
    GX_RESOURCE_ID      normal_font;
    GX_RESOURCE_ID      selected_font;
    GX_RESOURCE_ID      normal_text_color;
    GX_RESOURCE_ID      selected_text_color;
    GX_RESOURCE_ID      disabled_text_color;
    GX_RESOURCE_ID      wallpaper_id;
    GX_RESOURCE_ID      selected_background;
    GX_CONST GX_RESOURCE_ID  *string_id_list;
    UINT (*callback)(struct GX_NUMERIC_SCROLL_WHEEL_STRUCT*, INT, GX_STRING *);
} GX_STRING_SCROLL_WHEEL_PROPERTIES;

typedef struct
{
    int                 total_rows;
    int                 selected_row;
    GX_VALUE            row_height;
    GX_UBYTE            start_alpha;
    GX_UBYTE            end_alpha;
    GX_RESOURCE_ID      normal_font;
    GX_RESOURCE_ID      selected_font;
    GX_RESOURCE_ID      normal_text_color;
    GX_RESOURCE_ID      selected_text_color;
    GX_RESOURCE_ID      disabled_text_color;
    GX_RESOURCE_ID      wallpaper_id;
    GX_RESOURCE_ID      selected_background;
    UINT (*callback)(struct GX_NUMERIC_SCROLL_WHEEL_STRUCT*, INT, GX_STRING *);
    int                 start_val;
    int                 end_val;
} GX_NUMERIC_SCROLL_WHEEL_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID readonly_fill_color_id;
    GX_RESOURCE_ID readonly_text_color_id;
    GX_CHAR *buffer;
    UINT buffer_size;
} GX_SINGLE_LINE_TEXT_INPUT_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_UBYTE       whitespace;
    GX_BYTE        line_space;
} GX_ML_TEXT_VIEW_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID readonly_fill_color_id;
    GX_RESOURCE_ID readonly_text_color_id;
    GX_UBYTE whitespace;
    GX_BYTE  line_space;
    GX_CHAR *buffer;
    UINT buffer_size;
} GX_MULTI_LINE_TEXT_INPUT_PROPERTIES;

typedef struct
{
    GX_VALUE start_angle;
    GX_VALUE animation_steps;
    GX_VALUE animation_delay;
    GX_VALUE needle_xpos;
    GX_VALUE needle_ypos;
    GX_VALUE needle_xcor;
    GX_VALUE needle_ycor;
    GX_RESOURCE_ID normal_pixelmap_id;
    GX_RESOURCE_ID selected_pixelmap_id;
    GX_RESOURCE_ID needle_pixelmap_id;
} GX_CIRCULAR_GAUGE_PROPERTIES;

typedef struct
{
   GX_CONST GX_STUDIO_WIDGET *base_info;
   UINT (*base_create_function) (GX_CONST struct GX_STUDIO_WIDGET_STRUCT *, GX_WIDGET *, GX_WIDGET *);
   GX_RECTANGLE size;
} GX_TEMPLATE_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID string_id;
    GX_RESOURCE_ID font_id;
    GX_RESOURCE_ID normal_text_color_id;
    GX_RESOURCE_ID selected_text_color_id;
    GX_RESOURCE_ID disabled_text_color_id;
    GX_RESOURCE_ID left_map_id;
    GX_RESOURCE_ID fill_map_id;
    GX_RESOURCE_ID right_map_id;
    GX_RESOURCE_ID selected_left_map_id;
    GX_RESOURCE_ID selected_fill_map_id;
    GX_RESOURCE_ID selected_right_map_id;
    GX_VALUE       text_x_offset;
    GX_VALUE       text_y_offset;
    GX_VALUE       list_total_count;
} GX_MENU_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID expand_map_id;
    GX_RESOURCE_ID collapse_map_id;
    GX_RESOURCE_ID root_line_color_id;
    GX_VALUE       indentation;
} GX_TREE_VIEW_PROPERTIES;

typedef struct
{
    GX_RESOURCE_ID      string_id;
    GX_RESOURCE_ID      normal_text_color_id;
    GX_RESOURCE_ID      selected_text_color_id;
    GX_RESOURCE_ID      disabled_text_color_id;
    GX_UBYTE            whitespace;
    GX_BYTE             line_space;
    GX_RICH_TEXT_FONTS *fonts;
} GX_RICH_TEXT_VIEW_PROPERTIES;


/* Declare top-level control blocks                                            */

typedef struct BASE_SCREEN_CONTROL_BLOCK_STRUCT
{
    GX_WINDOW_MEMBERS_DECLARE
    GX_ICON Base_Screen_Icon_EL_Logo;
    GX_PIXELMAP_BUTTON Base_Screen_Next_Button;
} BASE_SCREEN_CONTROL_BLOCK;

typedef struct GAUGE_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_PROMPT Gauge_Screen_Prompt_Circular_Gauges;
    GX_CIRCULAR_GAUGE Gauge_Screen_Gauge_Animated;
    GX_PROMPT Gauge_Screen_prompt_animated;
    GX_PIXELMAP_SLIDER Gauge_Screen_Pixelmap_Slider_Animated;
    GX_PIXELMAP_SLIDER Gauge_Screen_Pixelmap_Slider_Not_Animated;
    GX_PROMPT Gauge_Screen_Prompt_Animated;
    GX_PROMPT Gauge_Screen_Prompt_Not_Animated;
    GX_CIRCULAR_GAUGE Gauge_Screen_Gauge_Not_Animated;
    GX_PROMPT Gauge_Screen_prompt_not_animated;
} GAUGE_SCREEN_CONTROL_BLOCK;

typedef struct WINDOW_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_PROMPT Window_Screen_Prompt_Nested_Windows;
    GX_PROMPT Window_Screen_Prompt_Scollable_Frame;
    GX_WINDOW Window_Screen_Nested_Window_BG;
    GX_WINDOW Window_Screen_Nested_Window_Blue;
    GX_WINDOW Window_Screen_nest_window;
    GX_WINDOW Window_Screen_Scrollable_Frame_BG;
    GX_WINDOW Window_Screen_Scrollable_Frame_Sky;
    GX_SCROLLBAR Window_Screen_Hscroll;
    GX_SCROLLBAR Window_Screen_Vertical_Scroll;
    GX_PROMPT Window_Screen_Prompt_Vertical_List;
    GX_PROMPT Window_Screen_Prompt_Horizontal_List;
    GX_VERTICAL_LIST Window_Screen_Vertical_List;
    GX_WINDOW Window_Screen_window_6;
    GX_TEXT_BUTTON Window_Screen_button_1;
    GX_WINDOW Window_Screen_window_8;
    GX_TEXT_BUTTON Window_Screen_button_3;
    GX_WINDOW Window_Screen_window_7;
    GX_TEXT_BUTTON Window_Screen_button_2;
    GX_HORIZONTAL_LIST Window_Screen_Horizontal_List;
    GX_WINDOW Window_Screen_window_5;
    GX_ICON_BUTTON Window_Screen_icon_button_1;
    GX_WINDOW Window_Screen_window_9;
    GX_ICON_BUTTON Window_Screen_icon_button_2;
    GX_WINDOW Window_Screen_window_10;
    GX_ICON_BUTTON Window_Screen_icon_button_3;
    GX_WINDOW Window_Screen_window_11;
    GX_ICON_BUTTON Window_Screen_icon_button_4;
    GX_WINDOW Window_Screen_window_12;
    GX_ICON_BUTTON Window_Screen_icon_button_5;
    GX_DROP_LIST Window_Screen_Drop_List;
    GX_PIXELMAP_BUTTON Window_Screen_Drop_List_Button;
    GX_PROMPT Window_Screen_Prompt_Drop_List;
} WINDOW_SCREEN_CONTROL_BLOCK;

typedef struct SLIDER_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_PROMPT Slider_Screen_Prompt_Various_Sliders;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixelmap_Slider_H;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixelmap_Slider_V;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixelmap_Slider_Thin_H;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixelmap_Slider_Thin_V;
    GX_WINDOW Slider_Screen_Progress_Bar_BG;
    GX_PROGRESS_BAR Slider_Screen_Progress_Bar;
    GX_PROMPT Slider_Screen_Progress_Bar_Prompt;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixelmap_Slider_Red;
    GX_ICON Slider_Screen_Icon_Red_Top;
    GX_ICON Slider_Screen_Icon_Red_Bottom;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixelmap_Slider_Green;
    GX_ICON Slider_Screen_Icon_Green_Top;
    GX_ICON Slider_Screen_Icon_Green_Bottom;
    GX_PIXELMAP_SLIDER Slider_Screen_Pixlemap_Slider_Orange;
    GX_ICON Slider_Screen_Icon_Orange_Bottom;
    GX_ICON Slider_Screen_Icon_Orange_Top;
} SLIDER_SCREEN_CONTROL_BLOCK;

typedef struct TEXT_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_PROMPT Text_Screen_Prompt_Texts;
    GX_PROMPT Text_Screen_Prompt_Transparent_Thin_Border;
    GX_PROMPT Text_Screen_Prompt_Transparent_No_Border;
    GX_PROMPT Text_Screen_Prompt_Transparent_Thick_Border;
    GX_PIXELMAP_PROMPT Text_Screen_Pixelmap_Prompt;
    GX_MULTI_LINE_TEXT_VIEW Text_Screen_ML_Text_View;
    GX_MULTI_LINE_TEXT_INPUT Text_Screen_ML_Text_Input;
    GX_SINGLE_LINE_TEXT_INPUT Text_Screen_text_input;
    GX_NUMERIC_PROMPT Text_Screen_numeric_prompt;
    GX_NUMERIC_PIXELMAP_PROMPT Text_Screen_numeric_pixelmap_prompt;
    GX_RICH_TEXT_VIEW Text_Screen_ML_Text_Rich_View;
} TEXT_SCREEN_CONTROL_BLOCK;

typedef struct BUTTON_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_PROMPT Button_Screen_Prompt_Button;
    GX_PROMPT Button_Screen_Prompt_GX_BUTTON;
    GX_PROMPT Button_Screen_Prompt_GX_TEXT_BUTTON;
    GX_PROMPT Button_Screen_Prompt_GX_MULTI_LINE_TEXT_BUTTON;
    GX_PIXELMAP_BUTTON Button_Screen_Pixelmap_Button;
    GX_PROMPT Button_Screen_Prompt_GX_PIXELMAP_BUTTON;
    GX_ICON Button_Screen_Icon;
    GX_PROMPT Button_Screen_Prompt_GX_ICON_BUTTON;
    GX_PROMPT Button_Screen_Prompt_GX_CHECKBOX;
    GX_PROMPT Button_Screen_Prompt_GX_RADIO_BUTTON;
    GX_CHECKBOX Button_Screen_Checkbox;
    GX_RADIO_BUTTON Button_Screen_Radio_Button_Off;
    GX_RADIO_BUTTON Button_Screen_Radio_Button_On;
    GX_ICON_BUTTON Button_Screen_Icon_Button;
    GX_TEXT_BUTTON Button_Screen_Text_Button;
    GX_BUTTON Button_Screen_Button;
    GX_MULTI_LINE_TEXT_BUTTON Button_Screen_Multi_Line_Button;
} BUTTON_SCREEN_CONTROL_BLOCK;

typedef struct MENU_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_PROMPT Menu_Screen_title;
    GX_ACCORDION_MENU Menu_Screen_accordion;
    GX_MENU Menu_Screen_accordion_menu_1;
    GX_ACCORDION_MENU Menu_Screen_embedded_accordion;
    GX_MENU Menu_Screen_accordion_menu_1_1;
    GX_MULTI_LINE_TEXT_VIEW Menu_Screen_accordion_menu_1_1_1;
    GX_MENU Menu_Screen_accordion_menu_1_2;
    GX_PROMPT Menu_Screen_accordion_menu_1_2_1;
    GX_MENU Menu_Screen_accordion_menu_2;
    GX_MENU Menu_Screen_accordion_menu_2_1;
    GX_MENU Menu_Screen_accordion_menu_2_2;
    GX_MENU Menu_Screen_accordion_menu_3;
    GX_PROMPT Menu_Screen_accordion_menu_3_1;
    GX_PROMPT Menu_Screen_accordion_menu_3_2;
    GX_ICON Menu_Screen_accordion_menu_3_icon;
    GX_TREE_VIEW Menu_Screen_tree_view;
    GX_MENU Menu_Screen_tree_menu;
    GX_PROMPT Menu_Screen_tree_menu_1;
    GX_PROMPT Menu_Screen_tree_menu_1_1;
    GX_ICON Menu_Screen_icon;
    GX_MENU Menu_Screen_tree_menu_2;
    GX_MENU Menu_Screen_tree_menu_2_1;
    GX_PROMPT Menu_Screen_tree_menu_2_1_1;
    GX_PROMPT Menu_Screen_tree_menu_2_1_2;
    GX_ICON Menu_Screen_icon_1;
    GX_MENU Menu_Screen_tree_menu_2_2;
    GX_MENU Menu_Screen_tree_menu_2_2_1;
    GX_PROMPT Menu_Screen_tree_menu_2_2_1_1;
    GX_PROMPT Menu_Screen_tree_menu_2_2_1_2;
    GX_ICON Menu_Screen_icon_2;
    GX_ICON Menu_Screen_icon_3;
    GX_ICON Menu_Screen_icon_4;
    GX_MENU Menu_Screen_tree_menu_3;
    GX_PROMPT Menu_Screen_tree_menu_3_1;
    GX_ICON Menu_Screen_icon_5;
    GX_SCROLLBAR Menu_Screen_tree_hscroll;
    GX_SCROLLBAR Menu_Screen_tree_vscroll;
    GX_MENU Menu_Screen_tree_menu_4;
    GX_PROMPT Menu_Screen_prompt_10;
    GX_ICON Menu_Screen_icon_6;
    GX_PROMPT Menu_Screen_prompt;
    GX_PROMPT Menu_Screen_prompt_1;
} MENU_SCREEN_CONTROL_BLOCK;

typedef struct SCROLL_WHEEL_SCREEN_CONTROL_BLOCK_STRUCT
{
    BASE_SCREEN_CONTROL_BLOCK base;
    GX_NUMERIC_SCROLL_WHEEL Scroll_Wheel_Screen_scroll_wheel_day;
    GX_NUMERIC_SCROLL_WHEEL Scroll_Wheel_Screen_scroll_wheel_year;
    GX_STRING_SCROLL_WHEEL Scroll_Wheel_Screen_scroll_wheel_month;
    GX_PROMPT Scroll_Wheel_Screen_Prompt_Circular_Gauges_1;
} SCROLL_WHEEL_SCREEN_CONTROL_BLOCK;


/* extern statically defined control blocks                                    */

#ifndef GUIX_STUDIO_GENERATED_FILE
extern MENU_SCREEN_CONTROL_BLOCK Menu_Screen;
extern SCROLL_WHEEL_SCREEN_CONTROL_BLOCK Scroll_Wheel_Screen;
extern BASE_SCREEN_CONTROL_BLOCK Base_Screen;
extern GAUGE_SCREEN_CONTROL_BLOCK Gauge_Screen;
extern WINDOW_SCREEN_CONTROL_BLOCK Window_Screen;
extern SLIDER_SCREEN_CONTROL_BLOCK Slider_Screen;
extern TEXT_SCREEN_CONTROL_BLOCK Text_Screen;
extern BUTTON_SCREEN_CONTROL_BLOCK Button_Screen;
#endif

/* Declare event process functions, draw functions, and callback functions     */

UINT scroll_wheel_screen_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT day_wheel_value_format(GX_NUMERIC_SCROLL_WHEEL *wheel, INT row, GX_STRING *string);
VOID custom_next_button_draw(GX_PIXELMAP_BUTTON *widget);
UINT gauge_window_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT window_screen_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr);
VOID nested_parent_window_draw(GX_WINDOW *widget);
UINT scroll_frame_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr);
VOID scroll_frame_draw(GX_WINDOW *widget);
VOID custom_text_button_draw(GX_TEXT_BUTTON *widget);
VOID custom_icon_button_draw(GX_ICON_BUTTON *widget);
VOID drop_list_row_create(GX_VERTICAL_LIST *, GX_WIDGET *, INT);
UINT slider_screen_event_process(GX_WINDOW *widget, GX_EVENT *event_ptr);
UINT text_screen_event_handler(GX_WINDOW *widget, GX_EVENT *event_ptr);
VOID custom_transparent_prompt_thick_border_draw(GX_PROMPT *widget);
VOID numeric_prompt_format_func(GX_NUMERIC_PROMPT *, INT);
VOID numeric_pixelmap_prompt_format_func(GX_NUMERIC_PIXELMAP_PROMPT *, INT);
VOID custom_button_draw(GX_BUTTON *widget);
VOID custom_multi_line_text_button_draw(GX_MULTI_LINE_TEXT_BUTTON *widget);

/* Declare the GX_STUDIO_DISPLAY_INFO structure                                */


typedef struct GX_STUDIO_DISPLAY_INFO_STRUCT 
{
    GX_CONST GX_CHAR *name;
    GX_CONST GX_CHAR *canvas_name;
    GX_CONST GX_THEME **theme_table;
    GX_CONST GX_STRING **language_table;
    USHORT   theme_table_size;
    USHORT   language_table_size;
    UINT     string_table_size;
    UINT     x_resolution;
    UINT     y_resolution;
    GX_DISPLAY *display;
    GX_CANVAS  *canvas;
    GX_WINDOW_ROOT *root_window;
    GX_COLOR   *canvas_memory;
    ULONG      canvas_memory_size;
    USHORT     rotation_angle;
} GX_STUDIO_DISPLAY_INFO;


/* Declare Studio-generated functions for creating top-level widgets           */

UINT gx_studio_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_text_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_multi_line_text_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_checkbox_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_radio_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_icon_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_pixelmap_button_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_icon_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_pixelmap_slider_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_progress_bar_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_numeric_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_pixelmap_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_numeric_pixelmap_prompt_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_window_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_vertical_list_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_horizontal_list_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_drop_list_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_string_scroll_wheel_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_numeric_scroll_wheel_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_text_input_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_multi_line_text_view_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_multi_line_text_input_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_horizontal_scrollbar_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_vertical_scrollbar_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_circular_gauge_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_template_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_menu_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_accordion_menu_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_tree_view_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
UINT gx_studio_rich_text_view_create(GX_CONST GX_STUDIO_WIDGET *info, GX_WIDGET *control_block, GX_WIDGET *parent);
GX_WIDGET *gx_studio_widget_create(GX_BYTE *storage, GX_CONST GX_STUDIO_WIDGET *definition, GX_WIDGET *parent);
UINT gx_studio_named_widget_create(char *name, GX_WIDGET *parent, GX_WIDGET **new_widget);
UINT gx_studio_display_configure(USHORT display, UINT (*driver)(GX_DISPLAY *), GX_UBYTE language, USHORT theme, GX_WINDOW_ROOT **return_root);
UINT gx_studio_auto_event_handler(GX_WIDGET *widget, GX_EVENT *event_ptr, GX_CONST GX_STUDIO_EVENT_PROCESS *record);

/* Determine if a C++ compiler is being used.  If so, complete the standard
  C conditional started above.                                                 */
#ifdef __cplusplus
}
#endif

#endif                                       /* sentry                         */
