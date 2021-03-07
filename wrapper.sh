#! /usr/bin/env bash

set -o errexit
set -o nounset
set -o pipefail

function _log()
{
  local type=$1
  local message=$2
  local date=$(date +"%Y-%m-%d %H:%M:%S")
  echo "[${date}] [${type}] ${message}"
}

function log_info()
{
  local message=$1
  _log "INFO" "${message}"
}

function log_error()
{
  local message=$1
  _log "ERROR" "${message}"
}

function main()
{
  local bin=solution

  log_info "Compiling..."
  if ! make ; then
    log_error "ERROR: Failed to compile file."
    exit 1
  fi

  local test_dir=tests
  rm -rf ${test_dir}
  mkdir -p ${test_dir}
  local examples_cnt=1000
  log_info "Generating tests (examples for each test=[${examples_cnt}])..."
  if ! python3 generator.py ${test_dir} ${examples_cnt} ; then
    log_error "ERROR: Failed to python generate tests."
    return 1
  fi

  for test_file in $(ls ${test_dir}/*.t); do
    log_info "Executing ${test_file}..."
    if ! ./${bin} < ${test_file} > last_answer ; then
        log_error "ERROR"
        continue
    fi
    local answer_file="${test_file%.*}"

    if ! diff -u "${answer_file}.a" last_answer ; then
        echo "Failed"
    else
        echo "OK"
    fi
  done

  make clean
}

main $@