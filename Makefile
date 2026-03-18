SRC_DIRS = lectures quickguide labs introductory_exercises practice_exams
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.md))

HTML_DIR = html
HTMLS := $(SRCS:%.md=$(HTML_DIR)/%.html)


ASSETS = images style.css quickguide.html

ETERM8_SRCS := $(wildcard labs/lab1/eterm8/*.asm)
ETERM8_DST  := $(patsubst labs/lab1/eterm8/%,$(HTML_DIR)/labs/%,$(ETERM8_SRCS))

all: $(HTMLS) assets $(ETERM8_DST)

# check if os is windows, imitate UNIX behavior
ifeq ($(OS), Windows_NT)
    MKDIR = powershell mkdir -Force
    RM = powershell rm -Force
else
    MKDIR = mkdir -p
endif


$(HTML_DIR)/%.html: %.md
	$(MKDIR) -p $(dir $@)
	pandoc $< -o $@ \
		--lua-filter=quickguide-generator/include-html.lua \
		--standalone \
		--css ../style.css \
		--ascii
	

assets: $(addprefix $(HTML_DIR)/,$(ASSETS))


$(HTML_DIR)/style.css: style.css
	$(MKDIR) -p $(HTML_DIR)
	cp $< $@

IMAGE_SRC := $(wildcard images/*)
IMAGE_DST := $(patsubst images/%,$(HTML_DIR)/images/%,$(IMAGE_SRC))

$(HTML_DIR)/images: $(IMAGE_DST)

$(HTML_DIR)/images/%: images/%
	$(MKDIR) -p $(dir $@)
	cp -r $< $@
	
$(HTML_DIR)/quickguide.html: quickguide.html
	$(MKDIR) -p $(HTML_DIR)
	cp $< $@

$(HTML_DIR)/labs/%.asm: labs/lab1/eterm8/%.asm
	$(MKDIR) -p $(HTML_DIR)/labs
	cp $< $@
