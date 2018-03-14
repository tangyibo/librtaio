#
#
BINDIR = bin

S_EXES = rtdb_server
C_LIBS = rtdbapi
C_EXES = rtdb_client

RM :=rm -f 

S_OBJS +=src/libs/log4z.o \
	src/common/input_stream.o \
	src/common/output_stream.o \
	src/common/net_stream.o \
	src/protocol/packet_protocol.o \
	src/core/network/network_server.o \
	src/core/network/async_acceptor.o \
	src/core/network/session_handler.o \
	src/core/network/api_network.o \
	src/libs/app.o \
	src/main.o

C_OBJS +=src/common/net_stream.o \
	src/common/input_stream.o \
        src/common/output_stream.o \
        src/protocol/packet_protocol.o \
	src/api/rtdb_client_impl.o \
	src/api/rtdb_client.o \
	src/api/dllmain.o

CXXFLAGS = -g -Wall
CPPFLAGS = -I./include -I./src
LIBS = -lboost_signals -lboost_system -lboost_thread-mt

all: server libapi client

show:
	@echo "EXES=$(S_EXES)"
	@echo "OBJS=$(S_OBJS)"

server: $(S_OBJS) $(S_EXES)

libapi: $(C_OBJS) $(C_LIBS)


$(S_EXES): $(S_OBJS)
	g++ -o $(BINDIR)/$@ $^ $(CXXFLAGS) $(CPPFLAGS) $(LIBS)

$(C_LIBS): $(C_OBJS)
	ar -cr $(BINDIR)/lib$@.a  $^

client:
	g++ -g -Wall -I./include -I./src src/test/client_main.cpp -o $(BINDIR)/$(C_EXES) -L$(BINDIR) -l$(C_LIBS) $(LIBS)

clean:
	$(RM) $(S_OBJS) $(C_OBJS)
	$(RM) src/test/*.o 
	$(RM) $(BINDIR)/*
#
#
