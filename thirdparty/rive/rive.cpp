/*
  ==============================================================================

   This file is part of the YUP library.
   Copyright (c) 2024 - kunitoki@gmail.com

   YUP is an open source library subject to open-source licensing.

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   YUP IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

#include "rive.h"

#if __clang__
 #pragma clang diagnostic push
 #pragma clang diagnostic ignored "-Wshorten-64-to-32"
#elif __GNUC__
 #pragma GCC diagnostic push
 #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER
 #pragma warning (push)
 #pragma warning (disable : 4244)
#endif

#include "source/generated/nested_artboard_layout_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_list_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_enum_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_color_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_number_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_enum_base.cpp"
#include "source/generated/viewmodel/data_enum_value_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_boolean_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_list_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_boolean_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_string_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_list_item_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_color_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_trigger_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_number_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_trigger_base.cpp"
#include "source/generated/viewmodel/viewmodel_property_viewmodel_base.cpp"
#include "source/generated/viewmodel/viewmodel_base.cpp"
#include "source/generated/viewmodel/data_enum_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_viewmodel_base.cpp"
#include "source/generated/viewmodel/viewmodel_component_base.cpp"
#include "source/generated/viewmodel/viewmodel_instance_string_base.cpp"
#include "source/generated/draw_target_base.cpp"
#include "source/generated/backboard_base.cpp"
#include "source/generated/constraints/follow_path_constraint_base.cpp"
#include "source/generated/constraints/distance_constraint_base.cpp"
#include "source/generated/constraints/translation_constraint_base.cpp"
#include "source/generated/constraints/ik_constraint_base.cpp"
#include "source/generated/constraints/scale_constraint_base.cpp"
#include "source/generated/constraints/transform_constraint_base.cpp"
#include "source/generated/constraints/rotation_constraint_base.cpp"
#include "source/generated/layout/n_slicer_base.cpp"
#include "source/generated/layout/axis_y_base.cpp"
#include "source/generated/layout/axis_x_base.cpp"
#include "source/generated/layout/n_sliced_node_base.cpp"
#include "source/generated/layout/layout_component_style_base.cpp"
#include "source/generated/layout/n_slicer_tile_mode_base.cpp"
#include "source/generated/audio_event_base.cpp"
#include "source/generated/nested_artboard_leaf_base.cpp"
#include "source/generated/layout_component_base.cpp"
#include "source/generated/animation/any_state_base.cpp"
#include "source/generated/animation/nested_state_machine_base.cpp"
#include "source/generated/animation/elastic_interpolator_base.cpp"
#include "source/generated/animation/state_machine_fire_event_base.cpp"
#include "source/generated/animation/transition_value_enum_comparator_base.cpp"
#include "source/generated/animation/transition_property_artboard_comparator_base.cpp"
#include "source/generated/animation/transition_value_boolean_comparator_base.cpp"
#include "source/generated/animation/blend_state_1d_base.cpp"
#include "source/generated/animation/state_machine_trigger_base.cpp"
#include "source/generated/animation/state_machine_base.cpp"
#include "source/generated/animation/listener_trigger_change_base.cpp"
#include "source/generated/animation/exit_state_base.cpp"
#include "source/generated/animation/listener_bool_change_base.cpp"
#include "source/generated/animation/listener_viewmodel_change_base.cpp"
#include "source/generated/animation/transition_value_string_comparator_base.cpp"
#include "source/generated/animation/listener_number_change_base.cpp"
#include "source/generated/animation/state_machine_fire_event.cpp"
#include "source/generated/animation/state_machine_number_base.cpp"
#include "source/generated/animation/state_transition_base.cpp"
#include "source/generated/animation/keyframe_double_base.cpp"
#include "source/generated/animation/transition_value_color_comparator_base.cpp"
#include "source/generated/animation/state_machine_bool_base.cpp"
#include "source/generated/animation/blend_state_transition_base.cpp"
#include "source/generated/animation/blend_animation_direct_base.cpp"
#include "source/generated/animation/transition_viewmodel_condition_base.cpp"
#include "source/generated/animation/transition_bool_condition_base.cpp"
#include "source/generated/animation/listener_align_target_base.cpp"
#include "source/generated/animation/nested_number_base.cpp"
#include "source/generated/animation/keyframe_string_base.cpp"
#include "source/generated/animation/transition_trigger_condition_base.cpp"
#include "source/generated/animation/transition_number_condition_base.cpp"
#include "source/generated/animation/keyframe_uint_base.cpp"
#include "source/generated/animation/keyframe_callback_base.cpp"
#include "source/generated/animation/nested_simple_animation_base.cpp"
#include "source/generated/animation/transition_value_trigger_comparator_base.cpp"
#include "source/generated/animation/nested_trigger_base.cpp"
#include "source/generated/animation/listener_fire_event_base.cpp"
#include "source/generated/animation/linear_animation_base.cpp"
#include "source/generated/animation/transition_value_number_comparator_base.cpp"
#include "source/generated/animation/entry_state_base.cpp"
#include "source/generated/animation/transition_artboard_condition_base.cpp"
#include "source/generated/animation/blend_state_direct_base.cpp"
#include "source/generated/animation/state_machine_layer_base.cpp"
#include "source/generated/animation/cubic_interpolator_component_base.cpp"
#include "source/generated/animation/blend_animation_1d_base.cpp"
#include "source/generated/animation/keyed_object_base.cpp"
#include "source/generated/animation/transition_property_viewmodel_comparator_base.cpp"
#include "source/generated/animation/keyframe_id_base.cpp"
#include "source/generated/animation/state_machine_listener_base.cpp"
#include "source/generated/animation/cubic_value_interpolator_base.cpp"
#include "source/generated/animation/nested_bool_base.cpp"
#include "source/generated/animation/keyframe_color_base.cpp"
#include "source/generated/animation/keyframe_bool_base.cpp"
#include "source/generated/animation/keyed_property_base.cpp"
#include "source/generated/animation/cubic_ease_interpolator_base.cpp"
#include "source/generated/animation/nested_remap_animation_base.cpp"
#include "source/generated/animation/animation_base.cpp"
#include "source/generated/animation/animation_state_base.cpp"
#include "source/generated/shapes/straight_vertex_base.cpp"
#include "source/generated/shapes/paint/radial_gradient_base.cpp"
#include "source/generated/shapes/paint/gradient_stop_base.cpp"
#include "source/generated/shapes/paint/dash_path_base.cpp"
#include "source/generated/shapes/paint/solid_color_base.cpp"
#include "source/generated/shapes/paint/trim_path_base.cpp"
#include "source/generated/shapes/paint/dash_base.cpp"
#include "source/generated/shapes/paint/linear_gradient_base.cpp"
#include "source/generated/shapes/paint/stroke_base.cpp"
#include "source/generated/shapes/paint/fill_base.cpp"
#include "source/generated/shapes/polygon_base.cpp"
#include "source/generated/shapes/cubic_detached_vertex_base.cpp"
#include "source/generated/shapes/clipping_shape_base.cpp"
#include "source/generated/shapes/mesh_base.cpp"
#include "source/generated/shapes/rectangle_base.cpp"
#include "source/generated/shapes/cubic_mirrored_vertex_base.cpp"
#include "source/generated/shapes/star_base.cpp"
#include "source/generated/shapes/mesh_vertex_base.cpp"
#include "source/generated/shapes/ellipse_base.cpp"
#include "source/generated/shapes/points_path_base.cpp"
#include "source/generated/shapes/triangle_base.cpp"
#include "source/generated/shapes/shape_base.cpp"
#include "source/generated/shapes/cubic_asymmetric_vertex_base.cpp"
#include "source/generated/shapes/contour_mesh_vertex_base.cpp"
#include "source/generated/shapes/image_base.cpp"
#include "source/generated/custom_property_number_base.cpp"
#include "source/generated/nested_artboard_base.cpp"
#include "source/generated/custom_property_boolean_base.cpp"
#include "source/generated/artboard_base.cpp"
#include "source/generated/draw_rules_base.cpp"
#include "source/generated/data_bind/bindable_property_enum_base.cpp"
#include "source/generated/data_bind/data_bind_base.cpp"
#include "source/generated/data_bind/bindable_property_boolean_base.cpp"
#include "source/generated/data_bind/converters/data_converter_rounder_base.cpp"
#include "source/generated/data_bind/converters/data_converter_group_base.cpp"
#include "source/generated/data_bind/converters/data_converter_operation_base.cpp"
#include "source/generated/data_bind/converters/data_converter_group_item_base.cpp"
#include "source/generated/data_bind/converters/data_converter_trigger_base.cpp"
#include "source/generated/data_bind/converters/data_converter_to_string_base.cpp"
#include "source/generated/data_bind/data_bind_context_base.cpp"
#include "source/generated/data_bind/bindable_property_color_base.cpp"
#include "source/generated/data_bind/bindable_property_trigger_base.cpp"
#include "source/generated/data_bind/bindable_property_string_base.cpp"
#include "source/generated/data_bind/bindable_property_number_base.cpp"
#include "source/generated/custom_property_string_base.cpp"
#include "source/generated/bones/tendon_base.cpp"
#include "source/generated/bones/skin_base.cpp"
#include "source/generated/bones/bone_base.cpp"
#include "source/generated/bones/weight_base.cpp"
#include "source/generated/bones/root_bone_base.cpp"
#include "source/generated/bones/cubic_weight_base.cpp"
#include "source/generated/text/text_modifier_group_base.cpp"
#include "source/generated/text/text_style_axis_base.cpp"
#include "source/generated/text/text_style_base.cpp"
#include "source/generated/text/text_modifier_range_base.cpp"
#include "source/generated/text/text_base.cpp"
#include "source/generated/text/text_variation_modifier_base.cpp"
#include "source/generated/text/text_style_feature_base.cpp"
#include "source/generated/text/text_value_run_base.cpp"
#include "source/generated/open_url_event_base.cpp"
#include "source/generated/node_base.cpp"
#include "source/generated/assets/file_asset_contents_base.cpp"
#include "source/generated/assets/folder_base.cpp"
#include "source/generated/assets/audio_asset_base.cpp"
#include "source/generated/assets/font_asset_base.cpp"
#include "source/generated/assets/image_asset_base.cpp"
#include "source/generated/event_base.cpp"
#include "source/generated/solo_base.cpp"
#include "source/generated/joystick_base.cpp"
#include "source/viewmodel/viewmodel_instance_color.cpp"
#include "source/viewmodel/viewmodel_instance_trigger.cpp"
#include "source/viewmodel/viewmodel_instance_list_item.cpp"
#include "source/viewmodel/viewmodel.cpp"
#include "source/viewmodel/viewmodel_instance_boolean.cpp"
#include "source/viewmodel/viewmodel_instance_string.cpp"
#include "source/viewmodel/viewmodel_property.cpp"
#include "source/viewmodel/data_enum.cpp"
#include "source/viewmodel/viewmodel_instance_list.cpp"
#include "source/viewmodel/viewmodel_instance_enum.cpp"
#include "source/viewmodel/viewmodel_property_enum.cpp"
#include "source/viewmodel/viewmodel_instance_value.cpp"
#include "source/viewmodel/viewmodel_instance.cpp"
#include "source/viewmodel/viewmodel_instance_viewmodel.cpp"
#include "source/viewmodel/viewmodel_instance_number.cpp"
#include "source/viewmodel/data_enum_value.cpp"
#include "source/draw_rules.cpp"
#include "source/core/field_types/core_bool_type.cpp"
#include "source/core/field_types/core_bytes_type.cpp"
#include "source/core/field_types/core_uint_type.cpp"
#include "source/core/field_types/core_double_type.cpp"
#include "source/core/field_types/core_string_type.cpp"
#include "source/core/field_types/core_color_type.cpp"
#include "source/core/binary_data_reader.cpp"
#include "source/core/binary_reader.cpp"
#include "source/core/binary_writer.cpp"
#include "source/scene.cpp"
#include "source/simple_array.cpp"
#include "source/nested_artboard.cpp"
#include "source/constraints/transform_constraint.cpp"
#include "source/constraints/distance_constraint.cpp"
#include "source/constraints/follow_path_constraint.cpp"
#include "source/constraints/constraint.cpp"
#include "source/constraints/rotation_constraint.cpp"
#include "source/constraints/ik_constraint.cpp"
#include "source/constraints/scale_constraint.cpp"
#include "source/constraints/targeted_constraint.cpp"
#include "source/constraints/translation_constraint.cpp"
#include "source/hittest_command_path.cpp"
#include "source/transform_component.cpp"
#include "source/importers/transition_viewmodel_condition_importer.cpp"
#include "source/importers/artboard_importer.cpp"
#include "source/importers/state_machine_importer.cpp"
#include "source/importers/linear_animation_importer.cpp"
#include "source/importers/layer_state_importer.cpp"
#include "source/importers/viewmodel_instance_importer.cpp"
#include "source/importers/enum_importer.cpp"
#include "source/importers/keyed_object_importer.cpp"
#include "source/importers/backboard_importer.cpp"
#include "source/importers/bindable_property_importer.cpp"
#include "source/importers/file_asset_importer.cpp"
#include "source/importers/state_machine_layer_component_importer.cpp"
#include "source/importers/data_converter_group_importer.cpp"
#include "source/importers/viewmodel_instance_list_importer.cpp"
#include "source/importers/state_transition_importer.cpp"
#include "source/importers/state_machine_layer_importer.cpp"
#include "source/importers/state_machine_listener_importer.cpp"
#include "source/importers/viewmodel_importer.cpp"
#include "source/importers/keyed_property_importer.cpp"
#include "source/nested_artboard_layout.cpp"
#include "source/component.cpp"
#include "source/layout/layout_component_style.cpp"
#include "source/layout/n_slicer_tile_mode.cpp"
#include "source/layout/n_slicer_details.cpp"
#include "source/layout/axis.cpp"
#include "source/layout/axis_y.cpp"
#include "source/layout/axis_x.cpp"
#include "source/layout/n_slicer.cpp"
#include "source/layout/n_sliced_node.cpp"
#include "source/node.cpp"
#include "source/layout.cpp"
#include "source/layout_component.cpp"
#include "source/animation/transition_value_string_comparator.cpp"
#include "source/animation/cubic_interpolator_solver.cpp"
#include "source/animation/nested_trigger.cpp"
#include "source/animation/keyframe_color.cpp"
#include "source/animation/keyframe_callback.cpp"
#include "source/animation/blend_animation.cpp"
#include "source/animation/listener_number_change.cpp"
#include "source/animation/state_machine_instance.cpp"
#include "source/animation/keyframe_interpolator.cpp"
#include "source/animation/transition_input_condition.cpp"
#include "source/animation/nested_bool.cpp"
#include "source/animation/blend_state_direct.cpp"
#include "source/animation/animation_state.cpp"
#include "source/animation/nested_animation.cpp"
#include "source/animation/keyframe_uint.cpp"
#include "source/animation/transition_trigger_condition.cpp"
#include "source/animation/state_machine_listener.cpp"
#include "source/animation/keyed_object.cpp"
#include "source/animation/nested_state_machine.cpp"
#include "source/animation/animation_reset_factory.cpp"
#include "source/animation/interpolating_keyframe.cpp"
#include "source/animation/state_machine.cpp"
#include "source/animation/nested_linear_animation.cpp"
#include "source/animation/blend_state_1d_instance.cpp"
#include "source/animation/state_machine_input_instance.cpp"
#include "source/animation/transition_viewmodel_condition.cpp"
#include "source/animation/keyframe_bool.cpp"
#include "source/animation/animation_state_instance.cpp"
#include "source/animation/nested_simple_animation.cpp"
#include "source/animation/transition_condition.cpp"
#include "source/animation/elastic_interpolator.cpp"
#include "source/animation/transition_comparator.cpp"
#include "source/animation/blend_state_direct_instance.cpp"
#include "source/animation/elastic_ease.cpp"
#include "source/animation/linear_animation_instance.cpp"
#include "source/animation/keyframe_double.cpp"
#include "source/animation/listener_input_change.cpp"
#include "source/animation/blend_animation_direct.cpp"
#include "source/animation/transition_property_comparator.cpp"
#include "source/animation/cubic_ease_interpolator.cpp"
#include "source/animation/blend_state_transition.cpp"
#include "source/animation/nested_number.cpp"
#include "source/animation/transition_property_artboard_comparator.cpp"
#include "source/animation/transition_number_condition.cpp"
#include "source/animation/listener_bool_change.cpp"
#include "source/animation/keyframe.cpp"
#include "source/animation/listener_action.cpp"
#include "source/animation/blend_state.cpp"
#include "source/animation/listener_fire_event.cpp"
#include "source/animation/state_machine_layer.cpp"
#include "source/animation/listener_align_target.cpp"
#include "source/animation/blend_animation_1d.cpp"
#include "source/animation/transition_value_enum_comparator.cpp"
#include "source/animation/blend_state_1d.cpp"
#include "source/animation/cubic_value_interpolator.cpp"
#include "source/animation/cubic_interpolator_component.cpp"
#include "source/animation/state_instance.cpp"
#include "source/animation/transition_value_color_comparator.cpp"
#include "source/animation/listener_trigger_change.cpp"
#include "source/animation/transition_value_boolean_comparator.cpp"
#include "source/animation/cubic_interpolator.cpp"
#include "source/animation/keyed_property.cpp"
#include "source/animation/transition_bool_condition.cpp"
#include "source/animation/state_machine_input.cpp"
#include "source/animation/animation_reset.cpp"
#include "source/animation/nested_remap_animation.cpp"
#include "source/animation/layer_state.cpp"
#include "source/animation/transition_value_number_comparator.cpp"
#include "source/animation/system_state_instance.cpp"
#include "source/animation/state_transition.cpp"
#include "source/animation/listener_viewmodel_change.cpp"
#include "source/animation/transition_property_viewmodel_comparator.cpp"
#include "source/animation/keyframe_string.cpp"
#include "source/animation/keyframe_id.cpp"
#include "source/drawable.cpp"
#include "source/shapes/paint/solid_color.cpp"
#include "source/shapes/paint/linear_gradient.cpp"
#include "source/shapes/paint/shape_paint.cpp"
#include "source/shapes/paint/fill.cpp"
#include "source/shapes/paint/dash_path.cpp"
#include "source/shapes/paint/radial_gradient.cpp"
#include "source/shapes/paint/color.cpp"
#include "source/shapes/paint/trim_path.cpp"
#include "source/shapes/paint/shape_paint_mutator.cpp"
#include "source/shapes/paint/gradient_stop.cpp"
#include "source/shapes/paint/dash.cpp"
#include "source/shapes/paint/stroke.cpp"
#include "source/shapes/points_path.cpp"
#include "source/shapes/star.cpp"
#include "source/shapes/mesh.cpp"
#include "source/shapes/cubic_asymmetric_vertex.cpp"
#include "source/shapes/image.cpp"
#include "source/shapes/cubic_detached_vertex.cpp"
#include "source/shapes/mesh_vertex.cpp"
#include "source/shapes/slice_mesh.cpp"
#include "source/shapes/cubic_vertex.cpp"
#include "source/shapes/shape.cpp"
#include "source/shapes/triangle.cpp"
#include "source/shapes/polygon.cpp"
#include "source/shapes/path.cpp"
#include "source/shapes/parametric_path.cpp"
#include "source/shapes/path_vertex.cpp"
#include "source/shapes/rectangle.cpp"
#include "source/shapes/straight_vertex.cpp"
#include "source/shapes/path_composer.cpp"
#include "source/shapes/shape_paint_container.cpp"
#include "source/shapes/cubic_mirrored_vertex.cpp"
#include "source/shapes/vertex.cpp"
#include "source/shapes/ellipse.cpp"
#include "source/shapes/clipping_shape.cpp"
#include "source/math/contour_measure.cpp"
#include "source/math/raw_path.cpp"
#include "source/math/bit_field_loc.cpp"
#include "source/math/aabb.cpp"
#include "source/math/mat2d_find_max_scale.cpp"
#include "source/math/path_measure.cpp"
#include "source/math/raw_path_utils.cpp"
#include "source/math/vec2d.cpp"
#include "source/math/mat2d.cpp"
#include "source/math/n_slicer_helpers.cpp"
#include "source/dependency_sorter.cpp"
#include "source/artboard.cpp"
#include "source/world_transform_component.cpp"
#include "source/draw_target.cpp"
#include "source/audio/audio_reader.cpp"
#include "source/audio/audio_engine.cpp"
#include "source/audio/audio_source.cpp"
#include "source/audio/audio_sound.cpp"
#include "source/container_component.cpp"
#include "source/file.cpp"
#include "source/data_bind/data_bind_context.cpp"
#include "source/data_bind/context/context_value_list.cpp"
#include "source/data_bind/context/context_value_number.cpp"
#include "source/data_bind/context/context_value_enum.cpp"
#include "source/data_bind/context/context_value_string.cpp"
#include "source/data_bind/context/context_value_trigger.cpp"
#include "source/data_bind/context/context_value_list_item.cpp"
#include "source/data_bind/context/context_value_color.cpp"
#include "source/data_bind/context/context_value.cpp"
#include "source/data_bind/context/context_value_boolean.cpp"
#include "source/data_bind/data_bind.cpp"
#include "source/data_bind/converters/data_converter.cpp"
#include "source/data_bind/converters/data_converter_to_string.cpp"
#include "source/data_bind/converters/data_converter_operation.cpp"
#include "source/data_bind/converters/data_converter_trigger.cpp"
#include "source/data_bind/converters/data_converter_group_item.cpp"
#include "source/data_bind/converters/data_converter_rounder.cpp"
#include "source/data_bind/converters/data_converter_group.cpp"
#include "source/data_bind/data_context.cpp"
#include "source/intrinsically_sizeable.cpp"
#include "source/bones/root_bone.cpp"
#include "source/bones/bone.cpp"
#include "source/bones/skinnable.cpp"
#include "source/bones/weight.cpp"
#include "source/bones/skin.cpp"
#include "source/bones/tendon.cpp"
#include "source/text/text.cpp"
#include "source/text/text_style_axis.cpp"
#include "source/text/text_modifier_group.cpp"
#include "source/text/text_variation_modifier.cpp"
#include "source/text/glyph_lookup.cpp"
#include "source/text/font_hb.cpp"
#include "source/text/text_modifier.cpp"
#include "source/text/line_breaker.cpp"
#include "source/text/text_style.cpp"
#include "source/text/text_style_feature.cpp"
#include "source/text/text_value_run.cpp"
#include "source/text/utf.cpp"
#include "source/text/raw_text.cpp"
#include "source/text/text_modifier_range.cpp"
#include "source/event.cpp"
#include "source/factory.cpp"
#include "source/assets/audio_asset.cpp"
#include "source/assets/image_asset.cpp"
#include "source/assets/font_asset.cpp"
#include "source/assets/file_asset_contents.cpp"
#include "source/assets/file_asset_referencer.cpp"
#include "source/assets/file_asset.cpp"
#include "source/joystick.cpp"
#include "source/solo.cpp"
#include "source/static_scene.cpp"
#include "source/renderer.cpp"
#include "source/audio_event.cpp"
#include "source/nested_artboard_leaf.cpp"

#if __clang__
 #pragma clang diagnostic pop
#elif __GNUC__
 #pragma GCC diagnostic pop
#elif _MSC_VER
 #pragma warning (pop)
#endif
