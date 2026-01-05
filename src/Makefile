MF_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
FOL_DIR := $(shell echo $(MF_DIR) | grep -o '.*/folat' | head -n1)

ifeq ($(FOL_DIR),)
	FOL_DIR := $(MF_DIR)
endif

BIN_DIR := $(FOL_DIR)/bin
SRC_DIR := $(FOL_DIR)/src
BINARY := $(BIN_DIR)/fol

YELLOW := \033[93m
BLUE := \033[94m
RESET := \033[0m

# Detect OS for strip command
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	STRIP_CMD = strip -x
else
	STRIP_CMD = strip --strip-all
endif

.PHONY: default
default: check-binary

.PHONY: check-binary
check-binary:
	@if [ -f "$(BINARY)" ]; then \
		echo "fol.$(BLUE)note$(RESET): the binary 'fol' already exists in '$(BINARY)'"; \
	else \
		echo "fol.$(BLUE)note$(RESET): binary not found, compiling ..."; \
		$(MAKE) build; \
	fi

.PHONY: build
build:
	@echo "fol.$(BLUE)note$(RESET): creating the 'bin' directory if it doesn't exist ..."
	@mkdir -p $(BIN_DIR)
	@echo "fol.$(BLUE)note$(RESET): compiling in '$(SRC_DIR)' ..."
	@cd $(SRC_DIR) && clang -O3 -mtune=native -funroll-loops -fomit-frame-pointer main.c flib.c -o fol
	@echo "fol.$(BLUE)note$(RESET): removing debug symbols..."
	@cd $(SRC_DIR) && $(STRIP_CMD) fol
	@echo "fol.$(BLUE)note$(RESET): moving binary to '$(BIN_DIR)' ..."
	@mv $(SRC_DIR)/fol $(BINARY)
	@echo "fol.$(BLUE)note$(RESET): binary in '$(BINARY)'"

.PHONY: addcli
addcli:
	@echo "fol.$(BLUE)note$(RESET): adding '$(BIN_DIR)' to PATH ..."
	@SHELL_RC=""; \
	SHELL_NAME=""; \
	if [ -n "$$BASH_VERSION" ]; then \
		SHELL_RC="$$HOME/.bashrc"; \
		SHELL_NAME="bash"; \
	elif [ -n "$$ZSH_VERSION" ]; then \
		SHELL_RC="$$HOME/.zshrc"; \
		SHELL_NAME="zsh"; \
	elif [ -n "$$KSH_VERSION" ]; then \
		SHELL_RC="$$HOME/.kshrc"; \
		SHELL_NAME="ksh"; \
	elif [ "$$0" = "sh" ] || [ "$$0" = "-sh" ]; then \
		SHELL_RC="$$HOME/.profile"; \
		SHELL_NAME="sh"; \
	else \
		SHELL_RC="$$HOME/.profile"; \
		SHELL_NAME="unknown"; \
		echo "fol.$(YELLOW)warn$(RESET): could not detect shell, using .profile"; \
	fi; \
	if [ -z "$$SHELL_RC" ]; then \
		echo "fol.$(YELLOW)warn$(RESET): could not determine shell config file"; \
		echo "fol.$(BLUE)note$(RESET): manually add 'export PATH=\"$(BIN_DIR):\$$PATH\"' to your shell config"; \
		exit 1; \
	fi; \
	if grep -q "$(BIN_DIR)" "$$SHELL_RC" 2>/dev/null; then \
		echo "fol.$(BLUE)note$(RESET): PATH already has '$(BIN_DIR)' in '$$SHELL_RC'"; \
	else \
		echo "" >> "$$SHELL_RC"; \
		echo "# Folat CLI" >> "$$SHELL_RC"; \
		echo "export PATH=\"$(BIN_DIR):\$$PATH\"" >> "$$SHELL_RC"; \
		echo "fol.$(BLUE)note$(RESET): added to '$$SHELL_RC' ($$SHELL_NAME)"; \
		echo "fol.$(BLUE)note$(RESET): execute 'source $$SHELL_RC' or open a new terminal"; \
	fi

.PHONY: clean
clean:
	@echo "fol.$(YELLOW)warn$(RESET): removing binary ..."
	@rm -f $(BINARY)
	@echo "fol.$(BLUE)note$(RESET): removed binary"

.PHONY: rebuild
rebuild: clean build

.PHONY: help
help:
	@echo "$(BLUE)Folat Makefile$(RESET)"
	@echo "  make          (default) verify and build if necessary."
	@echo "  make build    compile the C codes in 'src'."
	@echo "  make addcli   add the 'bin' directory to PATH (bash/zsh/ksh/sh)."
	@echo "  make clean    remove the binary."
	@echo "  make rebuild  recompile, clean and build."
	@echo "  make help     show this help."