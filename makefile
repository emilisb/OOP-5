TARGET_EXEC ?= uzduotis

BUILD_DIR ?= ./build
SRC_DIRS ?= ./src
TARGET_DIR ?= ./bin
DATA_DIR ?= ./data

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c')
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CPPFLAGS ?= $(INC_FLAGS) -MMD -MP -std=c++17 -O3

$(TARGET_DIR)/$(TARGET_EXEC): $(OBJS)
	$(MKDIR_P) $(dir $@)
	cp -a $(DATA_DIR)/. $(TARGET_DIR)/$(DATA_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@


.PHONY: clean

run:
	$(TARGET_DIR)/$(TARGET_EXEC)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) -r $(TARGET_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
