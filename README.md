An attempt to replicate Apple's PNG Mangler.

Apple's PNG files are nowhere near valid (extra critical chunk with an unknown format, zlib
headers discarded, premultiplied alpha). "Think different" and all that.

# Prerequisites
* a compiler
	* (for iPhone compilation) Apple's Xcode or the open toolchain

# Compiling
* iPhone (default target)

		make
* Native (linux, OS X)

		make target=native
