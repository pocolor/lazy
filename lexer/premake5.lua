project "lexer"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"

	targetdir ("%{wks.location}/bin/%{cfg.buildcfg}")
	objdir ("%{wks.location}/bin-int")  -- /%{cfg.buildcfg} is not necessary

	files {
		"src/**.cpp",
		"include/**.h"
	}

	includedirs {
		"include",
		"../ast/include"
	}

	links {
		"ast"
	}
