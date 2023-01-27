set(LibraryVersion "1.8")
add_definitions(-DLIBRARY_VERSION="${LibraryVersion}")

set(deps detector raw)

COMPILELIB(deps)