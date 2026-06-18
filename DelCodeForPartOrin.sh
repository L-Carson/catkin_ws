#!/bin/bash

source ./PackageList.sh

packLstStr=$(GetPackListForPartOrin)

DelExistPacksExcept "$packLstStr"

exit 0
