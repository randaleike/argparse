set (VARG_BASE_DIR, ${MASTER_PROJECT_BASE_DIR}/varg)

set(vargBaseSrc
    ${VARG_BASE_DIR}/src/varg_intf.cpp
    ${VARG_BASE_DIR}/src/varg.cpp
    ${VARG_BASE_DIR}/src/vargstring.cpp
    ${VARG_BASE_DIR}/src/listvarg.cpp
    ${VARG_BASE_DIR}/src/varg_clib.cpp
    )

set(vargIncDir
    ${VARG_BASE_DIR}/inc
    ${VARG_BASE_DIR}/inc_clib
    )

set(vargMockIncDir
    ${vargIncDir}
    ${VARG_BASE_DIR}/mock
    )
