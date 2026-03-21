import os
import re

# ================= 配置区 =================
# 按顺序指定头文件，Defs.h 必须排在第一位，以解决依赖问题
HEADER_ORDER = [
    "Defs.h",
    "Warrior.h",
    "Headquarter.h",
    "Game.h"
]

# 按顺序指定实现文件
SRC_ORDER = [
    "Warrior.cpp",
    "Headquarter.cpp",
    "Game.cpp"
]

OUTPUT_FILE = "submit.cpp"
# ==========================================

def merge():
    system_includes = set()
    code_body = []

    def process_file(file_path):
        if not os.path.exists(file_path):
            print(f"警告：找不到文件 {file_path}")
            return
        
        with open(file_path, 'r', encoding='utf-8') as f:
            lines = f.readlines()
            
        code_body.append(f"\n/* --- Start of {os.path.basename(file_path)} --- */\n")
        
        for line in lines:
            # 1. 提取系统头文件
            if re.match(r'^\s*#include\s*<.*>', line):
                system_includes.add(line.strip())
            # 2. 跳过本地头文件引用和 pragma once
            elif re.match(r'^\s*#include\s*".*"', line) or "#pragma once" in line:
                continue
            # 3. 保留其他所有内容（包括 inline, 前置声明等）
            else:
                code_body.append(line)

    # 第一步：按顺序处理头文件
    for h in HEADER_ORDER:
        process_file(os.path.join("include", h))

    # 第二步：按顺序处理实现文件
    for cpp in SRC_ORDER:
        process_file(os.path.join("src", cpp))

    # 第三步：处理 main.cpp
    process_file("main.cpp")

    # 组合最终代码
    headers = sorted(list(system_includes))
    # 保证 iostream 和 string 等在最上面
    full_code = "// Combined Source Code for OpenJudge\n"
    full_code += "\n".join(headers) + "\n\n"
    full_code += "".join(code_body)

    with open(OUTPUT_FILE, 'w', encoding='utf-8') as f:
        f.write(full_code)

if __name__ == "__main__":
    merge()
    print(f"成功！已按依赖顺序合并至 {OUTPUT_FILE}")
