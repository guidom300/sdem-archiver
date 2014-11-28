#!/usr/bin/env bash

alias find_project_files="find -s . -type f -name '*.pro'"
alias make_xcodeproj="qmake -spec macx-xcode"

generate_xcode_projects() {
    for pro in $(find_project_files -mindepth 2); do
        (cd "$(dirname "$pro")" && make_xcodeproj "$(basename "$pro")")
    done

    make_xcodeproj "$(find_project_files -depth 1)"
}

rm_xcode_projects() {
    find . -type d -name '*.xcodeproj' | xargs rm -rf
}

alias format_code="clang-format -i \$(find . -type f -name '*.h' -o -name '*.hpp' -o -name '*.cc' -o -name '*.cpp')"

which_gnu_sed() {
    if [ "$(uname)" = Linux ]; then
        which sed || exit 1
    else
        which gsed || exit 1
    fi
}

scaffold_test() {
    local name="$(echo $1 | tr '[:upper:]' '[:lower:]')"
    local template="template"
    local sed=$(which_gnu_sed)

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

    $sed "s/$template/$name/" "$pro_src" > "$pro_dst"
    $sed "s/$template/$name/" "$cc_src" > "$cc_dst"

    capitalize_words() {
        echo "$1" | $sed 's/[^ _-]*/\u&/g' | tr -d ' _-'
    }
    $sed "s/$(capitalize_words $template)/$(capitalize_words $name)/" "$cc_dst" -i
}

remove_trailing_whitespaces() {
    $(which_gnu_sed) -i 's/[[:space:]]*$//' "$1"
}

update_project() {
    local app_pro=app/app.pro
    local sed=$(which_gnu_sed)

    # Cleanup the project
    [ -f Makefile ] && make distclean

    # find all headers and update the project file
    $sed "/^    /d" $app_pro | $sed "/^HEADERS/a\\$(find app -type f -name '*.h' -o -name '*.hpp' | xargs basename | sort | $sed 's/^/    /' | $sed 's/$/ \\/')" > $app_pro
    remove_trailing_whitespaces $app_pro

    # update xcode project files
    qmake
    rm_xcode_projects
    generate_xcode_projects
}
