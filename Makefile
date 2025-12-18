SRC_DIRS = lectures quickguide labs
SRCS := $(foreach d,$(SRC_DIRS),$(wildcard $(d)/*.md))

HTML_DIR = html
HTMLS := $(SRCS:%.md=$(HTML_DIR)/%.html)


ASSETS = images style.css quickguide.html

all: $(HTMLS) assets

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

$(HTML_DIR)/images: images
	$(MKDIR) -p $(HTML_DIR)
	cp -r $< $@

$(HTML_DIR)/quickguide.html: quickguide.html
	$(MKDIR) -p $(HTML_DIR)
	cp $< $@
