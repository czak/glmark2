package:
	mkdir -p pkg
	cp -r res/* pkg/
	cp -r ../data pkg/
	cp ../build/src/glmark2-es2-webos pkg/
	ares-package pkg

install: package
	ares-install com.github.glmark2.glmark2_0.0.1_arm.ipk

.PHONY: clean
clean:
	rm -rf pkg *.ipk
