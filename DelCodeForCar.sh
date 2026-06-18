#!/bin/bash

source ./PackageList.sh

packLstStr=$(GetPackListForCar)

DelExistPacksExcept "$packLstStr"

exit 0
