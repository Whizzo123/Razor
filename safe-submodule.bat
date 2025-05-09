#!/bin/bash

# Usage:
# ./safe-submodule.sh add <repo-url> <path>
# ./safe-submodule.sh switch <branch-name>

set -e

SUBMODULES_FILE=".gitmodules"

add_submodule() {
  REPO_URL=$1
  PATH_TO_SUBMODULE=$2

  if [ -z "$REPO_URL" ] || [ -z "$PATH_TO_SUBMODULE" ]; then
    echo "Usage: $0 add <repo-url> <path>"
    exit 1
  fi

  echo "Adding submodule $REPO_URL at $PATH_TO_SUBMODULE"
  git submodule add "$REPO_URL" "$PATH_TO_SUBMODULE"
  git submodule update --init --recursive
  git add "$SUBMODULES_FILE" "$PATH_TO_SUBMODULE"
  git commit -m "Add submodule $REPO_URL at $PATH_TO_SUBMODULE"
  echo "Submodule added and committed."
}

safe_switch() {
  BRANCH=$1

  if [ -z "$BRANCH" ]; then
    echo "Usage: $0 switch <branch-name>"
    exit 1
  fi

  echo "Deinitializing all submodules to avoid dirty state..."
  git submodule deinit -f .
  git clean -fd
  git checkout "$BRANCH"
  echo "Reinitializing submodules for $BRANCH"
  git submodule update --init --recursive
  echo "Switched to $BRANCH and submodules updated."
}

# Main
COMMAND=$1
shift

case "$COMMAND" in
  add)
    add_submodule "$@"
    ;;
  switch)
    safe_switch "$@"
    ;;
  *)
    echo "Usage: $0 <add|switch> ..."
    ;;
esac