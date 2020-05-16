CXX = g++

INC_DIR = ./include
SRC_DIR = ./src
TESTS_DIR = ./tests
OBJ_DIR = ./obj
BIN_DIR = ./bin
TESTS_DIR = ./tests
TESTBIN_DIR = ./testbin


INCLUDE = -I $(INC_DIR) -I /opt/local/include
TESTLDFLAGS = -L /opt/local/lib -lgtest_main -lgtest -lpthread
CPPFLAGS        += -std=c++14
CFLAGS          += -Wall
PROJ_NAME       = proj3_89
TESTMAP_NAME   = testmap


MAIN_O        = $(OBJ_DIR)/main.o

TESTMAP_O    = $(OBJ_DIR)/testmap.o

PROJ_O       = $(OBJ_DIR)/KeyValueMap.o

all: directories runtest $(BIN_DIR)/$(PROJ_NAME)

runtest: $(TESTBIN_DIR)/$(TESTMAP_NAME)
	$(TESTBIN_DIR)/$(TESTMAP_NAME)

$(BIN_DIR)/$(PROJ_NAME): $(PROJ_O) $(MAIN_O)
	$(CXX) $(MAIN_O) $(PROJ_O) -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

$(TESTBIN_DIR)/$(TESTMAP_NAME): $(PROJ_O) $(TESTMAP_O)
	$(CXX) $(PROJ_O) $(TESTMAP_O) -o $(TESTBIN_DIR)/$(TESTMAP_NAME) $(CFLAGS) $(CPPFLAGS) $(TESTLDFLAGS)

$(OBJ_DIR)/%.o: $(TESTS_DIR)/%.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@


directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	mkdir -p $(TESTBIN_DIR)

clean::
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(TESTBIN_DIR)

.PHONY: clean
