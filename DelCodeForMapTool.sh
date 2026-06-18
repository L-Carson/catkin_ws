#!/bin/bash

source ./PackageList.sh

packLstStr=$(GetPackListForMapTool)

DelExistPacksExcept "$packLstStr"

exit 0
