## Runs minify.py on the whole batch of .glsl files in this folder.
##
## Premake can't do this build step because it has multiple inputs AND multiple outputs.
##
## The files have to be processed in batch in to ensure consistent renaming.
OUT := out/generated
FLAGS :=

## Shader minification.
MINIFY_INPUTS := $(wildcard *.glsl)
MINIFY_EXPORT_OUTPUTS := $(addprefix $(OUT)/, $(patsubst %.glsl, %.exports.h, $(MINIFY_INPUTS)))
MINIFY_GLSL_OUTPUTS := $(addprefix $(OUT)/, $(patsubst %.glsl, %.minified.glsl, $(MINIFY_INPUTS)))
MINIFY_HPP_OUTPUTS := $(addprefix $(OUT)/, $(patsubst %.glsl, %.glsl.hpp, $(MINIFY_INPUTS)))
MINIFY_OUTPUTS := $(MINIFY_EXPORT_OUTPUTS) $(MINIFY_GLSL_OUTPUTS) $(MINIFY_HPP_OUTPUTS)
MINIFY_STAMP := $(OUT)/glsl.stamp

minify: $(MINIFY_OUTPUTS)

## Using a stamp enables a build step with multiple inputs and multiple outputs.
## https://www.gnu.org/software/automake/manual/html_node/Multiple-Outputs.html
$(MINIFY_OUTPUTS): $(MINIFY_STAMP)
	@test -f $@ || rm -f $(MINIFY_STAMP)
	@test -f $@ || "$(MAKE)" $(AM_MAKEFLAGS) $(MINIFY_STAMP)

$(MINIFY_STAMP): $(MINIFY_INPUTS) minify.py
	python3 minify.py $(FLAGS) -o $(OUT) $(MINIFY_INPUTS)
	@touch $(MINIFY_STAMP)


