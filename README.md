# Razor

## Build Instructions

Razor is built using premake in order to get it to run therefore to build the project please download premake and extract the file to this directory you will create in $(RepoRoot)/vendor/bin/premake.

You can download premake from here https://premake.github.io/download/

After this you will be able to run the GenerateProjectFiles.bat included in the root of the repo and this will build the .sln files for you.

All libraries required for this project are included as submodules in Razor/vendor.

## Useful Git command if new submodule prevents switching to branch
branch=<whatever branch you're switching to> && rm -rf <eg. git/modules/Razor/vendor/Coral Razor/vendor/Coral whatever submodule it's complaing about> && git checkout "$branch" && git submodule sync && git submodule update --init --recursive
