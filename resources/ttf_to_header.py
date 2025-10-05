import os

ttf_path = "NotoSansSymbols2-Regular.ttf"
file_name = "NOTO_SYMBOLS"
array_name = "font_data_symbol"
header_path = file_name + ".h"



with open(ttf_path, 'rb') as f:
        font_data = f.read()
    
if array_name is None:
    base_name = os.path.splitext(os.path.basename(ttf_path))[0]
    array_name = ''.join(c if c.isalnum() or c == '_' else '_' for c in base_name)
    array_name = f"font_data_{array_name.lower()}"

with open(header_path, 'w') as f:
    f.write(f"#ifndef {file_name}_H\n")
    f.write(f"#define {file_name}_H\n\n")
    f.write(f"static const unsigned char {array_name}[] = {{\n")
    
    bytes_per_line = 16
    for i in range(0, len(font_data), bytes_per_line):
        chunk = font_data[i:i + bytes_per_line]
        hex_values = ', '.join(f'0x{b:02x}' for b in chunk)
        f.write(f"    {hex_values}")
        
        if i + bytes_per_line < len(font_data):
            f.write(',')
        
        f.write('\n')
    
    f.write("};\n\n")
    f.write(f"#endif // {file_name}\n")