import os
import re
import zlib

def sanitize_var_name(relative_path):
    sanitized = re.sub(r"[\\/.\- ]", "_", relative_path)
    sanitized = re.sub(r"^resource_", "bk_res_", sanitized)
    return sanitized.lower()


def pack_resources(source_dir):
    if not os.path.exists(source_dir):
        print(f"Error: Source directory '{source_dir}' does not exist.")
        return

    all_assets = []

    for root, _, files in os.walk(source_dir):
        for file in files:
            if file.startswith("."):
                continue

            full_path = os.path.join(root, file)
            rel_path = os.path.relpath(full_path, source_dir).replace("\\", "/")
            var_name = sanitize_var_name(os.path.join(source_dir, rel_path))

            with open(full_path, "rb") as f:
                raw_data = f.read()

            compressed_data = zlib.compress(raw_data, level=9)
            all_assets.append(
                {
                    "rel_path": rel_path,
                    "var_name": var_name,
                    "raw_len": len(raw_data),
                    "comp_data": compressed_data,
                }
            )

    header_lines = [
        "#ifndef BKRES_H",
        "#define BKRES_H",
        "",
    ]

    for asset in all_assets:
        header_lines.append(f"extern const unsigned int {asset['var_name']}_size;")
        header_lines.append(f"extern const unsigned int {asset['var_name']}_rsize;")
        header_lines.append(f"extern const unsigned char {asset['var_name']}[];\n")

    header_lines.extend(["#endif"])

    with open("include/bkres.hpp", "w", encoding="utf-8") as f:
        f.write("\n".join(header_lines))

    source_lines = [f'#include "bkres.hpp"', ""]

    for asset in all_assets:
        hex_rows = []
        comp_bytes = asset["comp_data"]
        for i in range(0, len(comp_bytes), 12):
            chunk = comp_bytes[i : i + 12]
            hex_row = ", ".join(f"0x{b:02X}" for b in chunk)
            hex_rows.append(f"    {hex_row}")

        formatted_hex = ",\n".join(hex_rows)

        source_lines.append(f"const unsigned int {asset['var_name']}_size = {len(comp_bytes)};")
        source_lines.append(f"const unsigned int {asset['var_name']}_rsize = {asset['raw_len']};")
        source_lines.append(f"const unsigned char {asset['var_name']}[] = {{")
        source_lines.append(formatted_hex)
        source_lines.append("};\n")

    with open("src/bkres.cpp", "w", encoding="utf-8") as f:
        f.write("\n".join(source_lines))

    print(f"successfully packed {len(all_assets)} resources")


if __name__ == "__main__":
    pack_resources("resource")