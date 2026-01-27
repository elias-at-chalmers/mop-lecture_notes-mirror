.PHONY: all clean

EXT = svg
BUILD_DIR = graphs
SRC_DIR = src
OUT = $(addprefix $(BUILD_DIR)/, $(addsuffix .$(EXT), main keyb ascii))

all: $(OUT)

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.$(EXT): $(SRC_DIR)/%.dot | $(BUILD_DIR)
	dot -T$(EXT) $< > $@

clean:
	$(RM) -r $(BUILD_DIR)
