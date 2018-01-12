```
3rdParty
├── glm (generally the latest release goes in here.)
│   ├── CMakeLists.txt
│   ├── common.hpp
│   ├── detail
│   │   ├── compute_vector_relational.hpp
│   │   ├── dummy.cpp
│   │   ├── _features.hpp
│   │   ├── _fixes.hpp
│   │   ├── func_common.inl
│   │   ├── func_common_simd.inl
│   │   ├── func_exponential.inl
│   │   ├── func_exponential_simd.inl
│   │   ├── func_geometric.inl
│   │   ├── func_geometric_simd.inl
│   │   ├── func_integer.inl
│   │   ├── func_integer_simd.inl
│   │   ├── func_matrix.inl
│   │   ├── func_matrix_simd.inl
│   │   ├── func_packing.inl
│   │   ├── func_packing_simd.inl
│   │   ├── func_trigonometric.inl
│   │   ├── func_trigonometric_simd.inl
│   │   ├── func_vector_relational.inl
│   │   ├── func_vector_relational_simd.inl
│   │   ├── glm.cpp
│   │   ├── _noise.hpp
│   │   ├── qualifier.hpp
│   │   ├── setup.hpp
│   │   ├── _swizzle_func.hpp
│   │   ├── _swizzle.hpp
│   │   ├── type_float.hpp
│   │   ├── type_gentype.hpp
│   │   ├── type_gentype.inl
│   │   ├── type_half.hpp
│   │   ├── type_half.inl
│   │   ├── type_int.hpp
│   │   ├── type_mat2x2.hpp
│   │   ├── type_mat2x2.inl
│   │   ├── type_mat2x3.hpp
│   │   ├── type_mat2x3.inl
│   │   ├── type_mat2x4.hpp
│   │   ├── type_mat2x4.inl
│   │   ├── type_mat3x2.hpp
│   │   ├── type_mat3x2.inl
│   │   ├── type_mat3x3.hpp
│   │   ├── type_mat3x3.inl
│   │   ├── type_mat3x4.hpp
│   │   ├── type_mat3x4.inl
│   │   ├── type_mat4x2.hpp
│   │   ├── type_mat4x2.inl
│   │   ├── type_mat4x3.hpp
│   │   ├── type_mat4x3.inl
│   │   ├── type_mat4x4.hpp
│   │   ├── type_mat4x4.inl
│   │   ├── type_mat4x4_simd.inl
│   │   ├── type_mat.hpp
│   │   ├── type_mat.inl
│   │   ├── type_vec1.hpp
│   │   ├── type_vec1.inl
│   │   ├── type_vec2.hpp
│   │   ├── type_vec2.inl
│   │   ├── type_vec3.hpp
│   │   ├── type_vec3.inl
│   │   ├── type_vec4.hpp
│   │   ├── type_vec4.inl
│   │   ├── type_vec4_simd.inl
│   │   ├── type_vec.hpp
│   │   ├── type_vec.inl
│   │   └── _vectorize.hpp
│   ├── exponential.hpp
│   ├── ext
│   │   ├── vec1.hpp
│   │   ├── vec1.inl
│   │   ├── vector_relational.hpp
│   │   └── vector_relational.inl
│   ├── ext.hpp
│   ├── fwd.hpp
│   ├── geometric.hpp
│   ├── glm.hpp
│   ├── gtc
│   │   ├── bitfield.hpp
│   │   ├── bitfield.inl
│   │   ├── color_space.hpp
│   │   ├── color_space.inl
│   │   ├── constants.hpp
│   │   ├── constants.inl
│   │   ├── epsilon.hpp
│   │   ├── epsilon.inl
│   │   ├── integer.hpp
│   │   ├── integer.inl
│   │   ├── matrix_access.hpp
│   │   ├── matrix_access.inl
│   │   ├── matrix_integer.hpp
│   │   ├── matrix_inverse.hpp
│   │   ├── matrix_inverse.inl
│   │   ├── matrix_transform.hpp
│   │   ├── matrix_transform.inl
│   │   ├── noise.hpp
│   │   ├── noise.inl
│   │   ├── packing.hpp
│   │   ├── packing.inl
│   │   ├── quaternion.hpp
│   │   ├── quaternion.inl
│   │   ├── quaternion_simd.inl
│   │   ├── random.hpp
│   │   ├── random.inl
│   │   ├── reciprocal.hpp
│   │   ├── reciprocal.inl
│   │   ├── round.hpp
│   │   ├── round.inl
│   │   ├── type_aligned.hpp
│   │   ├── type_precision.hpp
│   │   ├── type_precision.inl
│   │   ├── type_ptr.hpp
│   │   ├── type_ptr.inl
│   │   ├── ulp.hpp
│   │   ├── ulp.inl
│   │   ├── vec1.hpp
│   │   └── vec1.inl
│   ├── gtx
│   │   ├── associated_min_max.hpp
│   │   ├── associated_min_max.inl
│   │   ├── bit.hpp
│   │   ├── bit.inl
│   │   ├── closest_point.hpp
│   │   ├── closest_point.inl
│   │   ├── color_encoding.hpp
│   │   ├── color_encoding.inl
│   │   ├── color_space.hpp
│   │   ├── color_space.inl
│   │   ├── color_space_YCoCg.hpp
│   │   ├── color_space_YCoCg.inl
│   │   ├── common.hpp
│   │   ├── common.inl
│   │   ├── compatibility.hpp
│   │   ├── compatibility.inl
│   │   ├── component_wise.hpp
│   │   ├── component_wise.inl
│   │   ├── dual_quaternion.hpp
│   │   ├── dual_quaternion.inl
│   │   ├── euler_angles.hpp
│   │   ├── euler_angles.inl
│   │   ├── extended_min_max.hpp
│   │   ├── extended_min_max.inl
│   │   ├── extend.hpp
│   │   ├── extend.inl
│   │   ├── exterior_product.hpp
│   │   ├── exterior_product.inl
│   │   ├── fast_exponential.hpp
│   │   ├── fast_exponential.inl
│   │   ├── fast_square_root.hpp
│   │   ├── fast_square_root.inl
│   │   ├── fast_trigonometry.hpp
│   │   ├── fast_trigonometry.inl
│   │   ├── float_notmalize.inl
│   │   ├── functions.hpp
│   │   ├── functions.inl
│   │   ├── gradient_paint.hpp
│   │   ├── gradient_paint.inl
│   │   ├── handed_coordinate_space.hpp
│   │   ├── handed_coordinate_space.inl
│   │   ├── hash.hpp
│   │   ├── hash.inl
│   │   ├── integer.hpp
│   │   ├── integer.inl
│   │   ├── intersect.hpp
│   │   ├── intersect.inl
│   │   ├── io.hpp
│   │   ├── io.inl
│   │   ├── log_base.hpp
│   │   ├── log_base.inl
│   │   ├── matrix_cross_product.hpp
│   │   ├── matrix_cross_product.inl
│   │   ├── matrix_decompose.hpp
│   │   ├── matrix_decompose.inl
│   │   ├── matrix_factorisation.hpp
│   │   ├── matrix_factorisation.inl
│   │   ├── matrix_interpolation.hpp
│   │   ├── matrix_interpolation.inl
│   │   ├── matrix_major_storage.hpp
│   │   ├── matrix_major_storage.inl
│   │   ├── matrix_operation.hpp
│   │   ├── matrix_operation.inl
│   │   ├── matrix_query.hpp
│   │   ├── matrix_query.inl
│   │   ├── matrix_transform_2d.hpp
│   │   ├── matrix_transform_2d.inl
│   │   ├── mixed_product.hpp
│   │   ├── mixed_product.inl
│   │   ├── normal.hpp
│   │   ├── normal.inl
│   │   ├── normalize_dot.hpp
│   │   ├── normalize_dot.inl
│   │   ├── norm.hpp
│   │   ├── norm.inl
│   │   ├── number_precision.hpp
│   │   ├── number_precision.inl
│   │   ├── optimum_pow.hpp
│   │   ├── optimum_pow.inl
│   │   ├── orthonormalize.hpp
│   │   ├── orthonormalize.inl
│   │   ├── perpendicular.hpp
│   │   ├── perpendicular.inl
│   │   ├── polar_coordinates.hpp
│   │   ├── polar_coordinates.inl
│   │   ├── projection.hpp
│   │   ├── projection.inl
│   │   ├── quaternion.hpp
│   │   ├── quaternion.inl
│   │   ├── range.hpp
│   │   ├── raw_data.hpp
│   │   ├── raw_data.inl
│   │   ├── rotate_normalized_axis.hpp
│   │   ├── rotate_normalized_axis.inl
│   │   ├── rotate_vector.hpp
│   │   ├── rotate_vector.inl
│   │   ├── scalar_multiplication.hpp
│   │   ├── scalar_relational.hpp
│   │   ├── scalar_relational.inl
│   │   ├── spline.hpp
│   │   ├── spline.inl
│   │   ├── std_based_type.hpp
│   │   ├── std_based_type.inl
│   │   ├── string_cast.hpp
│   │   ├── string_cast.inl
│   │   ├── texture.hpp
│   │   ├── texture.inl
│   │   ├── transform2.hpp
│   │   ├── transform2.inl
│   │   ├── transform.hpp
│   │   ├── transform.inl
│   │   ├── type_aligned.hpp
│   │   ├── type_aligned.inl
│   │   ├── type_trait.hpp
│   │   ├── type_trait.inl
│   │   ├── vec_swizzle.hpp
│   │   ├── vector_angle.hpp
│   │   ├── vector_angle.inl
│   │   ├── vector_query.hpp
│   │   ├── vector_query.inl
│   │   ├── wrap.hpp
│   │   └── wrap.inl
│   ├── integer.hpp
│   ├── mat2x2.hpp
│   ├── mat2x3.hpp
│   ├── mat2x4.hpp
│   ├── mat3x2.hpp
│   ├── mat3x3.hpp
│   ├── mat3x4.hpp
│   ├── mat4x2.hpp
│   ├── mat4x3.hpp
│   ├── mat4x4.hpp
│   ├── matrix.hpp
│   ├── packing.hpp
│   ├── simd
│   │   ├── common.h
│   │   ├── exponential.h
│   │   ├── geometric.h
│   │   ├── integer.h
│   │   ├── matrix.h
│   │   ├── packing.h
│   │   ├── platform.h
│   │   ├── trigonometric.h
│   │   └── vector_relational.h
│   ├── trigonometric.hpp
│   ├── vec2.hpp
│   ├── vec3.hpp
│   ├── vec4.hpp
│   └── vector_relational.hpp
├── libpsvr
│   ├── include
│   │   ├── Control_Frame.h
│   │   ├── Device_Info.h
│   │   ├── Device_Status.h
│   │   ├── morpheus.h
│   │   ├── PSVR_Common.h
│   │   ├── psvr.h
│   │   ├── PSVR_Lib.h
│   │   ├── ReportID.h
│   │   ├── Sensor_Frame.h
│   │   └── Unsolicited_Report.h
│   ├── lib32
│   │   ├── Debug
│   │   │   └── LibPSVR.lib
│   │   └── Release
│   │       └── LibPSVR.lib
│   └── lib64
│       ├── Debug
│       │   └── LibPSVR.lib
│       └── Release
│           └── LibPSVR.lib
├── libusb (windows only)
│   ├── include
│   │   ├── hotplug.h
│   │   ├── libusb.h
│   │   ├── libusbi.h
│   │   ├── os
│   │   │   ├── darwin_usb.h
│   │   │   ├── haiku_usb.h
│   │   │   ├── haiku_usb_raw.h
│   │   │   ├── linux_usbfs.h
│   │   │   ├── poll_posix.h
│   │   │   ├── poll_windows.h
│   │   │   ├── sunos_usb.h
│   │   │   ├── threads_posix.h
│   │   │   ├── threads_windows.h
│   │   │   ├── wince_usb.h
│   │   │   ├── windows_common.h
│   │   │   ├── windows_nt_common.h
│   │   │   ├── windows_usbdk.h
│   │   │   └── windows_winusb.h
│   │   ├── version.h
│   │   └── version_nano.h
│   ├── lib32
│   │   ├── Debug
│   │   │   └── libusb-1.0.lib
│   │   └── Release
│   │       └── libusb-1.0.lib
│   └── lib64
│       ├── Debug
│       │   └── libusb-1.0.lib
│       └── Release
│           └── libusb-1.0.lib
└── openvr
    ├── include
    │   ├── openvr_api.cs
    │   ├── openvr_api.json
    │   ├── openvr_capi.h
    │   ├── openvr_driver.h
    │   └── openvr.h
    ├── lib32
    │   └── openvr_api.lib
    └── lib64
        └── openvr_api.lib
```