## Metal shader offline compiling.
DRAW_COMBINATIONS_METAL := $(OUT)/draw_combinations.metal
METAL_INPUTS := $(wildcard metal/*.metal)
METAL_MACOSX_AIR_OUTPUTS := \
	$(addprefix $(OUT)/, $(patsubst metal/%.metal, macosx/%.air, $(METAL_INPUTS)))
METAL_IOS_AIR_OUTPUTS := $(addprefix $(OUT)/, $(patsubst metal/%.metal, ios/%.air, $(METAL_INPUTS)))

$(DRAW_COMBINATIONS_METAL): metal/generate_draw_combinations.py
	@mkdir -p $(OUT)
	python3 metal/generate_draw_combinations.py $(DRAW_COMBINATIONS_METAL)

rive_pls_macosx_metallib: $(OUT)/rive_pls_macosx.metallib.c
rive_pls_ios_metallib: $(OUT)/rive_pls_ios.metallib.c
rive_pls_ios_simulator_metallib: $(OUT)/rive_pls_ios_simulator.metallib.c

## The source files all get regenerated in a batch, so there's no need to separate out separate
## rules for each intermediate file.
$(OUT)/macosx/rive_pls_macosx.metallib: $(MINIFY_GLSL_OUTPUTS) $(METAL_INPUTS) $(DRAW_COMBINATIONS_METAL)
	@mkdir -p $(OUT)/macosx
	$(foreach FILE, $(METAL_INPUTS), \
		xcrun -sdk macosx metal -std=macos-metal2.3 \
		-mmacosx-version-min=10.0 \
		-I$(OUT) -ffast-math -ffp-contract=fast -fpreserve-invariance -fvisibility=hidden \
		-c $(FILE) \
		-o $(patsubst metal/%.metal, $(OUT)/macosx/%.air, $(FILE));)
	xcrun -sdk macosx metallib $(METAL_MACOSX_AIR_OUTPUTS) -o $(OUT)/macosx/rive_pls_macosx.metallib

$(OUT)/rive_pls_macosx.metallib.c: $(OUT)/macosx/rive_pls_macosx.metallib
	xxd -i -n rive_pls_macosx_metallib \
		$(OUT)/macosx/rive_pls_macosx.metallib \
		$(OUT)/rive_pls_macosx.metallib.c

$(OUT)/ios/rive_pls_ios.metallib: $(MINIFY_GLSL_OUTPUTS) $(METAL_INPUTS) $(DRAW_COMBINATIONS_METAL)
	@mkdir -p $(OUT)/ios
	$(foreach FILE, $(METAL_INPUTS), \
		xcrun -sdk iphoneos metal -std=ios-metal2.2 \
		-I$(OUT) -mios-version-min=13 -ffast-math -ffp-contract=fast -fpreserve-invariance \
		-fvisibility=hidden \
		-c $(FILE) \
		-o $(patsubst metal/%.metal, $(OUT)/ios/%.air, $(FILE));)
	xcrun -sdk iphoneos metallib $(METAL_IOS_AIR_OUTPUTS) -o $(OUT)/ios/rive_pls_ios.metallib

$(OUT)/rive_pls_ios.metallib.c: $(OUT)/ios/rive_pls_ios.metallib
	xxd -i -n rive_pls_ios_metallib $(OUT)/ios/rive_pls_ios.metallib $(OUT)/rive_pls_ios.metallib.c

$(OUT)/ios/rive_pls_ios_simulator.metallib: $(MINIFY_GLSL_OUTPUTS) $(METAL_INPUTS) $(DRAW_COMBINATIONS_METAL)
	@mkdir -p $(OUT)/ios
	$(foreach FILE, $(METAL_INPUTS), \
		xcrun -sdk iphonesimulator metal -std=ios-metal2.2 \
		-I$(OUT) -miphonesimulator-version-min=13 -ffast-math -ffp-contract=fast -fpreserve-invariance \
		-fvisibility=hidden \
		-c $(FILE) \
		-o $(patsubst metal/%.metal, $(OUT)/ios/%.air, $(FILE));)
	xcrun -sdk iphonesimulator metallib $(METAL_IOS_AIR_OUTPUTS) -o $(OUT)/ios/rive_pls_ios_simulator.metallib

$(OUT)/rive_pls_ios_simulator.metallib.c: $(OUT)/ios/rive_pls_ios_simulator.metallib
	xxd -i -n rive_pls_ios_simulator_metallib $(OUT)/ios/rive_pls_ios_simulator.metallib $(OUT)/rive_pls_ios_simulator.metallib.c


## SPIRV compilation.
SPIRV_INPUTS := $(wildcard spirv/*.main)
SPIRV_OUTPUTS := \
    $(addprefix $(OUT)/, $(patsubst %.main, %.vert.h, $(SPIRV_INPUTS))) \
    $(addprefix $(OUT)/, $(patsubst %.main, %.frag.h, $(SPIRV_INPUTS))) \
    $(OUT)/spirv/atomic_draw_image_mesh.fixedcolor_frag.h \
    $(OUT)/spirv/atomic_draw_image_rect.fixedcolor_frag.h \
    $(OUT)/spirv/atomic_draw_interior_triangles.fixedcolor_frag.h \
    $(OUT)/spirv/atomic_draw_path.fixedcolor_frag.h \
    $(OUT)/spirv/atomic_resolve_pls.fixedcolor_frag.h \

## Compile *.main into vertex shaders.
$(OUT)/spirv/%.vert.h: spirv/%.main $(MINIFY_STAMP)
	@mkdir -p $(OUT)/spirv
	@glslangValidator -S vert -DTARGET_VULKAN -DVERTEX -I$(OUT) -V --vn $(subst .main,_vert,$(notdir $<)) -o $@ $<

## Compile *.main again into fragment shaders.
$(OUT)/spirv/%.frag.h: spirv/%.main $(MINIFY_STAMP)
	@mkdir -p $(OUT)/spirv
	@glslangValidator -S frag -DTARGET_VULKAN -DFRAGMENT -I$(OUT) -V --vn $(subst .main,_frag,$(notdir $<)) -o $@ $<

## Compile atomic fragment shaders again with FIXED_FUNCTION_COLOR_OUTPUT defined.
$(OUT)/spirv/%.fixedcolor_frag.h: spirv/%.main $(MINIFY_STAMP)
	@mkdir -p $(OUT)/spirv
	@glslangValidator -S frag -DTARGET_VULKAN -DFRAGMENT -DFIXED_FUNCTION_COLOR_OUTPUT -I$(OUT) -V --vn $(subst .main,_fixedcolor_frag,$(notdir $<)) -o $@ $<

spirv: $(SPIRV_OUTPUTS)


## Cleaning.
clean:
	@rm -fr out
