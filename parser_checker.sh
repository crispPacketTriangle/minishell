#!/usr/bin/env bash

# Good commands
# good_commands=(
#     'echo "Hello, World!"'
#     'ls -l /home/user'
#     'ls > outfile.txt'
#     'cat < infile.txt'
#     'grep "search_term" < infile.txt > outfile.txt'
#     'ls | grep "search_term"'
#     'cat file.txt | wc -l'
#     'mkdir new_dir && cd new_dir'
#     'touch file1 && echo "content" > file1'
#     'false || echo "This runs because the previous command failed"'
#     'true || echo "This won'\''t run because the previous command succeeded"'
#     '(cd /tmp && ls)'
#     '{ echo "Hello"; echo "World"; }'
#     'echo "This is a string with spaces"'
#     'echo '\''This is another string with spaces'\'''
#     'echo "Mixing '\''single'\'' and \\"double\\" quotes"'
#     'echo "Current date: $(date)"'
#     'echo "Files: $(ls)"'
#     'echo "Hello, World!" && ls -l | grep "txt"'
#     '(echo "Start" && ls) || echo "Failed"'
#     '{ echo "Block 1"; echo "Block 2"; } > blocks.txt'
#     'echo "$(date): File list" && ls > filelist.txt'
# )

# Bad commands
bad_commands=(
    'echo "This is an unmatched quote'
    'echo '\''This is another unmatched quote'
    'echo (unmatched parentheses'
    'echo unmatched parentheses)'
    'echo {unmatched braces'
    'echo unmatched braces}'
    'echo [unmatched brackets'
    'echo unmatched brackets]'
    # '| echo "Pipe at the beginning"'
    # 'echo "Pipe at the end" |'
    # 'echo "Double pipe" || echo "another command"'
    # '> outfile.txt echo "Redirection at the beginning"'
    # 'echo "Redirection at the end" >'
    # 'echo "Double redirection" >> >> outfile.txt'
    # '&& echo "Logical AND at the beginning"'
    # 'echo "Logical AND at the end" &&'
    # '|| echo "Logical OR at the beginning"'
    # 'echo "Logical OR at the end" ||'
    # '*'
    # 'echo *'
    # 'echo "Unquoted * should be fine" *'
    # 'echo * |'
    # 'echo | *'
    # 'echo "Mismatched quotes'\'' && ls'
    # '(echo "Missing closing parenthesis" || ls'
    # 'echo "Redirection in the middle" > | grep "error"'
    # 'echo "Logical AND at end" &&'
    # 'echo "Unmatched quote in command substitution $(date && echo '\''Done)'"'
)

# Function to run commands
run_commands() {
    local commands=("$@")
    for cmd in "${commands[@]}"; do
        echo "Running: ./pp $cmd"
        ./pp $cmd
    done
}

# Run good commands
echo "Running good commands:"
run_commands "${good_commands[@]}"

# Run bad commands
echo "Running bad commands:"
run_commands "${bad_commands[@]}"
