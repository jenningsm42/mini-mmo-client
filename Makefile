LIBS=-lsfml-graphics -lsfml-window -lsfml-system -lprotobuf
CXX := g++
SRC_DIR := ./src
OBJ_DIR := ./obj
PROTO_DIR := ./proto
OUT_DIR := ./build
DATA_DIR := ./data
PROTO_FILES := $(wildcard $(PROTO_DIR)/*.proto)
PROTO_GEN_FILES := $(patsubst $(PROTO_DIR)/%.proto,$(SRC_DIR)/%.pb.cc,$(PROTO_FILES))
PROTO_GEN_FILES_FULL := \
	$(PROTO_GEN_FILES) \
	$(patsubst $(PROTO_DIR)/%.proto,$(SRC_DIR)/%.pb.h,$(PROTO_FILES))
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := \
	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES)) \
	$(patsubst $(SRC_DIR)/%.pb.cc,$(OBJ_DIR)/%.o,$(PROTO_GEN_FILES))

all: client

.SECONDARY:
$(SRC_DIR)/%.pb.cc: $(PROTO_DIR)/%.proto
	protoc -I=$(PROTO_DIR) --cpp_out=$(SRC_DIR) $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.pb.cc
	$(CXX) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(PROTO_GEN_FILES)
	$(CXX) -c $< -o $@

client: $(OBJ_FILES)
	@echo '** Building the client'
	$(CXX) -o $(OUT_DIR)/$@ $^ $(LIBS)
	cp -r $(DATA_DIR) $(OUT_DIR)/

clean:
	@echo '** Cleaning project'
	$(RM) -r $(OUT_DIR)/* $(OBJ_DIR)/*
	$(RM) $(PROTO_GEN_FILES_FULL)

.PHONY: all client clean
