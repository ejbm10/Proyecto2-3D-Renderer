ifeq ($(KERNELRELEASE),)
# Directorio de encabezados del kernel
KERNELDIR := /lib/modules/$(shell uname -r)/build
# Directorio actual
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.mk .tmp_versions Module.symvers *.mod.c *.o *.ko .*.cmd \
		Module.markers modules.order *.a *.mod

load:
	sudo insmod driverp.ko || echo "Error: Verifica que el módulo no esté ya cargado"

unload:
	sudo rmmod driverp || echo "Error: Verifica que el módulo esté cargado"

else
# Archivo del módulo
obj-m := driverp.o
endif

