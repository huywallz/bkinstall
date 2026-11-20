#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

#include <windows.h>

#include <zlib/zlib.h>
#include "bkres.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Check_Button.H>

#include <cpr/cpr.h>

struct {
    Fl_Window* win;
    int mode = 0;
    bool dirty = true;
    Fl_Button* next;
    Fl_Button* back;
} bk;

static void bk_cancel_click(Fl_Widget* w, void* data) {
    bk.win->hide();
}

static void bk_next_click(Fl_Widget* w, void* data) {
    bk.mode++;
    bk.dirty = true;

    bk.back->activate();
}

static void bk_back_click(Fl_Widget* w, void* data) {
    bk.mode--;
    if (bk.mode < 0) bk.mode = 0;
    bk.dirty = true;

    if (bk.mode == 0) {
        w->deactivate();
    }
}

static void bk_checkbox(Fl_Widget* w, void* data) {
    bk.dirty = true;
}

static void bk_center_window(Fl_Window* win) {
    int sx = 0, sy = 0, sw = 0, sh = 0;
    Fl::screen_xywh(sx, sy, sw, sh);

    int x = sx + (sw - win->w()) / 2;
    int y = sy + (sh - win->h()) / 2;
    win->position(x, y);
}

static std::string bk_exe_folder() {
    char buf[MAX_PATH] = {0};
    
    DWORD len = GetModuleFileNameA(nullptr, buf, MAX_PATH);
    if (len == 0 || len >= MAX_PATH) return ".";

    std::string p(buf, len);
    size_t slash = p.find_last_of("\\/");
    return (slash == std::string::npos) ? "." : p.substr(0, slash);
}

std::vector<uint8_t> bk_decompress(const uint8_t* src, uLong src_len, uLong raw_len) {
    std::vector<uint8_t> out(raw_len);
    uLongf dest_len = raw_len;

    fltk_z_uncompress(out.data(), &dest_len, src, src_len);
    
    out.resize(dest_len);
    return out;
}

