#!/usr/bin/env bash

#requires installation of expect tool for interactivity of prompts

mutex_src="sync_mutex_pattern.c"
sem_src="sync_semaphore_limiter.c"

mutex_file="./mutex_demo"
sem_file="./sem_demo"

build() {
    local src="$1"
    local out="$2"
    #first argument is source file, second is output binary, last is for linking pthread library
    clang "$src" -o "$out" "-pthread"
}

run_sem_expect() {
    local threads="$1"
    local limit="$2"

    expect <<'EOF'
set timeout -1
set threads $env(THREADS)
set limit   $env(LIMIT)

spawn ./sem_demo

expect -re "Provide a number for the total number of threads created.*:\r?\n"
send -- "$threads\r"

expect -re "Provide a number for the limit to the threads allowed for concurrent accesses to a resource.*:\r?\n"
send -- "$limit\r"

expect eof
EOF
}

#build from the start
build "$mutex_src" "${mutex_file}"
build "$sem_src"   "${sem_file}"

while true; do
    echo "1) Run demo 1 (mutex)"
    echo "2) Run demo 2 (semaphore interactive)"
    echo "3) Run both demos (semaphore interactive, automated via expect)"
    echo "4) Rebuild both demos"
    echo "5) Quit"
    read -rp "Choose: " choice

    case "$choice" in
        1)  "$mutex_file" ;;
        2)  "$sem_file";;
        3)  echo "First, provide parameters for the semaphore demo."
            read -rp "Provide a number for the total number of threads created: " threads
            read -rp "Provide a number for the limit to the threads allowed for concurrent accesses to a resource: " limit
            echo ">>> Running mutex demo"
            "$mutex_file"
            echo -e "\n>>> Running semaphore demo (expect)"
            THREADS="$threads" LIMIT="$limit" run_sem_expect "$threads" "$limit" ;;
        4)  build "$mutex_src" "${mutex_file}"
            build "$sem_src"   "${sem_file}"
            echo "Rebuilt both demos." ;;
        5)  exit 0 ;;
        *)  echo "Invalid choice." ;;
    esac
done
