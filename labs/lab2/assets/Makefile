.PHONY: all clean

EXT = svg
BUILD_DIR = ../../../images
SRC_DIR = src
OUT = $(addprefix $(BUILD_DIR)/lab2-, $(addsuffix .$(EXT), main keyb ascii))

all: $(OUT)

$(BUILD_DIR):
	mkdir $@

$(BUILD_DIR)/%.$(EXT): $(SRC_DIR)/%.dot | $(BUILD_DIR)
	dot -T$(EXT) $< > $@

clean:
	$(RM) -r $(BUILD_DIR)
