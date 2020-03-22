LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

LUA_PATH := ../lua
SDL_PATH := ../SDL2
SDLIMAGE_PATH := ../SDL2_image
SDLMIXER_PATH := ../SDL2_mixer
SDLNET_PATH := ../SDL2_net

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(LUA_PATH)/src $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/$(SDLIMAGE_PATH) $(LOCAL_PATH)/$(SDLMIXER_PATH) $(LOCAL_PATH)/$(SDLNET_PATH)

# Add your application source files here...
LOCAL_SRC_FILES := \
	$(SDL_PATH)/src/main/android/SDL_android_main.c \
	src/bind.c \
	src/entry.c \
	src/game.c \
	src/gamesbind.c \
	src/gfx.c \
	src/host.c \
	src/item.c \
	src/itemsbind.c \
	src/key.c \
	src/level.c \
	src/levelaid.c \
	src/levelgen.c \
	src/levelsbind.c \
	src/main.c \
	src/math.c \
	src/menu.c \
	src/net.c \
	src/nethost.c \
	src/netsend.c \
	src/send.c \
	src/sound.c \
	src/tile.c \
	src/unit.c \
	src/unitsbind.c \
	src/unituse.c \
	src/view.c

LOCAL_SHARED_LIBRARIES := lua SDL2 SDL2_image SDL2_mixer SDL2_net

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
