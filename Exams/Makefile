SOURCES := tenta_2026_03_14.md tenta_2026_03_14_solution.md tenta_2026_03_14_ARM.md
PDFS    := $(SOURCES:.md=.pdf)

.PHONY: all clean

all: $(PDFS)

%.pdf: %.md
	pandoc $< -o $@ --pdf-engine=wkhtmltopdf --pdf-engine-opt=--enable-local-file-access \
	  --include-in-header=pdf-header.html \
	  --pdf-engine-opt=--margin-top --pdf-engine-opt=15 \
	  --pdf-engine-opt=--margin-bottom --pdf-engine-opt=15 \
	  --pdf-engine-opt=--margin-left --pdf-engine-opt=15 \
	  --pdf-engine-opt=--margin-right --pdf-engine-opt=15

clean:
	rm -f $(PDFS)
