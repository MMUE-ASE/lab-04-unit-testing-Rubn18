# Lab 4 — Unit testing
#
# This Makefile is a thin, familiar front door over Ceedling (the tool that
# actually builds and runs the tests, using rake under the hood). The targets
# mirror the muscle memory from Lab 2.
#
#   make test       run all unit tests          (Ceedling: ceedling test:all)
#   make coverage   run tests + coverage summary (Ceedling: ceedling gcov:all)
#   make exercises  run the warm-up exercises    (docs/exercises/)
#   make clean      delete generated build files (Ceedling: ceedling clobber)
#   make help       show this message
#
# Requires: Ruby + Ceedling on PATH (see Docs/software_tools.md, section 9).

CEEDLING   = ceedling
EXERCISES  = docs/exercises

.DEFAULT_GOAL := help

test:
	$(CEEDLING) test:all

coverage:
	$(CEEDLING) gcov:all

exercises:
	cd $(EXERCISES) && $(CEEDLING) test:all

clean:
	$(CEEDLING) clobber

help:
	@echo ""
	@echo "  Lab 4 — Unit testing targets:"
	@echo "    make test       Run all unit tests"
	@echo "    make coverage   Run tests and print a coverage summary"
	@echo "    make exercises  Run the warm-up exercises (docs/exercises/)"
	@echo "    make clean      Remove generated build files"
	@echo "    make help       Show this message"
	@echo ""

.PHONY: test coverage exercises clean help
