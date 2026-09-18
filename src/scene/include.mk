SCENE_FILES = camera.c cylinder.c cylinder_utils.c cylinder_utils2.c light.c plane.c read_file.c scene.c sphere.c sphere_utils.c

SOURCES += $(addprefix $(SCENE_DIR)/,$(SCENE_FILES))
