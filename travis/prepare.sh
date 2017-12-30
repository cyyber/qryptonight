#!/usr/bin/env bash

echo "TRAVIS_OS_NAME" ${TRAVIS_OS_NAME}
echo "PLATFORM" ${PLATFORM}

case "${TRAVIS_OS_NAME}" in
    osx)
        echo "OSX PREPARE"
        brew install python3 swig hwloc
        brew outdated cmake || brew upgrade cmake
        sudo pip3 install -U pip setuptools
        ;;

    linux)
        echo "LINUX PREPARE " ${PLATFORM}
        ;;
    *)
        echo "UNSUPPORTED OS"
        exit 1
        ;;
esac
