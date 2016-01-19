LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sanguocpp_shared

LOCAL_MODULE_FILENAME := libsanguocpp


#####################################

#工程目录
MY_PROJECT_PATH := $(LOCAL_PATH)/../../../GameSanGuo/GameSanGuo
#android的SDK的工程目录
SDK_PROJECT_PATH := $(LOCAL_PATH)/../..
#/Users/fygowoo/Workspace/test/sanguo-client-xcode5/GameSanGuo/GameSanGuo
#引擎目录
#MY_ENGINE_PATH := /Users/fygowoo/Workspace/test/sanguo-client-xcode5/GameSanGuo/GameSanGuo/libs
#工程文件目录
MY_FILES_PATH  := $(MY_PROJECT_PATH)/Classes

MY_FILES_SUFFIX := %.cpp %.cc

#
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

#
MY_ALL_FILES := $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*.*) )
MY_ALL_FILES += $(MY_PROJECT_PATH)/OCCaller/PlatformAdp.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/OCCaller/AndroidSDKAdp.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/ExtClass/ExtClass.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/Sdk/SdkController.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/Sdk/SdkImp.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/Sdk/Go2playSdkImp/Go2playAdrImp.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/CocosDenshion/playSound/CCPlaySound.cpp


#added by cgp for spine
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Animation.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/AnimationState.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/AnimationStateData.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Atlas.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/AtlasAttachmentLoader.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Attachment.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/AttachmentLoader.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Bone.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/BoneData.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/CCSkeleton.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/CCSkeletonAnimation.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Json.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/RegionAttachment.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Skeleton.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/SkeletonData.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/SkeletonJson.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Skin.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/Slot.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/SlotData.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/extension.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/spine/spine-cocos2dx.cpp


#add for cocostudio
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/CCArmature.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/CCBone.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/animation/CCArmatureAnimation.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/animation/CCProcessBase.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/animation/CCTween.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/datas/CCDatas.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/display/CCBatchNode.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/display/CCDecorativeDisplay.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/display/CCDisplayFactory.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/display/CCDisplayManager.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/display/CCSkin.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/physics/CCColliderDetector.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCArmatureDefine.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCArmatureDataManager.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCDataReaderHelper.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCSpriteFrameCacheHelper.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCTransformHelp.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCTweenFunction.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Armature/utils/CCUtilMath.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/BaseClasses/UIWidget.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/Layouts/UILayout.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/Layouts/UILayoutParameter.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/Layouts/UILayoutDefine.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/System/CocosGUI.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/System/UIHelper.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/System/UITouchGroup.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/ScrollWidget/UIListView.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/ScrollWidget/UIPageView.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/ScrollWidget/UIScrollView.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UIButton.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UICheckBox.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UIImageView.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UILabel.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UILabelAtlas.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UILabelBMFont.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UILoadingBar.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UISlider.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UITextField.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/GUI/UIWidgets/UIRichText.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Components/CCComAttribute.cpp
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Components/CCComAudio.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Components/CCComController.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Components/CCComRender.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Components/CCInputDelegate.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Json/DictionaryHelper.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Json/CocoLoader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/ActionTimeline/CCActionTimeline.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/ActionTimeline/CCActionTimelineCache.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/ActionTimeline/CCFrame.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/ActionTimeline/CCNodeReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/ActionTimeline/CCTimeline.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/SceneReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/GUIReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/WidgetReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/ButtonReader/ButtonReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/CheckBoxReader/CheckBoxReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/ImageViewReader/ImageViewReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/LabelAtlasReader/LabelAtlasReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/LabelBMFontReader/LabelBMFontReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/LabelReader/LabelReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/LayoutReader/LayoutReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/ListViewReader/ListViewReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/LoadingBarReader/LoadingBarReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/PageViewReader/PageViewReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/ScrollViewReader/ScrollViewReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/SliderReader/SliderReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Reader/WidgetReader/TextFieldReader/TextFieldReader.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Action/CCActionManager.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Action/CCActionObject.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Action/CCActionNode.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Action/CCActionFrame.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Action/CCActionEaseEx.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Action/CCActionFrameEasing.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Trigger/ObjectFactory.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Trigger/TriggerBase.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Trigger/TriggerMng.cpp 
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/CocoStudio/Trigger/TriggerObj.cpp 