int main(int argc, char** argv) {
    const int padding = 25 / 2;

    Fl_Image::RGB_scaling(FL_RGB_SCALING_BILINEAR);

    auto icon_data = bk_decompress(bk_res_icon_png, bk_res_icon_png_size, bk_res_icon_png_rsize);
    Fl_PNG_Image* icon_png = new Fl_PNG_Image("icon.png", icon_data.data(), icon_data.size());
    icon_data.clear();

    auto bk_third_party_licenses_txt = bk_decompress(bk_res_bk_third_party_licenses_txt, bk_res_bk_third_party_licenses_txt_size, bk_res_bk_third_party_licenses_txt_rsize);
    std::string bk_tpl(bk_third_party_licenses_txt.begin(), bk_third_party_licenses_txt.end());
    bk_third_party_licenses_txt.clear();

    auto bkinstall_third_party_licenses_txt = bk_decompress(bk_res_bkinstall_third_party_licenses_txt, bk_res_bkinstall_third_party_licenses_txt_size, bk_res_bkinstall_third_party_licenses_txt_rsize);
    std::string bkinstall_tpl(bkinstall_third_party_licenses_txt.begin(), bkinstall_third_party_licenses_txt.end());
    bkinstall_third_party_licenses_txt.clear();

    Fl_Window* win = new Fl_Window(0, 0, 480, 360, "bkinstall");
    bk_center_window(win);
    bk.win = win;

    Fl_Box* top = new Fl_Box(0, 0, win->w(), 50 + padding);
    top->box(FL_FLAT_BOX);
    top->color(fl_rgb_color(255, 255, 255));

    Fl_Box* icon = new Fl_Box(top->w() - top->h(), padding / 2, 0, 0);
    icon->size(top->h() - padding, top->h() - padding);
    Fl_Image* icon_img = icon_png->copy(icon->w(), icon->h());
    icon->image(icon_img);
    icon->align(FL_ALIGN_INSIDE | FL_ALIGN_CENTER);

    Fl_Box* card = new Fl_Box(0, top->h(), win->w(), win->h() - top->h() - 50);
    card->box(FL_FLAT_BOX);
    card->color(fl_rgb_color(240, 240, 240));

    Fl_Box* title = new Fl_Box(padding, padding, win->w() - padding, 50);
    title->labelsize(16);
    title->labelfont(FL_BOLD);
    title->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);

    Fl_Box* desc = new Fl_Box(padding, title->y() + title->h() + padding, 0, 0);
    desc->align(FL_ALIGN_TOP_LEFT | FL_ALIGN_INSIDE);
    desc->size(win->w() - padding * 2, win->h() - desc->y() - 50 - padding);
    desc->label(
        "This will install bkboot and bklaunch on your computer.\n\n"
        "It is recommended that you close all other applications before continuing.\n\n"
        "Click Next to continue, or Cancel to exit Setup."
    );

    // EULA SECTION
    Fl_Text_Buffer* eula_buf = new Fl_Text_Buffer();
    std::string content;
    for (int i = 1; i <= 50; ++i) {
        content += "Line " + std::to_string(i) + ": Sample text inside a scrollable box.\n";
    }
    eula_buf->text(content.c_str());

    Fl_Text_Buffer* bk_tpl_buf = new Fl_Text_Buffer();
    bk_tpl_buf->text(bk_tpl.c_str());

    Fl_Text_Buffer* bkinstall_tpl_buf = new Fl_Text_Buffer();
    bkinstall_tpl_buf->text(bkinstall_tpl.c_str());

    Fl_Text_Display* disp = new Fl_Text_Display(desc->x(), desc->y(), desc->w(), desc->h() - padding / 2 - 25);
    disp->wrap_mode(Fl_Text_Display::WRAP_AT_BOUNDS, 0);
    disp->scrollbar_align(FL_ALIGN_RIGHT | FL_ALIGN_BOTTOM);

    Fl_Check_Button* cb = new Fl_Check_Button(padding, disp->y() + disp->h() + padding / 2, win->w() - padding * 2, 25, "I accept the terms in the License Agreement");
    cb->callback(bk_checkbox);

    Fl_Box* bottom = new Fl_Box(0, 0, win->w(), 50);
    bottom->position(0, win->h() - bottom->h());
    bottom->box(FL_FLAT_BOX);
    bottom->color(fl_rgb_color(240, 240, 240));
    
    Fl_Box* line = new Fl_Box(0, bottom->y(), win->w(), 1);
    line->color(fl_rgb_color(200, 200, 200));
    line->box(FL_FLAT_BOX);

    Fl_Button* cancel = new Fl_Button(0, 0, 70, 25, "Cancel");
    cancel->position(bottom->x() + bottom->w() - cancel->w() - padding, bottom->y() + bottom->h() / 2 - cancel->h() / 2);
    cancel->callback(bk_cancel_click);

    Fl_Button* next = new Fl_Button(0, 0, 70, 25, "Next");
    next->position(bottom->x() + bottom->w() - (next->w() + padding) * 2 - padding, bottom->y() + bottom->h() / 2 - next->h() / 2);
    next->callback(bk_next_click);
    bk.next = next;

    Fl_Button* back = new Fl_Button(0, 0, 70, 25, "Back");
    back->position(bottom->x() + bottom->w() - (back->w() + padding) * 3 - padding, bottom->y() + bottom->h() / 2 - back->h() / 2);
    back->callback(bk_back_click);
    back->deactivate();
    bk.back = back;

    win->end();
    win->show(argc, argv);
    
    while (Fl::first_window()) {
        Fl::wait(0);
        
        if (bk.dirty) {
            if (bk.mode != 0) {
                desc->hide();
            } else {
                title->label("Welcome to the Brickate Installer");
                desc->show();
                next->activate();
            }

            if (bk.mode != 1) {
                disp->hide();
                cb->hide();
            } else {
                title->label("End-User License Agreement");
                disp->show();
                cb->show();
                
                if (!cb->value()) {
                    next->deactivate();
                } else {
                    next->activate();
                }

                disp->buffer(eula_buf);
            }

            if (bk.mode != 2) {
                
            } else {
                title->label("(bk) Third Party Licenses");
                disp->show();

                disp->buffer(bk_tpl_buf);
            }

            if (bk.mode != 3) {
                
            } else {
                title->label("(bkinstall) Third Party Licenses");
                disp->show();

                disp->buffer(bkinstall_tpl_buf);
            }

            bk.dirty = false;
        }
    }

    return 0;
}