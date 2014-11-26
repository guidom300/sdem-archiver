#!/usr/bin/env bash

alias find_project_files="find -s . -type f -name '*.pro'"
alias make_xcodeproj="qmake -spec macx-xcode"

generate_xcode_projects() {
    for pro in $(find_project_files -mindepth 2); do
        (cd "$(dirname "$pro")" && make_xcodeproj "$(basename "$pro")")
    done

    make_xcodeproj "$(find_project_files -depth 1)"
}

alias find_xcodeproj="find . -type d -name '*.xcodeproj'"

rm_xcode_projects() {
    rm -rv $(find_xcodeproj)
}

alias format_code="clang-format -i \$(find . -type f -name '*.h' -o -name '*.hpp' -o -name '*.cc' -o -name '*.cpp')"
