#!/bin/bash
export LD_PRELOAD="$(pwd)/toralize.so"
${@}
unset LD_PRELOAD
