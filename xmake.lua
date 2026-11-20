add_rules("mode.debug", "mode.release")

target("bkinstall") do
    set_kind("binary")
    set_languages("cxx17")
    add_files("src/*.cpp")
    add_includedirs("include")

    add_includedirs("external/fltk")
    add_includedirs("external/fltk/build")
    add_linkdirs("external/fltk/build/lib")
    add_links("fltk_images", "fltk_png", "fltk_z", "fltk_jpeg", "fltk_gl", "fltk")

    if is_plat("mingw", "windows") then
        add_syslinks(
            "winspool",
            "gdiplus",
            "gdi32",
            "user32",
            "comctl32",
            "comdlg32",
            "ole32",
            "uuid",
            "ws2_32",
            "uxtheme",
            "imm32",
            "winmm",
            "msimg32"
        )
    end

    if is_plat("mingw") then
        add_ldflags("-static-libgcc", "-static-libstdc++", {force = true})
    end
end