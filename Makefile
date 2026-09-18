# =============================================================================
# PROJECT CONFIGURATION — edit this section for each project
# =============================================================================

# Project name (produces bin/<mode>/NAME)
NAME = miniRT

# Compiler
CC = cc

# Preprocessor flags (include paths, defines)
CPPFLAGS = -Iinclude -Ilibft -Ift_linalg/include -IMLX42/include/MLX42

# Common compiler flags shared by all build modes
CFLAGS_COMMON = -Wall -Wextra -Werror

# Per-mode compiler flags (appended to CFLAGS_COMMON)
CFLAGS_RELEASE = -O3
CFLAGS_DEBUG   = -g -O0 -DDEBUG

# System libraries to link (order matters for static linking)
LDLIBS = -lm -ldl -lglfw -lpthread

# Source directory and top-level source files
SRC_DIR = src
SRC_FILES = main.c

# Subdirectory source modules (each include.mk appends to SOURCES)
SOURCES =
SOURCES += $(addprefix $(SRC_DIR)/,$(SRC_FILES))

DYNARR_DIR = $(SRC_DIR)/dynarr
include $(DYNARR_DIR)/include.mk

RAY_DIR = $(SRC_DIR)/ray
include $(RAY_DIR)/include.mk

SCENE_DIR = $(SRC_DIR)/scene
include $(SCENE_DIR)/include.mk

UTIL_DIR = $(SRC_DIR)/utils
include $(UTIL_DIR)/include.mk

# Project-local static libraries (without lib prefix / .a suffix)
LIBS = mlx42 ft_linalg ft

# Arguments passed to the binary by 'run' / 'run-debug' targets
RUN_ARGS = scenes/test.rt

# =============================================================================
# BUILD SYSTEM — do not edit below this line
# =============================================================================

DEPFLAGS = -MMD -MP
BUILD_MODES = release debug

# -----------------------------------------------------------------------------
# BUILD_template(mode, extra_cflags)
#
# Generates all targets for a single build mode:
#   - object compilation with dependency tracking
#   - linking
#   - clean / fclean / re per mode
#   - phony convenience target
# -----------------------------------------------------------------------------
define BUILD_template

# Derived directories
OBJ_DIR_$(1)  = $(1)/obj
LIB_DIR_$(1)  = $(1)/lib
BIN_DIR_$(1)  = $(1)/bin

# Derived file lists
OBJECTS_$(1) = $$(patsubst $$(SRC_DIR)/%.c,$$(OBJ_DIR_$(1))/%.o,$$(SOURCES))
DEPS_$(1)    = $$(OBJECTS_$(1):.o=.d)
TARGET_$(1)  = $$(BIN_DIR_$(1))/$$(NAME)
LIBS_$(1)    = $$(patsubst %,$$(LIB_DIR_$(1))/lib%.a,$$(LIBS))

# Full CFLAGS and LDFLAGS for this mode
CFLAGS_FULL_$(1) = $$(CFLAGS_COMMON) $(2)
LDFLAGS_$(1)     = -L$$(LIB_DIR_$(1)) $$(patsubst %,-l%,$$(LIBS)) $$(LDLIBS)

# Phony convenience target
.PHONY: $(1)
$(1): $$(TARGET_$(1))
	@ln -srf $$(TARGET_$(1)) $$(NAME)
	@echo "$(1) build complete"

# Link
$$(TARGET_$(1)): $$(OBJECTS_$(1)) $$(LIBS_$(1))
	@mkdir -p $$(BIN_DIR_$(1))
	$$(CC) $$(CFLAGS_FULL_$(1)) $$(OBJECTS_$(1)) -o $$@ $$(LDFLAGS_$(1))

# Compile
$$(OBJ_DIR_$(1))/%.o: $$(SRC_DIR)/%.c | .submodules
	@mkdir -p $$(dir $$@)
	$$(CC) $$(DEPFLAGS) $$(CPPFLAGS) $$(CFLAGS_FULL_$(1)) -c $$< -o $$@

# Include dependency files
-include $$(DEPS_$(1))

# --- per-mode clean targets ---
.PHONY: clean-$(1)
clean-$(1):
	rm -rf $(1)
	@echo "$(1) build cleaned"

