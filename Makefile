cwd=$(shell readlink -f .)
all:
	@mkdir -p ../build_YearlyTrendMakerOverlap/ 2>/dev/null
	@cd ../build_YearlyTrendMakerOverlap/; cmake $(cwd); make
	@ln -sf ../build_YearlyTrendMakerOverlap/main

clean:
	@rm -rf ../build_YearlyTrendMakerOverlap/*
