
set(sources
	${sources}
	"${CMAKE_CURRENT_SOURCE_DIR}/main.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/threads/threads.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/threads/threads.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/threads/threads.S"
)

set(sources ${sources} PARENT_SCOPE)