.PHONY: fclean-$(1)
fclean-$(1):
	@rm -f $(NAME)
	@rm -rf .submodules
	rm -rf $(1)
	@echo "$(1) build fully cleaned"

.PHONY: re-$(1)
re-$(1): fclean-$(1) $(1)

endef

# Instantiate for each build mode
$(eval $(call BUILD_template,release,$$(CFLAGS_RELEASE)))
$(eval $(call BUILD_template,debug,$$(CFLAGS_DEBUG)))

# -----------------------------------------------------------------------------
# Top-level targets
# -----------------------------------------------------------------------------

.PHONY: all
all: release

.PHONY: clean
clean: $(addprefix clean-,$(BUILD_MODES))
	@echo "Clean complete"

.PHONY: fclean
fclean: $(addprefix fclean-,$(BUILD_MODES))
	@echo "Full clean complete"

.PHONY: re
re: fclean all

# -----------------------------------------------------------------------------
# Run targets
# -----------------------------------------------------------------------------

.PHONY: run
run: release
	./$(TARGET_release) $(RUN_ARGS)

.PHONY: run-debug
run-debug: debug
	timeout --preserve-status -k 5s 1m ./$(TARGET_debug) $(RUN_ARGS)

# -----------------------------------------------------------------------------
# Sanitizer (project-specific extras)
# -----------------------------------------------------------------------------

.PHONY: asan
asan: CFLAGS_DEBUG += -fsanitize=address
asan: clean-debug debug

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------

.PHONY: norm
norm: .submodules
	@bash tools/norm

# -----------------------------------------------------------------------------
# Setup
# -----------------------------------------------------------------------------

.submodules:
	@git submodule update --init --recursive
	@touch $@

# -----------------------------------------------------------------------------
# Help
# -----------------------------------------------------------------------------

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  all (default)    - Build release version"
	@echo "  release          - Build release version"
	@echo "  debug            - Build debug version"
	@echo "  clean            - Remove all build artifacts"
	@echo "  clean-release    - Remove release build artifacts"
	@echo "  clean-debug      - Remove debug build artifacts"
	@echo "  fclean           - Full clean (includes submodule state)"
	@echo "  fclean-release   - Full clean release"
	@echo "  fclean-debug     - Full clean debug"
	@echo "  re               - Full clean and rebuild"
	@echo "  re-release       - Full clean and rebuild release"
	@echo "  re-debug         - Full clean and rebuild debug"
	@echo "  run              - Build and run release version"
	@echo "  run-debug        - Build and run debug version"
	@echo "  asan             - Build debug with AddressSanitizer"
	@echo "  gprof            - Build release with profiling"
	@echo "  help             - Show this help message"

# =============================================================================
# PROJECT LIBRARY RULES — edit this section for each project
#
# Library rules are inherently project-specific (cmake, make, etc.).
# Use the directory layout:  <mode>/lib/lib<name>.a
# =============================================================================

# --- MLX42 (cmake — same build for both modes) ---
%/lib/libmlx42.a: | .submodules
	@mkdir -p $(dir $@)
	@cmake -B MLX42/build -S MLX42
	@cmake --build MLX42/build
	@ln -srf MLX42/build/libmlx42.a $@

# --- libft (different targets per mode) ---
release/lib/libft.a: | .submodules
	@mkdir -p $(dir $@)
	@$(MAKE) -C libft re CFLAGS="-Wall -Wextra -Werror -Wno-prio-ctor-dtor -I."
	@cp -f libft/libft.a $@

debug/lib/libft.a: | .submodules
	@mkdir -p $(dir $@)
	@$(MAKE) -C libft clean debug CFLAGS="-Wall -Wextra -Werror -Wno-prio-ctor-dtor -I."
	@cp -f libft/libft.a $@

# --- libft_linalg (depends on libft) ---
release/lib/libft_linalg.a: release/lib/libft.a | .submodules
	@mkdir -p $(dir $@)
	@$(MAKE) -C ft_linalg re LIBFTDIR="../libft"
	@cp -f ft_linalg/libft_linalg.a $@

debug/lib/libft_linalg.a: debug/lib/libft.a | .submodules
	@mkdir -p $(dir $@)
	@$(MAKE) -C ft_linalg clean debug LIBFTDIR="../libft"
	@cp -f ft_linalg/libft_linalg.a $@
