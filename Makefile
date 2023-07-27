.PHONY: all bench-runner coremark rv8-bench eyrie-rt

all: bench-runner coremark rv8-bench eyrie-rt
	grep -rl "rdcycle" . | xargs sed -i 's/rdcycle/rdtime/g'
	rm -rf staging
	./copy_all_tests.sh

coremark:
	make -C ./coremark/ CC=riscv64-unknown-linux-gnu-gcc PORT_DIR=rv64 compile

rv8-bench:
	make -C ./rv8-bench/

iozone:
	make -C ./iozone/ riscv_musl

bench-runner:
	make -C ./bench-runner/

eyrie-rt:
	cp ${KEYSTONE_DIR}/build/examples/hello/eyrie-rt .
#$(MAKE) -C ${KEYSTONE_DIR}/runtime eyrie-rt #-DFREEMEM=ON -DUNTRUSTED_IO_SYSCALL=ON -DENV_SETUP=ON -DLINUX_SYSCALL=ON

clean:
	make -C ./rv8-bench clean
	make -C ./coremark clean
	make -C ./bench-runner clean
	rm -r staging
