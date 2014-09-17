test_motu_class:
	gcc -o test_motu_class -I./ -I../portaudio/include -Werror -g -O2 -DPA_LITTLE_ENDIAN -isysroot /Developer/SDKs/MacOSX10.5.sdk -mmacosx-version-min=10.3 -DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DSTDC_HEADERS=1 -DHAVE_SYS_TYPES_H=1 -DHAVE_SYS_STAT_H=1 -DHAVE_STDLIB_H=1 -DHAVE_STRING_H=1 -DHAVE_MEMORY_H=1 -DHAVE_STRINGS_H=1 -DHAVE_INTTYPES_H=1 -DHAVE_STDINT_H=1 -DHAVE_UNISTD_H=1 -DHAVE_DLFCN_H=1 -DSIZEOF_SHORT=2 -DSIZEOF_INT=4 -DSIZEOF_LONG=4 -DHAVE_NANOSLEEP=1 -DPA_USE_COREAUDIO=1 test_motu_class.c motu.c -framework CoreAudio -framework AudioToolbox -framework AudioUnit -framework Carbon ../portaudio/lib/.libs/libportaudio.a -lpthread

clean: 
	rm test_motu_class
