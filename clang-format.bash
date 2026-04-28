#! /usr/bin/env bash

shopt -s globstar

files=("${0%/*}"/src/**/*.{cc,hh})
if [ -n "${GITHUB_ACTIONS+.}" ]
then
    clang-format --dry-run --verbose --Werror "${files[@]}"
else
    clang-format -i --verbose "${files[@]}"
fi
