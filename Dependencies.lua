IncludeDir = {}
IncludeDir["SDL3"] = "%{wks.location}/TwoD/vendor/SDL3/include"
IncludeDir["SDL3_image"] = "%{wks.location}/TwoD/vendor/SDL3_image/include"
IncludeDir["SDL3_shadercross"] = "%{wks.location}/TwoD/vendor/SDL3_shadercross/include"
IncludeDir["msdf_atlas_gen"] = "%{wks.location}/TwoDLib/vendor/msdf-atlas-gen"
IncludeDir["msdfgen"] = "%{wks.location}/TwoDLib/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["yaml_cpp"] = "%{wks.location}/TwoD/vendor/yaml-cpp/include"
IncludeDir["glm"] = "%{wks.location}/TwoD/vendor/glm"

Library = {}
Library["SDL3"] = "%{wks.location}/TwoD/vendor/SDL3/lib/SDL3.lib"
Library["SDL3_image"] = "%{wks.location}/TwoD/vendor/SDL3_image/lib/SDL3_image.lib"
Library["SDL3_shadercross"] = "%{wks.location}/TwoD/vendor/SDL3_shadercross/lib/SDL3_shadercross.lib"

Dynamic = {}
Dynamic["SDL3"] = "%{wks.location}/TwoD/vendor/SDL3/lib/SDL3.dll"
Dynamic["SDL3_image"] = "%{wks.location}/TwoD/vendor/SDL3_image/lib/SDL3_image.dll"
Dynamic["SDL3_shadercross"] = "%{wks.location}/TwoD/vendor/SDL3_shadercross/bin/*.dll"