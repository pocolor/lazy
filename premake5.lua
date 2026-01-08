workspace "Lazy"
	architecture "x64"
	startproject "cli"

	configurations { "Debug", "Release" }

	filter "configurations:Debug"
		symbols "On"
		defines { "LZ_DEBUG" }

	filter "configurations:Release"
		optimize "Full"
		defines { "LZ_RELEASE" }

group "Core"
	include "ast"
	include "lexer"
	include "parser"
	include "sema"
	include "runtime"

group "Execution"
	include "interpreter"
	include "bytecode"
	include "vm"

group "Tools"
	include "cli"
	--include "repl"
	--include "compiler"
