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

scaffold_test() {
    local name="$(echo $1 | tr '[:upper:]' '[:lower:]')"
    local template="template"

    if [ -z "$name" -o "$name" = "$template" ]; then
        echo "Usage: $0 test_name"
        echo "'test_name' cannot be '$template'"
        exit 1
    fi

    local src_dir="tests/$template"
    local dst_dir="tests/$name"
    mkdir "$dst_dir"

    local pro_src="$src_dir/${template}.pro"
    local pro_dst="$dst_dir/${name}.pro"
    local cc_src="$src_dir/${template}_test.cc"
    local cc_dst="$dst_dir/${name}_test.cc"

    local sed
    if [ "$(uname)" = Linux ]; then
        sed='sed'
    else
        sed='gsed'
    fi

    $sed "s/$template/$name/" "$pro_src" > "$pro_dst"
    $sed "s/$template/$name/" "$cc_src" > "$cc_dst"

    capitalize_words() {
        echo "$1" | $sed 's/[^ _-]*/\u&/g' | tr -d ' _-'
    }
    $sed "s/$(capitalize_words $template)/$(capitalize_words $name)/" "$cc_dst" -i
}
