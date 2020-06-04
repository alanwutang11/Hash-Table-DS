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
PROJ_NAME       = proj4_89
TESTHASH_NAME   = testhash
TESTVECTOR_NAME = testvector


MAIN_O        = $(OBJ_DIR)/main.o

TESTVECTOR_O    = $(OBJ_DIR)/testvector.o

TESTHASH_O    = $(OBJ_DIR)/testhash.o

HASH_O    = $(OBJ_DIR)/HashTable.o

VECTOR_O    =$(OBJ_DIR)/Vector.o

PROJ_O       = $(HASH_O) $(VECTOR_O)

all: directories runtestvector runtesthash $(BIN_DIR)/$(PROJ_NAME)


runtestvector: $(TESTBIN_DIR)/$(TESTVECTOR_NAME)
	$(TESTBIN_DIR)/$(TESTVECTOR_NAME)

runtesthash: $(TESTBIN_DIR)/$(TESTHASH_NAME)
	$(TESTBIN_DIR)/$(TESTHASH_NAME)


$(BIN_DIR)/$(PROJ_NAME): $(PROJ_O) $(MAIN_O)
	$(CXX) $(MAIN_O) $(PROJ_O) -o $(BIN_DIR)/$(PROJ_NAME) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

$(TESTBIN_DIR)/$(TESTVECTOR_NAME): $(PROJ_O) $(TESTVECTOR_O)
	$(CXX) $(PROJ_O) $(TESTVECTOR_O) -o $(TESTBIN_DIR)/$(TESTVECTOR_NAME) $(CPPFLAGS) $(TESTLDFLAGS)

$(TESTBIN_DIR)/$(TESTHASH_NAME): $(PROJ_O) $(TESTHASH_O)
	$(CXX) $(PROJ_O) $(TESTHASH_O) -o $(TESTBIN_DIR)/$(TESTHASH_NAME) $(CPPFLAGS) $(TESTLDFLAGS)

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