#native
MY_ALL_FILES += $(MY_PROJECT_PATH)/libs/extensions/native/CCNative.cpp


MY_ALL_FILES := $(MY_ALL_FILES:$(MY_CPP_PATH)/./%=$(MY_CPP_PATH)%)
MY_SRC_LIST  := $(filter $(MY_FILES_SUFFIX),$(MY_ALL_FILES)) 
MY_SRC_LIST  := $(MY_SRC_LIST:$(LOCAL_PATH)/%=%)

#
define uniq =
  $(eval seen :=)
  $(foreach _,$1,$(if $(filter $_,${seen}),,$(eval seen += $_)))
  ${seen}
endef

#
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))

#
#.cpp .cc
LOCAL_SRC_FILES := xdyoucpp/main.cpp
LOCAL_SRC_FILES  += $(MY_SRC_LIST)

#.h
LOCAL_C_INCLUDES := $(MY_PROJECT_PATH)/OCCaller
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/ExtClass
LOCAL_C_INCLUDES += $(MY_ALL_DIRS)
LOCAL_C_INCLUDES += $(MY_FILES_PATH)/game

#sdk扩展
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/Sdk
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/Sdk/Go2playSdkImp

LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/actions
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/base_nodes
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/cocoa
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/include
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/kazmath
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/keypad_dispatcher
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/label_nodes
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/language
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/layers_scenes_transitions_nodes
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/menu_nodes
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/misc_nodes
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/platform
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/platform/android
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/platform/third_party/android
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/script_support
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/shaders
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/sprite_nodes
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/support
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/support/component
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/support/data_support
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/support/tinyxml2
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/support/user_default
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/support/zip_support

LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/text_input_node
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/textures
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/cocos2dx/touch_dispatcher

MY_FILES_PATH  := $(MY_PROJECT_PATH)/libs/extensions
MY_ALL_DIRS := $(dir $(foreach src_path,$(MY_FILES_PATH), $(call rwildcard,$(src_path),*/) ) )
MY_ALL_DIRS := $(call uniq,$(MY_ALL_DIRS))
LOCAL_C_INCLUDES += $(MY_ALL_DIRS)

LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/CocosDenshion/android
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/CocosDenshion/playSound
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/external


###lua
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/cocos2dx_support
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/lua
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/luajit
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/luajit/android
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/luajit/include
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/luajit/android
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/script
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/scripting/lua/tolua

###json
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/libjson
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/libjson/Source
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/libjson/Source/JSONDefs
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/libjson/Dependencies/mempool++
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/libjson/Dependencies/libbase64++

###protobuf
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/external/protobuf/src/
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/external/protobuf/src/google/protobuf
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/external/protobuf/src/google/protobuf/stubs
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/external/protobuf/src/google/protobuf/io
LOCAL_C_INCLUDES += $(MY_PROJECT_PATH)/libs/external/protobuf/src/google/protobuf/compiler

###talkingdata adpter class
LOCAL_C_INCLUDES += $(SDK_PROJECT_PATH)/Game_Analytics_SDK/include
LOCAL_C_INCLUDES += $(SDK_PROJECT_PATH)/Game_Analytics_SDK/platform/android



################################

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_lua_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
LOCAL_WHOLE_STATIC_LIBRARIES += protobuf_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_curl_static
LOCAL_WHOLE_STATIC_LIBRARIES += json_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx-talkingdata

include $(BUILD_SHARED_LIBRARY)


$(call import-add-path,	$(MY_PROJECT_PATH)/libs)
$(call import-add-path, $(MY_PROJECT_PATH)/libs/cocos2dx)
$(call import-add-path, $(MY_PROJECT_PATH)/libs/cocos2dx/platform/third_party/android/prebuilt)
$(call import-add-path, $(MY_PROJECT_PATH)/libs/external)
$(call import-add-path, $(SDK_PROJECT_PATH)/Game_Analytics_SDK)


$(call import-module,cocos2dx)
$(call import-module,CocosDenshion/android)
$(call import-module,scripting/lua/proj.android)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,extensions)
$(call import-module,external/protobuf)
$(call import-module,libjson)
$(call import-module,proj.android/jni)

