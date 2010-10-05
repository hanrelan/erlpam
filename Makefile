SERVICE_NAME="service_name"
ERL_ROOT=/usr/local/lib/erlang

OUTPUT_FILE=erlpam.so

all: $(OUTPUT_FILE)

$(OUTPUT_FILE): erlpam.c
	gcc -DSERVICE_NAME=$(SERVICE_NAME) -fPIC -shared -o $(OUTPUT_FILE) -I $(ERL_ROOT)/usr/include/ -lpam -lpam_misc

clean:
	rm $(OUTPUT_FILE)
