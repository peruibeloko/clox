find := C:\msys64\usr\bin\find
src_dir := src
out_dir := out
src_files := $(shell $(find) $(src_dir) -name "*.c")

clox: $(src_files) | $(out_dir)
	gcc -g -o $(out_dir)/$@.exe $(src_files)

$(out_dir):
	mkdir $@

clean:
	-rd /q /s out