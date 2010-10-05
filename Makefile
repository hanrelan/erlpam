SERVICE_NAME="service_name"
ERL_ROOT=/usr/local/lib/erlang

OUTPUT_LIB=erlpam.so
OUTPUT_MODULE=erlpam.beam

all: $(OUTPUT_LIB) $(OUTPUT_MODULE)

$(OUTPUT_LIB): erlpam.c
	gcc -DSERVICE_NAME=$(SERVICE_NAME) -fPIC -shared -o $(OUTPUT_LIB) -I $(ERL_ROOT)/usr/include/ -lpam -lpam_misc

$(OUTPUT_MODULE): erlpam.erl
	erlc erlpam.erl

clean:
	rm $(OUTPUT_LIB) $(OUTPUT_MODULE)
