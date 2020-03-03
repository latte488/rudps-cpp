
set fenc=utf-8
set number
set autoread
set expandtab
set tabstop=4
set shiftwidth=4
set softtabstop=4
set autoindent
set smartindent

let g:ale_lint_on_text_changed = 'always'

let g:ale_c_gcc_options = "-W -Wall -Iinclude"
let g:ale_cpp_gcc_options = "-std=c++17 -W -Wall"

