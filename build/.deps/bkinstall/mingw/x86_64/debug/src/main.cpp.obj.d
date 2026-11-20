{
    depfiles_format = "gcc",
    files = {
        [[src\main.cpp]]
    },
    values = {
        [[C:\msys64\mingw64\bin\x86_64-w64-mingw32-g++]],
        {
            "-m64",
            "-g",
            "-O0",
            "-std=c++17",
            "-Iinclude",
            [[-Iexternal\fltk]],
            [[-Iexternal\fltk\build]]
        }
    },
    depfiles = "main.o: src\\main.cpp external\\fltk/zlib/zlib.h external\\fltk/zlib/zconf.h  include/bkres.hpp external\\fltk/FL/Fl.H  external\\fltk\\build/FL/fl_config.h external\\fltk/FL/Fl_Export.H  external\\fltk/FL/platform_types.h external\\fltk/FL/fl_casts.H  external\\fltk/FL/core/function_types.H external\\fltk/FL/Enumerations.H  external\\fltk/FL/Fl_Export.H external\\fltk/FL/fl_types.h  external\\fltk/FL/fl_attr.h external\\fltk/FL/core/events.H  external\\fltk/FL/core/options.H external\\fltk/FL/core/pen_events.H  external\\fltk/FL/Fl_Widget_Tracker.H external\\fltk/FL/fl_utf8.h  external\\fltk/FL/Enumerations.H external\\fltk/FL/Fl_Window.H  external\\fltk/FL/Fl_Group.H external\\fltk/FL/Fl_Widget.H  external\\fltk/FL/Fl.H external\\fltk/FL/Fl_Bitmap.H  external\\fltk/FL/Fl_Image.H external\\fltk/FL/Fl_Button.H  external\\fltk/FL/fl_ask.H external\\fltk/FL/fl_attr.h  external\\fltk/FL/Fl_Box.H external\\fltk/FL/Fl_PNG_Image.H  external\\fltk/FL/Fl_Plugin.H external\\fltk/FL/Fl_Preferences.H  external\\fltk/FL/Fl_Text_Buffer.H external\\fltk/FL/Fl_Text_Display.H  external\\fltk/FL/fl_draw.H external\\fltk/FL/Fl_Graphics_Driver.H  external\\fltk/FL/Fl_Device.H external\\fltk/FL/Fl_Image.H  external\\fltk/FL/Fl_Pixmap.H external\\fltk/FL/Fl_RGB_Image.H  external\\fltk/FL/Fl_Rect.H external\\fltk/FL/Fl_Widget.H  external\\fltk/FL/Fl_Group.H external\\fltk/FL/Fl_Scrollbar.H  external\\fltk/FL/Fl_Slider.H external\\fltk/FL/Fl_Valuator.H  external\\fltk/FL/Fl_Text_Buffer.H external\\fltk/FL/Fl_Check_Button.H  external\\fltk/FL/Fl_Light_Button.H external\\fltk/FL/Fl_Button.H\
"
}